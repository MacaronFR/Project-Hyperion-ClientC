#ifndef CLIENTC_XML_H
#define CLIENTC_XML_H

#include <libxml/tree.h>
#include <bdd.h>

xmlNodePtr *prepareProductList(MYSQL *bdd, queryResult *products);
char ***prepareSpec(queryResult *spec, char **model, char **mark);
xmlNodePtr newProduct(char *mark, char *type, char *model, char ***specification);

#endif //CLIENTC_XML_H
