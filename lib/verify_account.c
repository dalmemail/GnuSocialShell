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

#include <ctype.h>
#include "constants.h"

extern int loglevel;

int verify_account(struct gss_account account)
{
	int ret = 0;
	char *xml_data = send_to_api(account, NULL, "account/verify_credentials.xml");
	int xml_data_size = strlen(xml_data);
	char error[512];
	if (parseXml(xml_data, xml_data_size, "<error>", 7, error, 512) > 0) {
		printf("Error: %s\n", error);
		ret = -1;
	}
	else if (parseXml(xml_data, xml_data_size, "<screen_name>", 13, "", 0) < 0) {
		printf("Error: Conecting to @%s@%s\n", account.user, account.server);
		if (loglevel>=LOG_DEBUG) {
		    for (int i = 0; i < xml_data_size; ++i) {
		      if (xml_data[i] == '\0') break;
		      /* fprintf(stderr, "%02x ", (unsigned char)xml_data[i]); */
		      if (xml_data[i] == '\\') {
		      	fprintf(stderr, "\\\\");
		      } else if (isprint(xml_data[i])) {
		      	fprintf(stderr, "%c", (unsigned char)xml_data[i]);
		      } else {
		      	fprintf(stderr, "\\x%2x", (unsigned char)xml_data[i]);
		      }
		    }
		    fprintf(stderr, "\n");
		}
		ret = -1;
	}
	free(xml_data);
	return ret;
}
