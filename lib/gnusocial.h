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

#include <string.h>
#include <stdio.h>

struct account_info {
	char name[64];
	char screen_name[64];
	char location[64];
	char description[256];
	char url[128];
	int followers;
	int friends;
	int statuses;
};

struct gss_account {
	char protocol[8];
	char user[64];
	char password[64];
	char server[32];
};

struct account_info get_account_info(struct gss_account account);

size_t save_xml(void *ptr, size_t size, size_t nmemb, FILE *stream);

int parseXml(char *xml_data, int xml_data_size, char *tofind, int tofind_size, char *output, int output_size);
