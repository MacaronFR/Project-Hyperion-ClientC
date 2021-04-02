#ifndef CLIENTC_CURL_H
#define CLIENTC_CURL_H

#include <curl/curl.h>
#include <utils.h>
#include <sys/types.h>
#include <sys/stat.h>

void sendFile(char *file);

#endif //CLIENTC_CURL_H
