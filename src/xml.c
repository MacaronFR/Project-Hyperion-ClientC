#include <xml.h>

xmlDocPtr createProductsXml(){
	xmlNodePtr *products;
	MYSQL *bdd = createHyperionBDD();
	queryResult *tab = selectProduct(bdd);
	products = prepareProductList(bdd, tab);
	unsigned long long int nproduct = tab->rows;
	xmlDocPtr doc = NULL;
	xmlNodePtr root;
	freeResult(tab);
	doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "products");
	xmlDocSetRootElement(doc, root);
	for(int i = 0; i < nproduct; ++i){
		xmlAddChild(root, products[i]);
	}
	free(products);
	mysql_close(bdd);
	return doc;
}

xmlNodePtr *prepareProductList(MYSQL *bdd, queryResult *products){
	xmlNodePtr *xmlProducts = malloc(sizeof(xmlNodePtr) * (products->rows + 1));
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	char *month = malloc(3);
	sprintf(month, "%02d", local->tm_mon+1);
	queryResult *bddSpec;
	char ***spec;
	char *brand, *type, *model, *bought, *sold, *buying_price, *selling_price;
	int prod_flag = 0;
	for(int i = 0; i < products->rows; ++i){
		bddSpec = selectSpec(bdd, products->value[i][3], products->value[i][5]);
		type = products->value[i][0];
		bought = products->value[i][6];
		sold = products->value[i][7];
		buying_price = products->value[i][2];
		selling_price = products->value[i][1];
		spec = prepareSpec(bddSpec, &model, &brand);
		if(sold == NULL){
			prod_flag = BOUGHT;
		}else{
			if(strncmp(month, products->value[i][6] + 5, 2) == 0)
				prod_flag |= BOUGHT;
			if(strncmp(month, products->value[i][7] + 5, 2) == 0)
				prod_flag |= SOLD;
		}
		xmlProducts[i] = newProduct(brand, type, model, bought, sold, buying_price, selling_price, spec, prod_flag);
		free(spec);
		freeResult(bddSpec);
	}
	free(month);
	return xmlProducts;
}

char ***prepareSpec(queryResult *spec, char **model, char **brand){
	int delta = 0;
	char *** result = malloc(sizeof(char **) * spec->rows - 2);
	for(int j = 0; j < spec->rows; ++j){
		if(strcmp(spec->value[j][0], "brand") == 0){
			*brand = spec->value[j][1];
			delta++;
		}else if(strcmp(spec->value[j][0], "model") == 0){
			*model = spec->value[j][1];
			delta++;
		}else{
			result[j - delta] = spec->value[j];
		}
		result[spec->rows - 2] = NULL;
	}
	return result;
}

xmlNodePtr newProduct(char *brand, char *type, char *model, char *buying_date, char *selling_date, char *buying_price, char *selling_price, char ***specification, int flag){
    xmlNodePtr product = NULL;
    xmlNodePtr spec;
    int i = 0;
    if(brand != NULL && type != NULL && model != NULL) {
        product = xmlNewNode(NULL, BAD_CAST "product");
        xmlNewProp(product, BAD_CAST "brand", BAD_CAST brand);
        xmlNewProp(product, BAD_CAST "model", BAD_CAST model);
        xmlNewProp(product, BAD_CAST "type", BAD_CAST type);
        xmlNewProp(product, BAD_CAST "buying_date", BAD_CAST buying_date);
        xmlNewProp(product, BAD_CAST "selling_date", BAD_CAST selling_date);
        xmlNewProp(product, BAD_CAST "buying_price", BAD_CAST buying_price);
        xmlNewProp(product, BAD_CAST "selling_price", BAD_CAST selling_price);
		if(flag & BOUGHT)
			xmlNewProp(product, BAD_CAST "bought", BAD_CAST"yes");
		if(flag & SOLD)
			xmlNewProp(product, BAD_CAST "sold", BAD_CAST"yes");
        if(specification != NULL){
            while(specification[i] != NULL){
                spec = xmlNewChild(product, NULL, BAD_CAST"specification", NULL);
                xmlNewProp(spec, BAD_CAST specification[i][0], BAD_CAST specification[i][1]);
                ++i;
            }
        }
    }
    return product;
}