#include <curl.h>

int sendFile(char *file){
	CURL *curl;
	CURLcode res;
	FILE *f;
	curl_off_t  fsize;
	int i = 0;
	char **info = get_credentials("[DEPOSIT]");
	char **credentials = get_credentials("[SFTP]");
	char *remote_path = "/web/hyperion/";
	char *url = malloc(strlen(credentials[0]) + strlen(credentials[3]) + strlen(remote_path) + strlen(file) + 9);
	char *localfile = malloc(strlen(info[2]) + strlen(file) + 1);
	strcpy(localfile, info[2]);
	strcat(localfile, file);
	strcpy(url, "sftp://");
	strcat(url, credentials[0]);
	strcat(url, ":");
	strcat(url, credentials[3]);
	strcat(url, remote_path);
	strcat(url, file);
	f = fopen(localfile, "rb");
	if(f == NULL)
        return 1;
	curl_global_init(CURL_GLOBAL_ALL);
	curl = curl_easy_init();
	if(curl){
		curl_easy_setopt(curl, CURLOPT_USERNAME, credentials[1]);
		curl_easy_setopt(curl, CURLOPT_PASSWORD, credentials[2]);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_READDATA, f);
		res = curl_easy_perform(curl);
		if(res != CURLE_OK){
			printf("%s\n", curl_easy_strerror(res));
			return 2;
		}
		curl_easy_cleanup(curl);
	}
	curl_global_cleanup();
	while(credentials[i] != NULL){
		free(credentials[i]);
		++i;
	}
	free(credentials);
	free(url);
	fclose(f);
}