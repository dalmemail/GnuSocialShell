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

#define USER_GROUPS 0
#define SERVER_GROUPS 1

char timelines[2][64] = {"statusnet/groups/list.xml", "statusnet/groups/list_all.xml"};

struct little_group_info *list_groups(struct gss_account account, int n_groups, int group_timeline)
{
	char count[32];
	snprintf(count, 32, "count=%d", n_groups);
	char *xml_data = send_to_api(account,count,timelines[group_timeline]);
	char error[512];
	int xml_data_size = strlen(xml_data);
	struct little_group_info *groups = (struct little_group_info*)malloc(n_groups * sizeof(struct group_info));
	int i;
	for (i = 0; i < n_groups; i++) {
		groups[i].id = 0;
	}
	if (parseXml(xml_data, xml_data_size, "<error>", 7, error, 512) > 0) {
		printf("Error: %s\n", error);
	}
	else if (xml_data_size > 0) {
		int start_status_point = 0;
		int real_status_point = 0;
		char *array_data;
		char id[16];
		array_data = &xml_data[0];
		for (i = 0; i < n_groups && (real_status_point+13) < xml_data_size; i++) {
			parseXml(array_data, (xml_data_size-real_status_point), "<id>", 4, id, 16);
			groups[i].id = atoi(id);
			parseXml(array_data, (xml_data_size-real_status_point), "<nickname>", 10, groups[i].nickname, 64);
			parseXml(array_data, (xml_data_size-real_status_point), "<description>", 13, groups[i].description, 256);
			start_status_point = parseXml(array_data, (xml_data_size-real_status_point), "</group>", 8, "", 0);
			real_status_point += start_status_point;
			array_data = &xml_data[real_status_point];
		}
	}
	else {
		printf("Error: Reading '%d' groups from '%s:%s/api/%s'\n", n_groups, account.protocol, account.server, timelines[group_timeline]);
	}
	free(xml_data);
	return groups;
}
