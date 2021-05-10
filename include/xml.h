#ifndef CLIENTC_XML_H
#define CLIENTC_XML_H

#include <libxml/tree.h>
#include <bdd.h>
#include <time.h>

#define BOUGHT 1
#define SOLD 2

xmlDocPtr createProductsXml();
xmlNodePtr *prepareProductList(MYSQL *bdd, queryResult *products);
char ***prepareSpec(queryResult *spec, char **model, char **brand);
xmlNodePtr newProduct(char *brand, char *type, char *model, char*buying_date, char *selling_date, char *buying_price, char *selling_price, char ***specification, int flag);

#endif //CLIENTC_XML_H
