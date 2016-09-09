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

struct status *read_timeline(struct gss_account account, char *timeline, int n_status)
{
	struct status *status_list = (struct status*)malloc(n_status * sizeof(struct status));
	char count[32];
	snprintf(count, 32, "count=%d", n_status);
	char *xml_data = send_to_api(account,count,timeline);
	int xml_data_size = strlen(xml_data);
	char error[512];
	int i;
	for (i = 0; i < n_status; i++) {
		status_list[i].id = 0;
	}
	if (parseXml(xml_data, xml_data_size, "<error>", 7, error, 512) > 0) {
		printf("Error: %s\n", error);
	}
	else {
		int i;
		int start_status_point = 0;
		int real_status_point = 0;
		char *status_data;
		status_data = &xml_data[0];
		for (i = 0; i < n_status && (real_status_point+13) < xml_data_size; i++) {
			status_list[i] = makeStatusFromRawSource(status_data, strlen(status_data));
			start_status_point = parseXml(status_data, (xml_data_size-real_status_point), "</status>", 9, "", 0);
			real_status_point += start_status_point;
			status_data = &xml_data[real_status_point];
		}
	}
	free(xml_data);
	return status_list;
}
