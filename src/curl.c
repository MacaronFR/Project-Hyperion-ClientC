#include <curl.h>

void sendFile(char *file){
	CURL *curl;
	CURLcode res;
	FILE *f;
	struct stat file_info;
	curl_off_t  fsize;
	struct curl_slist *headerlist = NULL;
	int i = 0;
	char *buf1 = malloc(strlen(file) + 6);
	char *buf2 = malloc(strlen(file) + 6);
	char **credentials = get_credentials("[SFTP]");
	char *url = malloc(strlen(credentials[0]) + strlen(file) + 21);
	strcpy(buf1, "RNFR ");
	strcpy(buf2, "RNTO ");
	strcat(buf1, file);
	strcat(buf2, file);
	strcpy(url, "ftp://");
	strcat(url, credentials[0]);
	strcat(url, "/");
	strcat(url, file);
	if(stat(file, &file_info)){
		return;
	}
	fsize = (curl_off_t)file_info.st_size;
	f = fopen(file, "rb");
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl){
		headerlist = curl_slist_append(headerlist, buf1);
		headerlist = curl_slist_append(headerlist, buf2);
		curl_easy_setopt(curl, CURLOPT_USERNAME, credentials[1]);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, credentials[2]);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
		curl_easy_setopt(curl, CURLOPT_READDATA, f);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)fsize);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
			printf("NIKK %s\n", curl_easy_strerror(res));
		}
		curl_slist_free_all(headerlist);
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	while(credentials[i] != NULL){
		free(credentials[i]);
		++i;
	}
	free(credentials);
	free(url);
	free(buf1);
	free(buf2);
	fclose(f);
}