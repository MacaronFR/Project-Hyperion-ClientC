#include <xml.h>
#include <bdd.h>
#include <libxml/parser.h>

int main(){
	xmlNodePtr *products;
	char *mark, *type, *model;
	char ***spec;
	MYSQL *bdd = createHyperionBDD();
	queryResult *tab = selectProduct(bdd);
	products = prepareProductList(bdd, tab);
	unsigned long long int nproduct = tab->rows;
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