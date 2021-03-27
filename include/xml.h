#ifndef CLIENTC_XML_H
#define CLIENTC_XML_H

#include <libxml/tree.h>

xmlNodePtr newProduct(char *mark, char *type, char *model, char ***specification);

#endif //CLIENTC_XML_H
