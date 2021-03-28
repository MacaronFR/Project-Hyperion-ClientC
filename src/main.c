#include <xml.h>
#include <bdd.h>
#include <libxml/parser.h>

int main(){
	xmlNodePtr *products;
	char *mark, *type, *model;
	char ***spec;
	MYSQL *bdd = createHyperionBDD();
	queryResult *tab = selectProduct(bdd);
	queryResult *tab2;
	int delta;
	unsigned long long int nproduct = tab->rows;
	products = malloc(sizeof(xmlNodePtr) * (nproduct + 1));
	for(int i = 0; i < tab->rows; ++i){
		delta = 0;
		type = tab->value[i][0];
		tab2 = selectSpec(bdd, tab->value[i][3]);
		spec = malloc(sizeof(char **) * tab2->rows - 2);
		for(int j = 0; j < tab2->rows; ++j){
			if(strcmp(tab2->value[j][0], "marque") == 0){
				mark = tab2->value[j][1];
				delta++;
			}else if(strcmp(tab2->value[j][0], "modele") == 0){
				model = tab2->value[j][1];
				delta++;
			}else{
				spec[j - delta] = tab2->value[j];
			}
		}
		spec[tab2->rows - 2] = NULL;
		products[i] = newProduct(mark, type, model, spec);
		free(spec);
		freeResult(tab2);
	}
	freeResult(tab);
	xmlDocPtr doc = NULL;
	xmlNodePtr root;
	doc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "products");
	xmlDocSetRootElement(doc, root);
	//xmlCreateIntSubset(doc, BAD_CAST"products", NULL, BAD_CAST "tree.dtd");
	for(int i = 0; i < nproduct; ++i){
		xmlAddChild(root, products[i]);
	}
	xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return 0;
}