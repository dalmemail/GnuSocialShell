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

void print_user_info(struct account_info info)
{
	printf("\033[31mName: %s\n\033[32mUser: %s\n\033[33mLocation: %s\n\033[34mDescription: %s\n\033[35mUrl: %s\n",
	info.name, info.screen_name, info.location, info.description, info.url);
	if (info.followers > 0) {
		printf("\033[36mFollowers: %d\n\033[m", info.followers);
	}
	else {
		printf("\033[36mFollowers: ?\n\033[m");
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

void print_group_info(struct group_info group)
{
	printf("\033[31mNickname: %s\n\033[32mFullname: %s\n\033[33mDescription: %s\n\033[34mUrl: %s\n", group.nickname,
	group.fullname, group.description, group.url);
	if (group.member) {
		printf("\033[35mYou are member of this group\n");
	}
	else {
		printf("\033[35mYou aren't member of this group\n");
	}
	if (group.admins >= 0) {
		printf("\033[36mAdmins: %d\n", group.admins);
	}
	else {
		printf("\033[36Admins: ?\n");
	}
	if (group.members >= 0) {
		printf("\033[mMembers: %d\n", group.members);
	}
	else {
		printf("\033[mMembers: ?\n");
	}
}

void help_command(char *args)
{
	if (args == NULL) {
		printf("/help\t\t\t\tPrints this help\n");
		printf("/quit\t\t\t\tClose GnuSocialShell\n");
		printf("/me\t\t\t\tPrints account information\n");
		printf("/send [MESSAGE]\t\t\tSend MESSAGE to GnuSocial\n");
		printf("/favorite [ID]\t\t\tFavorites the status ID\n");
		printf("/unfavorite [ID]\t\tDelete the status ID from the favorites list\n");
		printf("/search [ID]\t\t\tFind one status using its ID\n");
		printf("/delete [ID]\t\t\tDelete one status using its ID\n");
		printf("/reply [ID] [MESSAGE]\t\tSend MESSAGE as a reply of status ID\n");
		printf("/favorites [NUMBER]\t\tDownloads 'NUMBER' favorites from favorites list\n");
		printf("/mentions [NUMBER]\t\tDownloads 'NUMBER' mentions\n");
		printf("/home_timeline [NUMBER]\t\tDownloads 'NUMBER' status from Home Timeline\n");
		printf("/ht [NUMBER]\t\t\tThe same that /home_timeline\n");
		printf("/public_timeline [NUMBER]\tDownloads 'NUMBER' status from Public Timeline\n");
		printf("/pt [NUMBER]\t\t\tThe same that /public_timeline\n");
		printf("/rt [ID]\t\t\tRepeat the notice 'ID'\n");
		printf("/user_info [ID]\t\t\tPrints user detailed information using its ID\n");
		printf("/ui [SCREEN_NAME]\t\tPrints user detailes information using its screen_name\n");
		printf("/followers_list\t\t\tPrints the people who is following you\n");
		printf("/friends_list\t\t\tPrints the people you are following\n");
		printf("/group\t\t\t\tThe same that '/help group'\n");
	}
	else if (strcmp(args, "group") == 0) {
		printf("/group show [ID]\t\tPrints group detailed information using its ID\n");
		printf("/group join [ID]\t\tSubscrite to the given group\n");
		printf("/group leave [ID]\t\tLeave the given group\n");
		printf("/group list [NUMBER]\t\tPrints detailed information of 'NUMBER' groups\n");
	}
	else {
		printf("No such help for '%s'\n", args);
	}
}
