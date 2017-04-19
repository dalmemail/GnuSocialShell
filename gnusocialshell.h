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

/* Using a account_info structure it prints user information (followers, friends, number of status, etc). Nothing will be returned (void) */
void print_user_info(struct account_info info);

/* The same as print_user_info(), but instead of printing user information it prints group information from a "group_info" structure */
void print_group_info(struct group_info group);

/* Prints a little information about a group (nick, id and description) from a "little_group_info" structure */
void print_little_group_info(struct little_group_info group);
