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

#include "constants.h"

struct account_info {
	char name[MAX_ACCOUNT_NAME];
	char screen_name[MAX_SCREEN_NAME];
	char location[MAX_LOCATION];
	char description[MAX_DESCRIPTION];
	char url[MAX_URL];
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

struct status {
	char text[1024];
	int id;
	char author_screen_name[64];
	int in_reply_to_id;
	char in_reply_to_user[64];
	char date[64];
};

struct group_info {
	int id;
	char url[MAX_URL];
	char nickname[MAX_GROUP_NICKNAME];
	char fullname[MAX_GROUP_FULLNAME];
	/* ZERO if not, Non-ZERO if true */
	int member;
	int admins;
	int members;
	char description[MAX_DESCRIPTION];
};

struct little_group_info {
	int id;
	char nickname[MAX_GROUP_NICKNAME];
	char description[MAX_DESCRIPTION];
};

/**
 * @brief Downloads the user information from the GnuSocial server
 * @param account A gss_account structure to send the request to the GnuSocial server
 * @param result An intenger to store if there is an error or not. ZERO if not, Non-ZERO if yes
 * @return The user information as an account_info structure
 */

struct account_info get_my_account_info(struct gss_account account, int *result);

/**
 * @brief Post a new notice as the authenticating user
 * @param account A gss_account structure to authenticate the user into the server
 * @param msg String that contains the notice content to post
 */

void send_status(struct gss_account account, char *msg);

/**
 * @brief Favorites the status specified in the ID parameter as the authenticating user
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The notice ID to favorite
 */

void favorite(struct gss_account account, int id);

/**
 * @brief Un-favorites the status specified in the ID parameter as the authenticating user
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The notice ID to un-favorite
 */

void unfavorite(struct gss_account account, int id);

/**
 * @brief Prints a single notice, specified by the id parameter
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The notice ID to print
 * @param result An intenger to store Non-ZERO if there is an error, ZERO if not
 * @return An status structure with the status information
 */

struct status search_by_id(struct gss_account account, int id, int *result);

/**
 * @brief Destroys the notice specified by the required ID parameter
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The notice ID to destroy
 */

void delete_status_by_id(struct gss_account account, int id);

/**
 * @brief Post a new notice in reply specified by the required ID parameter
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The notice ID to reply
 */

void answer_status_by_id(struct gss_account account, int id, char *msg);

/**
 * @brief Read a number of status from 'timeline'
 * @param account A gss_account structure to authenticate the user into the server
 * @param timeline The timeline path to read
 * @param n_status The number of status to read from timeline
 * @return An array of status
 */

struct status *read_timeline(struct gss_account account, char *timeline, int n_status);

/**
 * @brief Repeats a notice espeficied by its ID
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The notice ID to repeat
 * @param code Should be 1 to work correctly
 */

void retweet(struct gss_account account, int id, int code);

/**
 * @brief Gets user information especified by "source"
 * @param account A gss_account structure to authenticate the user into the server
 * @param source The user to get the information. Can be used the user ID or user SCREEN_NAME
 * Example: source="&id=1" source="&screen_name=4dmin"
 * @return A account_info structure with all the info
 */

struct account_info get_user_info(struct gss_account account, char *source);

/**
 * @brief Checks if user credentials are correct
 * @param A gss_account structure to check
 */

int verify_account(struct gss_account account);

/**
 * @brief Gets group information espeficied by its ID
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The group ID to get the info
 * @return A group_info structure with all the info
 */

struct group_info get_group_info(struct gss_account account, int id);

/**
 * @brief Join a group espeficied by its ID
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The group ID to join
 */

void join_group(struct gss_account account, int id);

/**
 * @brief Leave a group especified by its ID
 * @param account A gss_account structure to authenticate the user into the server
 * @param id The group ID to leave
 */

void leave_group(struct gss_account account, int id);

/**
 * @brief Gets little information of a number of groups especified by a 'n_groups'
 * @param account A gss_account structure to authenticate the user into the server
 * @param n_groups The number of groups to be readed
 * @param group_timeline Can be USER_GROUPS (ZERO) or SERVER_GROUPS (1)
 * @return An array of little_group_info structures with all the info
 */

struct little_group_info *list_groups(struct gss_account account, int n_groups, int group_timeline);

/**
 * @brief Start to follow a user especified by its screen_name
 * @param account A gss_account structure to authenticate the user into the server
 * @param screen_name The user screen_name to follow
 */

void follow_user(struct gss_account account, char *screen_name);

/**
 * @brief Stop to follow a user espeficied by its screen_name
 * @param account A gss_account structure to authenticate the user into the server
 * @param screen_name The user to unfollow
 */

void unfollow_user(struct gss_account account, char *screen_name);

/**
 * @brief Prints a list of users from a especified list
 * @param source The source list to read the users
 * @param n_users The number of users to read
 */

void print_users_array_info(struct gss_account account, char *source, int n_users);

/**
 * @brief Make a status structure using raw xml data
 * @param raw_data The xml file
 * @param data_size The xml file size
 * @return A status structure with the status
 */

struct status makeStatusFromRawSource(char *raw_data, int data_size);

/**
 * @brief Prints an status
 * @param status_ The status to print
 */

void print_status(struct status status_);

/**
 * @brief Find a tag into a xml raw file and copy content
 * @param xml_data The raw xml file content
 * @param xml_data_size The xml_data size or size to search in
 * @param tofind The tag to find in xml_data
 * @param tofind_size The size of tofind
 * @param output A variable to save the tag content
 * @param output_size The size of output
 * @return -1 if not exits or the position on the file
 */

int parseXml(char *xml_data, int xml_data_size, char *tofind, int tofind_size, char *output, int output_size);

/**
 * @brief Sends a formatted content to the server API
 * @param account A gss_structure to authenticate into the server
 * @param send The content to send to the server
 * @param xml_doc Where to send the content using 'account'
 */

char *send_to_api(struct gss_account account, char *send, char *xml_doc);

/**
 * @brief Searchs for error into a especified string using parseXml()
 * @param xml_data The string with the xml file content returned by the server
 * @param xml_data_size The size of the string 'xml_data'
 * @return A positive number if there is an error, -1 if not
 */

int FindXmlError(char *xml_data, int xml_data_size);

/**
 * @brief Returns the number of groups in which "account" is
 * @param account A gss_account structure to authenticate the user into the server
 * @return The number of groups in which "account" is, or ZERO if fail
 */

int get_number_of_groups(struct gss_account account);
