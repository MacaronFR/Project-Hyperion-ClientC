#include <xml.h>

xmlNodePtr *prepareProductList(MYSQL *bdd, queryResult *products){
	xmlNodePtr *xmlProducts = malloc(sizeof(xmlNodePtr) * (products->rows + 1));
	queryResult *bddSpec;
	char ***spec;
	char *mark, *type, *model;
	for(int i = 0; i < products->rows; ++i){
		bddSpec = selectSpec(bdd, products->value[i][3], products->value[i][5]);
		type = products->value[i][0];
		spec = prepareSpec(bddSpec, &model, &mark);
		xmlProducts[i] = newProduct(mark, type, model, spec);
		free(spec);
		freeResult(bddSpec);
	}
	return xmlProducts;
}

char ***prepareSpec(queryResult *spec, char **model, char **mark){
	int delta = 0;
	char *** result = malloc(sizeof(char **) * spec->rows - 2);
	for(int j = 0; j < spec->rows; ++j){
		if(strcmp(spec->value[j][0], "mark") == 0){
			*mark = spec->value[j][1];
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

xmlNodePtr newProduct(char *mark, char *type, char *model, char ***specification){
    xmlNodePtr product = NULL;
    xmlNodePtr spec;
    int i = 0;
    if(mark != NULL && type != NULL && model != NULL) {
        product = xmlNewNode(NULL, BAD_CAST "product");
        xmlNewProp(product, BAD_CAST "mark", BAD_CAST mark);
        xmlNewProp(product, BAD_CAST "model", BAD_CAST model);
        xmlNewProp(product, BAD_CAST "type", BAD_CAST type);
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