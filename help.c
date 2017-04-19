/*
 * Copyright (C) 2017 Daniel Martín
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *commands[] = {
        "/help",
	"/quit",
	"/me",
	"/post",
	"/favorite",
	"/unfavorite",
	"/status",
	"/delete",
	"/reply",
	"/favorites",
	"/mentions",
	"/home",
	"/personal",
	"/public",
	"/repeat",
	"/userinfo",
	"/listfollowers",
	"/listfriends",
	"/group",
	"/listgroups",
	"/groupinfo",
	"/groupjoin",
	"/groupleave",
	"/groupdirectory",
	"/subscribe",
	"/unsubscribe",
	NULL
};

void help_(char *command)
{
	if (command == NULL) {
		int i;
		printf("\t\tAll Commands\n\n");
		for (i = 0; commands[i] != NULL; i++) {
			printf("%s", commands[i]);
			int x = strlen(commands[i]);
			while (x++ != 20) printf(" ");
			if ((i+1) % 3 == 0) {
				printf("\n\n");
			}
		}
		printf("\n\nUse /help [command] without the leading slash, for help on a specific command\n\n");
	}
}
