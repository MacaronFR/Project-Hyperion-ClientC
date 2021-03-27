#include <bdd.h>

MYSQL *createHyperionBDD(){
	MYSQL *mysql = NULL;
	if((mysql = mysql_init(NULL)) == NULL){
		fprintf(stderr, "Error initializing MYSQL instance");
		return NULL;
	}
//    my_bool yes = 1;
//    mysql_optionsv(mysql, MYSQL_OPT_SSL_ENFORCE, &yes);
	if(mysql_real_connect(
			mysql,
			"hyperion.dev.macaron-dev.fr", "creader", "creaderdeb1", "HYPERION", 3306,
			NULL, CLIENT_REMEMBER_OPTIONS
	) == NULL){
		fprintf(stderr, "Error %u : %s", mysql_errno(mysql), mysql_error(mysql));
		return NULL;
	}
	return mysql;
}

queryResult *fetch(MYSQL *bdd){
	MYSQL_RES *res = mysql_store_result(bdd);
	MYSQL_FIELD *field;
	MYSQL_ROW row;
	queryResult *tab = malloc(sizeof(queryResult));
	tab->rows = res->row_count;
	tab->value = malloc(sizeof(char **) * tab->rows);
	tab->fields = res->field_count;
	tab->fields_name = malloc(sizeof(char *) * tab->fields);
	for(int i = 0; i < tab->fields; ++i){
		field = mysql_fetch_field(res);
		tab->fields_name[i] = malloc(strlen(field->name) + 1);
		strcpy(tab->fields_name[i], field->name);
	}
	for(int i = 0; i < tab->rows; ++i){
		row = mysql_fetch_row(res);
		tab->value[i] = malloc(sizeof(char *) * tab->fields);
		for(int j = 0; j < tab->fields; ++j){
			tab->value[i][j] = malloc(strlen(row[j]) + 1);
			strcpy(tab->value[i][j], row[j]);
		}
	}
	mysql_free_result(res);
	return tab;
}

queryResult *selectProduct(MYSQL *bdd){
	const char *query_product = "SELECT T.type, PRODUCTS.selling_price, PRODUCTS.buying_price, PRODUCTS.id_product, state FROM PRODUCTS INNER JOIN REFERENCE_PRODUCTS RP on PRODUCTS.id_ref = RP.id_product INNER JOIN TYPES T on RP.type = T.id_type;";
	mysql_query(bdd, query_product);
	return fetch(bdd);
}

queryResult *selectSpec(MYSQL *bdd, char *id){
	const char *query_spec = "SELECT S.name, S.value FROM PRODUCTS INNER JOIN REFERENCE_PRODUCTS RP on PRODUCTS.id_ref = RP.id_product INNER JOIN REF_HAVE_SPEC RHS on RP.id_product = RHS.id_product INNER JOIN SPECIFICATION S on RHS.id_spec = S.id_specification WHERE PRODUCTS.id_product=";
	char *prepared_query = malloc(strlen(query_spec) + strlen(id) + 2);
	strcpy(prepared_query, query_spec);
	strcat(prepared_query, id);
	strcat(prepared_query, ";");
	mysql_query(bdd, prepared_query);
	return fetch(bdd);
}

void freeResult(queryResult *tab){
	for(int i = 0; i < tab->fields; ++i){
		free(tab->fields_name[i]);
	}
	free(tab->fields_name);
	for(int i = 0; i < tab->rows; ++i){
		for(int j = 0; j < tab->fields; ++j){
			free(tab->value[i][j]);
		}
		free(tab->value[i]);
	}
	free(tab->value);
	free(tab);
}