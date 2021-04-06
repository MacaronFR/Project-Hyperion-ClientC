#include <utils.h>

char **get_credentials(char *section){
	FILE *f = fopen("/etc/hyperion/database_saver.conf", "rb");
	char **credentials = NULL;
	int line = 0;
	if(f == NULL){
		exit(1);
	}
	char buf[256];
	do{
		if(!fgets(buf, 255, f))
			break;
		buf[strlen(buf) - 1] = buf[strlen(buf) - 1] == '\n' ? 0 : buf[strlen(buf) - 1];
	}while(strcmp(buf, section) != 0);
	do{
		if(!fgets(buf, 255, f))
			break;
		buf[strlen(buf) - 1] = buf[strlen(buf) - 1] == '\n' ? 0 : buf[strlen(buf) - 1];
		line++;
		credentials = realloc(credentials, sizeof(char *) * line);
		credentials[line-1] = malloc(strlen(buf) + 1);
		strcpy(credentials[line-1], buf);
	}while(buf[0] != 0);
	free(credentials[line-1]);
	credentials[line-1] = NULL;
	return credentials;
}