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

struct group_info get_group_info(struct gss_account account, int id)
{
	char send[16];
	snprintf(send, 16, "id=%d", id);
	char *xml_data = send_to_api(account, send, "statusnet/groups/show.xml");
	char error[512];
	char output[512];
	int xml_data_size = strlen(xml_data);
	struct group_info group;
	if (parseXml(xml_data, xml_data_size, "<error>", 7, error, 512) > 0) {
		printf("Error: %s\n", error);
		group.id = -1;
	}
	else {
		group.id = id;
		if (parseXml(xml_data, xml_data_size, "<url>", 5, output, 512) > 0) {
			strncpy(group.url, output, MAX_URL);
		}
		else {
			group.url[0] = '?';
			group.url[1] = '\0';
		}
		if (parseXml(xml_data, xml_data_size, "<nickname>", 10, output, 512) > 0) {
			strncpy(group.nickname, output, MAX_GROUP_NICKNAME);
		}
		else {
			group.nickname[0] = '?';
			group.nickname[1] = '\0';
		}
		if (parseXml(xml_data, xml_data_size, "<fullname>", 10, output, 512) > 0) {
			strncpy(group.fullname, output, MAX_GROUP_FULLNAME);
		}
		else {
			group.fullname[0] = '?';
			group.fullname[1] = '\0';
		}
		if (parseXml(xml_data, xml_data_size, "<member>", 8, output, 512) > 0) {
			if (strcmp(output, "true") == 0) {
				group.member = 1;
			}
			else {
				group.member = 0;
			}
		}
		else {
			group.member = -1;
		}
		if (parseXml(xml_data, xml_data_size, "<admin_count>", 13, output, 512) > 0) {
			group.admins = atoi(output);
		}
		else {
			group.admins = -1;
		}
		if (parseXml(xml_data, xml_data_size, "<member_count>", 14, output, 512) > 0) {
			group.members = atoi(output);
		}
		else {
			group.members = -1;
		}
		if (parseXml(xml_data, xml_data_size, "<description>", 13, output, 512) > 0) {
			strncpy(group.description, output, MAX_DESCRIPTION);
		}
		else {
			group.description[0] = '?';
			group.description[1] = '\0';
		}
	}
	free(xml_data);
	return group;
}
