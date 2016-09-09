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

void print_users_array_info(struct gss_account account, char *source, int n_users)
{
	char count[32];
	snprintf(count, 32, "count=%d", n_users);
	char *xml_data = send_to_api(account,count,source);
	int xml_data_size = strlen(xml_data);
	char error[512];
	if (parseXml(xml_data, xml_data_size, "<error>", 7, error, 512) > 0) {
		printf("Error: %s\n", error);
	}
	else if (xml_data_size > 0) {
		char name[64];
		char screen_name[64];
		int start_status_point = 0;
		int real_status_point = 0;
		char *array_data;
		char id[16];
		array_data = &xml_data[0];
		int i;
		for (i = 0; i < n_users && (real_status_point+13) < xml_data_size; i++) {
			parseXml(array_data, (xml_data_size-real_status_point), "<id>", 4, id, 16);
			parseXml(array_data, (xml_data_size-real_status_point), "<name>", 6, name, 64);
			parseXml(array_data, (xml_data_size-real_status_point), "<screen_name>", 13, screen_name, 64);
			start_status_point = parseXml(array_data, (xml_data_size-real_status_point), "</user>", 7, "", 0);
			printf("\033[36m%s\033[31m @%s\033[32m (ID %s)\n\033[m", name, screen_name, id);
			real_status_point += start_status_point;
			array_data = &xml_data[real_status_point];
		}
	}
	else {
		printf("Error: Reading %d users from '%s://%s/api/%s'\n", n_users, account.protocol, account.server, source);
	}
	free(xml_data);
}
