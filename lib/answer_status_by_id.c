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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void answer_status_by_id(struct gss_account account, int id, char *msg)
{
        int amount = 60+strlen(msg);
	char *send = malloc(amount);
	snprintf(send, amount, "in_reply_to_status_id=%d&source=GnuSocialShell&status=%s", id, msg);
	send[sizeof(send)-1] = '\0'; // snprintf does that too
	char *xml_data = send_to_api(account, send, "statuses/update.xml");
	FindXmlError(xml_data, strlen(xml_data));
	free(xml_data);
	free(send);
}
