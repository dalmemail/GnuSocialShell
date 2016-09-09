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

char *send_to_api(struct gss_account account, char *send, char *xml_doc)
{
        CURLcode err;
	char url[128];
	sprintf(url, "%s://%s/api/%s", account.protocol, account.server, xml_doc);
	FILE *xml = fopen("temp/file.xml", "wb");
	CURL *curl = curl_easy_init();
	// libcurl never reads .curlrc:
	curl_easy_setopt(curl, CURLOPT_CAPATH, "/etc/ssl/certs/" );
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_USERPWD, account.user);
        curl_easy_setopt(curl, CURLOPT_PASSWORD, account.password);
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, xml);

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
	fclose(xml);
	xml = fopen("temp/file.xml", "r");
	fseek(xml, 0L, SEEK_END);
	int filesize = ftell(xml);
	rewind(xml);
	char *xml_data = (char *)malloc(filesize);
	fread(xml_data, filesize, filesize, xml);
	fclose(xml);
	return xml_data;
}
