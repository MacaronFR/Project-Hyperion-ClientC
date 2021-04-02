#include <bdd.h>

MYSQL *createHyperionBDD(){
	int i;
	MYSQL *mysql = NULL;
	char **credentials = get_credentials("[BDD]");
	if((mysql = mysql_init(NULL)) == NULL){
		fprintf(stderr, "Error initializing MYSQL instance");
		return NULL;
	}
//    my_bool yes = 1;
//    mysql_optionsv(mysql, MYSQL_OPT_SSL_ENFORCE, &yes);
	if(mysql_real_connect(
			mysql,
			credentials[0], credentials[1], credentials[2], credentials[3], strtol(credentials[4], NULL, 10),
			NULL, CLIENT_REMEMBER_OPTIONS
	) == NULL){
		fprintf(stderr, "Error %u : %s", mysql_errno(mysql), mysql_error(mysql));
		return NULL;
	}
	i = 0;
	while(credentials[i] != NULL){
		free(credentials[i]);
		++i;
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
			if(row[j] != NULL){
				tab->value[i][j] = malloc(strlen(row[j]) + 1);
				strcpy(tab->value[i][j], row[j]);
			}else{
				tab->value[i][j] = NULL;
			}
		}
	}
	mysql_free_result(res);
	return tab;
}

queryResult *selectProduct(MYSQL *bdd){
	const char *query_product = "SELECT T.type, PROD.selling_price, PROD.buying_price, PROD.id_product, state, REF.id_product, buying_date, selling_date as id_ref FROM PRODUCTS PROD INNER JOIN REFERENCE_PRODUCTS REF ON PROD.id_ref = REF.id_product INNER JOIN TYPES T on REF.type = T.id_type WHERE MONTH(PROD.buying_date)=MONTH(CURRENT_DATE) OR MONTH(PROD.selling_date)=MONTH(CURRENT_DATE)";
	mysql_query(bdd, query_product);
	return fetch(bdd);
}

queryResult *selectRefSpec(MYSQL *bdd, char *id){
	const char *query_spec = "SELECT name, value FROM REF_HAVE_SPEC INNER JOIN SPECIFICATION S on REF_HAVE_SPEC.id_spec = S.id_specification WHERE id_product=";
	char *prepared_query = malloc(strlen(query_spec) + strlen(id) + 2);
	strcpy(prepared_query, query_spec);
	strcat(prepared_query, id);
	strcat(prepared_query, ";");
	mysql_query(bdd, prepared_query);
	free(prepared_query);
	return fetch(bdd);
}

queryResult *selectProdSpec(MYSQL *bdd, char *id){
	const char *query_spec = "SELECT name, value FROM PRODUCT_HAVE_SPEC INNER JOIN SPECIFICATION S on PRODUCT_HAVE_SPEC.id_spec = S.id_specification WHERE id_product=";
	char *prepared_query = malloc(strlen(query_spec) + strlen(id) + 2);
	strcpy(prepared_query, query_spec);
	strcat(prepared_query, id);
	strcat(prepared_query, ";");
	mysql_query(bdd, prepared_query);
	free(prepared_query);
	return fetch(bdd);
}

void deleteRow(queryResult *result, int index){
	for(int i = 0; i < result->fields; ++i){
		free(result->value[index][i]);
	}
	free(result->value[index]);
	for(int i = index; i < result->rows - 1; ++i){
		result->value[i] = result->value[i+1];
	}
	(result->rows)--;
}

queryResult *selectSpec(MYSQL *bdd, char *id_prod, char *id_ref){
	queryResult *refspec = selectRefSpec(bdd, id_ref);
	queryResult *refprod = selectProdSpec(bdd, id_prod);
	for(int i = 0; i < refspec->rows; ++i){
		for(int j = 0; j < refprod->rows; ++j){
			if(strcmp(refspec->value[i][0], refprod->value[j][0]) == 0 && strcmp(refspec->value[i][1], refprod->value[j][1]) != 0){
				deleteRow(refspec, i);
				--i;
				break;
			}
		}
	}
	freeResult(refprod);
	return refspec;
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