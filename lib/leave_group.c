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

void leave_group(struct gss_account account, int id)
{
	char send[16];
	snprintf(send, 16, "id=%d", id);
	char *xml_data = send_to_api(account, send, "statusnet/groups/leave.xml");
	char error[512];
	int xml_data_size = strlen(xml_data);
	if (parseXml(xml_data, xml_data_size, "<error>", 7, error, 512) > 0) {
		printf("Error: %s\n", error);
	}
	free(xml_data);
}
