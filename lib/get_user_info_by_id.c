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

struct account_info get_user_info(struct gss_account account, char *source)
{
	char *xml_data = send_to_api(account, source, "users/show.xml");
	char error[512];
	char output[512];
	int xml_data_size = strlen(xml_data);
	struct account_info info;
	if (parseXml(xml_data, xml_data_size, "<error>", 7, error, 512) > 0) {
		printf("Error: %s\n", error);
		info.screen_name[0] = '\0';
	}
	else {
		if (parseXml(xml_data, xml_data_size, "<name>", 6, output, 512) > 0) {
			strncpy(info.name, output, MAX_ACCOUNT_NAME);
		}
		else {
			info.name[0] = '?';
			info.name[1] = '\0';
		}
		if (parseXml(xml_data, xml_data_size, "<screen_name>", 13, output, 512) > 0) {
			strncpy(info.screen_name, output, MAX_SCREEN_NAME);
		}
		else {
			info.screen_name[0] = '?';
			info.screen_name[1] = '\0';
		}
		if (parseXml(xml_data, xml_data_size, "<location>", 10, output, 512) > 0) {
			strncpy(info.location, output, MAX_LOCATION);
		}
		else {
			info.location[0] = '?';
			info.location[1] = '\0';
		}
		if (parseXml(xml_data, xml_data_size, "<description>", 13, output, 512) > 0) {
			strncpy(info.description, output, MAX_DESCRIPTION);
		}
		else {
			info.description[0] = '?';
			info.description[1] = '\0';
		}
		if (parseXml(xml_data, xml_data_size, "<url>", 5, output, 512) > 0) {
			strncpy(info.url, output, MAX_URL);
		}
		else {
			info.url[0] = '?';
			info.url[1] = '\0';
		}
		if (parseXml(xml_data, xml_data_size, "<followers_count>", 17, output, 512) > 0) {
			info.followers = atoi(output);
		}
		else {
			info.followers = -1;
		}
		if (parseXml(xml_data, xml_data_size, "<friends_count>", 15, output, 512) > 0) {
			info.friends = atoi(output);
		}
		else {
			info.friends = -1;
		}
		if (parseXml(xml_data, xml_data_size, "<statuses_count>", 16, output, 512) > 0) {
			info.statuses = atoi(output);
		}
		else {
			info.statuses = -1;
		}
	}
	free(xml_data);
	return info;
}
