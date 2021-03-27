#include <xml.h>

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