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

#include "lib/gnusocial.h"

void me_command(struct account_info info)
{
	printf("Name: %s\nUser: %s\nLocation: %s\nDescription: %s\nUrl: %s\n", info.name, info.screen_name,
	info.location, info.description, info.url);
	if (info.followers > 0) {
		printf("Followers: %d\n", info.followers);
	}
	else {
		printf("Followers: ?\n");
	}
	if (info.friends > 0) {
		printf("Friends: %d\n", info.friends);
	}
	else {
		printf("Friends: ?\n");
	}
	if (info.statuses > 0) {
		printf("Status: %d\n", info.statuses);
	}
	else {
		printf("Status: ?\n");
	}
}

void help_command()
{
	printf("/help\t\t\tPrints this help\n");
	printf("/quit\t\t\tClose GnuSocialShell\n");
	printf("/me\t\t\tPrints account information\n");
	printf("/send [MESSAGE]\t\tSend MESSAGE to GnuSocial\n");
	printf("/favorite [ID]\t\tFavorites the status ID\n");
	printf("/unfavorite [ID]\tDelete the status ID from the favorites list\n");
	printf("/search [ID]\t\tFind one status using its ID\n");
	printf("/delete [ID]\t\tDelete one status using its ID\n");
}
