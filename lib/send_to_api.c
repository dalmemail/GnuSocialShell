/*
 * Copyright (C) 2016 Dan Rulos.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <curl/curl.h>
#include "gnusocial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "constants.h"

extern int loglevel;

struct Chunk {
  char *memory;
  size_t size;
};

/* This in-memory cURL callback is from
   https://curl.haxx.se/libcurl/c/getinmemory.html */
static size_t cb_writeXmlChunk(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct Chunk *mem = (struct Chunk *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    /* out of memory! */ 
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

char *send_to_api(struct gss_account account, char *send, char *xml_doc)
{
        CURLcode err;
	char url[MAX_URL];
	char userpwd[129];
	snprintf(userpwd, 129, "%s:%s", account.user, account.password);
	snprintf(url, MAX_URL, "%s://%s/api/%s", account.protocol, account.server, xml_doc);
	struct Chunk xml;
	xml.memory = (char *)malloc(1);
	xml.size = 0; 
	CURL *curl = curl_easy_init();
	// libcurl never reads .curlrc:
	curl_easy_setopt(curl, CURLOPT_CAPATH, "/etc/ssl/certs/" );
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb_writeXmlChunk);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&xml);

	if (send != NULL) {
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, send);
	}

	if (loglevel > LOG_NONE) {
	        char errbuf[CURL_ERROR_SIZE];
		err = curl_easy_perform(curl);
		size_t len = strlen(errbuf);
		switch (err) {
		case CURLE_OK:
  		        break;
		default:
		  fprintf(stderr, "\nlibcurl: error (%d) ", err);
		  if(len)
		    fprintf(stderr, "%s%s", errbuf,
			    ((errbuf[len - 1] != '\n') ? "\n" : ""));
		  else
		    fprintf(stderr, "%s\n", curl_easy_strerror(err));
		}
	} else {
	        curl_easy_perform(curl);
	}

        curl_easy_cleanup(curl);
	return xml.memory;
}
