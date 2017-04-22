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

static char *commands[] = {
        "help",
	"quit",
	"me",
	"post",
	"favorite",
	"unfavorite",
	"status",
	"delete",
	"reply",
	"favorites",
	"mentions",
	"home",
	"public",
	"repeat",
	"userinfo",
	"listfollowers",
	"listfriends",
	"group",
	"listgroups",
	"groupinfo",
	"groupjoin",
	"groupleave",
	"groupdirectory",
	"subscribe",
	"unsubscribe",
	"user",
	"context",
	NULL
};

static char *cmdInfo[] = {
	"/help [COMMAND NAME]\tThis will give some information about a command specified by the user. Else, it will print a list of available commands",
	"/quit\tThis will exit GnuSocialShell",
	"/me\tThis will print some information about the current user given by your server's API",
	"/post [MESSAGE]\tThis will post a new status to the user's timeline",
	"/favorite [ID]\tThis will mark one status as favorite using its ID\n\t\tAliases: /fav",
	"/unfavorite [ID]\tThis will unmark one status from your favorite list using its ID\n\t\t\tAliases: /unfav",
	"/status [ID]\tThis will print a status using its ID to find it",
	"/delete [ID]\tThis will delete the status with the ID specified\n\t\tAliases: /del",
	"/reply [ID] [MESSAGE]\tThis will create a reply to the status with the ID specified",
	"/favorites [NUMBER]\tThis will show you a number of states that you have marked as favorite\n\t\t\tAliases: /favs",
	"/mentions [NUMBER]\tThis will show you a number of states that mention you\n\t\t\tAliases: /replies",
	"/home [NUMBER]\tThis will show you a number of states from your home timeline\n\t\tAliases: /personal",
	"/public [NUMBER]\tThis will print a number of states from the public timeline",
	"/repeat [ID]\tThis will repeat the status with the ID specified\n\t\tAliases: /rt",
	"/userinfo [NICKNAME]\tThis will print some information about the user with the nickname specified (without the '@')\n\t\t\tAliases: /uinfo",
	"/listfollowers [NUMBER]\tThis will show you a number of followers specified. Else, it will print all your followers list.\n\t\t\tAliases: /followers",
	"/listfriends [NUMBER]\tThis will show you a number of friends specified. Else, it will print all your friends list.\n\t\t\tAliases: /friends",
	"/group [ID] [NUMBER]\tThis will show you a number of states from a group timeline with the ID specified",
	"/listgroups [NUMBER]\tThis will show you a number of groups specified in which you are. Else, it will show you all the groups in which you are.\n\t\t\tAliases: /groups",
	"/groupinfo [ID]\tThis will print some information about the group with the ID specified\n\t\tAliases: /ginfo",
	"/groupjoin [ID]\tThis will add you as a member of the group specified\n\t\tAliases: /gjoin",
	"/groupleave [ID]\tThis will remove you from membership of the group specified\n\t\t\tAliases: /gleave",
	"/groupdirectory [NUMBER]\tThis will print you a specified number of public groups from your server",
	"/subscribe [NICKNAME]\tThis will subscribe you to the user with the nickname specified (without the '@')\n\t\t\tAliases: /sub",
	"/unsubscribe [NICKNAME]\tThis will unsubscribe you to the user with the nickname specified (without the '@')\n\t\t\tAliases: /unsub",
	"/user [NICKNAME] [NUMBER]\tThis will print a number of states specified from a specific user's timeline (without the '@')",
	"/context [ID]\tThis will show the context of the state with the ID specified",
	NULL
};

void help_(char *command)
{
	int i;
	if (command == NULL) {
		printf("\t\tAll Commands\n\n");
		for (i = 0; commands[i] != NULL; i++) {
			printf("/%s", commands[i]);
			int x = strlen(commands[i]);
			while (x++ != 20) printf(" ");
			if ((i+1) % 3 == 0) {
				printf("\n\n");
			}
		}
		printf("\n\nUse /help [command] without the leading slash, for help on a specific command\n\n");
	}
	else {
		int help_pos = -1;
		for(i = 0; commands[i] != NULL && help_pos < 0; i++) {
			if (strcmp(command, commands[i]) == 0) {
				help_pos = i;
				printf("%s\n", cmdInfo[i]);
			}
		}
		if (help_pos < 0) printf("Error: No such command\n");
	}
}
