#include <curl.h>

void sendFile(char *file){
	CURL *curl;
	CURLcode res;
	FILE *f;
	struct stat file_info;
	curl_off_t  fsize;
	struct curl_slist *headerlist = NULL;
	char *buf1 = malloc(strlen(file) + 6);
	char *buf2 = malloc(strlen(file) + 6);
	strcpy(buf1, "RNFR ");
	strcpy(buf2, "RNTO ");
	strcat(buf1, file);
	strcat(buf2, file);
	if(stat(file, &file_info)){
		return;
	}
	fsize = (curl_off_t)file_info.st_size;
	printf("%"CURL_FORMAT_CURL_OFF_T"\n", fsize);
	f = fopen(file, "rb");
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl){
		headerlist = curl_slist_append(headerlist, buf1);
		headerlist = curl_slist_append(headerlist, buf2);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	}
	free(buf1);
	free(buf2);
	fclose(f);
}