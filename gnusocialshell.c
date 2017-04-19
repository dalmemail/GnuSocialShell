/*
 * Copyright (C) 2016, 2017 Daniel Martín
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

void print_little_group_info(struct little_group_info group)
{
	printf("\033[36m!%s\033[31m (ID %d)\n\033[32m%s\n\033[m", group.nickname, group.id, group.description);
}
