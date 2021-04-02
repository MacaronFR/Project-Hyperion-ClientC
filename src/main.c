#include <xml.h>
#include <libxml/parser.h>
#include <curl.h>

int main(){
	time_t now;
	time(&now);
	struct tm *local = localtime(&now);
	char *id = "01basti";
	char *filename = malloc(strlen(id) + 9);
	if(local->tm_mday == 1){
		sprintf(filename, "%s%02d%02d.xml", id, local->tm_mon, local->tm_year % 100);
		sendFile(filename);
	}
	sprintf(filename, "%s%02d%02d.xml", id, local->tm_mon+1, local->tm_year % 100);
	xmlDocPtr doc = createProductsXml();
	xmlSaveFormatFileEnc(filename, doc, "UTF-8", 1);
	xmlFreeDoc(doc);
	xmlCleanupParser();
	return 0;
}