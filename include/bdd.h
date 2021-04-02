#ifndef CLIENTC_BDD_H
#define CLIENTC_BDD_H

#include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct queryResult{
    unsigned long long int rows;
    unsigned int fields;
    char **fields_name;
    char ***value;
} queryResult;

MYSQL *createHyperionBDD();
queryResult *fetch(MYSQL *bdd);
queryResult *selectProduct(MYSQL *bdd);
queryResult *selectRefSpec(MYSQL *bdd, char *id);
queryResult *selectProdSpec(MYSQL *bdd, char *id);
queryResult *selectSpec(MYSQL *bdd, char *id_prod, char *id_ref);
void deleteRow(queryResult *result, int index);
void freeResult(queryResult *tab);

#endif //CLIENTC_BDD_H
