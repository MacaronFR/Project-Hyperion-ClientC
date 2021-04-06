#include <xml.h>
#include <libxml/parser.h>
#include <curl.h>

int main(int argc, char **argv){
    if(argc == 3 && strcmp(argv[1], "--test") == 0){
        if(strcmp(argv[2], "BDD") == 0){
            MYSQL * test = createHyperionBDD();
            if(test != NULL){
                mysql_close(test);
                return 0;
            }
            return 1;
        }else if(strcmp(argv[2], "SFTP") == 0){
            return sendFile("sftp.test");
        }
    }
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	char **deposit = get_credentials("[DEPOSIT]");
	char *id = malloc(strlen(deposit[0]) + strlen(deposit[1]) + 1);
	char *path;
	strcpy(id, deposit[0]);
	strcat(id, deposit[1]);
	int i = 0;
	char *filename = malloc(strlen(id) + 9);
	if(local->tm_mday == 1){
		sprintf(filename, "%s%02d%02d.xml", id, local->tm_mon, local->tm_year % 100);
		sendFile(filename);
	}
	sprintf(filename, "%s%02d%02d.xml", id, local->tm_mon+1, local->tm_year % 100);
	if(deposit[2][strlen(deposit[2]) - 1] != '/') {
        path = malloc(strlen(filename) + strlen(deposit[2]) + 2);
        strcpy(path, deposit[2]);
        strcat(path, "/");
    }else{
        path = malloc(strlen(filename) + strlen(deposit[2]) + 1);
        strcpy(path, deposit[2]);
	}
    strcat(path, filename);
	xmlDocPtr doc = createProductsXml();
	xmlSaveFormatFileEnc(path, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
    while(deposit[i] != NULL){
        free(deposit[i]);
        ++i;
    }
    free(deposit);
	free(filename);
	free(id);
	free(path);
	return 0;
}