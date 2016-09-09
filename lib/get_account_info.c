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

#include "gnusocial.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct account_info datatoaccount(char *xml_data, int xml_data_size)
{
	struct account_info info;
	char output[512];
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
		info.name[0] = '?';
		info.name[1] = '\0';
	}
	if (parseXml(xml_data, xml_data_size, "<location>", 10, output, 512) > 0) {
		strncpy(info.location, output, MAX_LOCATION);
	}
	else {
		info.name[0] = '?';
		info.name[1] = '\0';
	}
	if (parseXml(xml_data, xml_data_size, "<description>", 13, output, 512) > 0) {
		strncpy(info.description, output, MAX_DESCRIPTION);
	}
	else {
		info.name[0] = '?';
		info.name[1] = '\0';
	}
	if (parseXml(xml_data, xml_data_size, "<url>", 5, output, 512) > 0) {
		strncpy(info.url, output, MAX_URL);
	}
	else {
		info.name[0] = '?';
		info.name[1] = '\0';
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
	return info;
}

struct account_info get_my_account_info(struct gss_account account, int *result)
{
	char send[79];
	snprintf(send, 79, "screen_name=%s", account.user);
	char *xml_data = send_to_api(account, send, "users/show.xml");
	int xml_data_size = strlen(xml_data);
	struct account_info info;
	if (FindXmlError(xml_data, xml_data_size) < 0) {
		info = datatoaccount(xml_data, xml_data_size);
		*result = 0;
	}
	free(xml_data);
	return info;
}
