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

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include "loadConfig.h"
#include "lib/gnusocial.h"
#include "gnusocialshell.h"
#include "lib/constants.h"
#include "help.h"

#ifdef HAVE_READLINE_H
#include <readline/readline.h>
#include <readline/history.h>
#endif

// #define VERSION "1.0.2" // -> config.h
#define MAX_PATH 256
#define MAX_CMD 4096
#define MAX_ARGV 1024
#ifdef HAVE_READLINE_H
#define MAX_PROMPT 256
#endif
#define _FALSE 0
#define _TRUE 1
#define ALL_OK 0

#define FAVORITES "favorites.xml"
#define PUBLIC_TIMELINE "statuses/public_timeline.xml"
#define HOME_TIMELINE "statuses/home_timeline.xml"
#define MENTIONS "statuses/mentions.xml"

#define FOLLOWERS "statuses/followers.xml"
#define FRIENDS "statuses/friends.xml"

#define USER_GROUPS 0
#define SERVER_GROUPS 1

int loglevel=LOG_NONE;

void version();
void help(char *prog);
void gss_shell();

struct gss_account main_account;

#ifdef HAVE_READLINE_H
char *command_names[] = {
        "/help",
	"/quit",
	"/me",
	"/post",
	"/favorite",
	"/fav",
	"/unfavorite",
	"/unfav",
	"/status",
	"/delete",
	"/del",
	"/reply",
	"/favorites",
	"/favs",
	"/mentions",
	"/home",
	"/personal",
	"/public",
	"/rt",
	"/repeat",
	"/userinfo",
	"/uinfo",
	"/followers",
	"/listfollowers",
	"/friends",
	"/listfriends",
	"/group",
	"/groups",
	"/listgroups",
	"/groupinfo",
	"/ginfo",
	"/groupjoin",
	"/gjoin",
	"/groupleave",
	"/gleave",
	"/groupdirectory",
	"/subscribe",
	"/sub",
	"/unsubscribe",
	"/unsub",
	"/user",
	"/context",
	NULL
};

// Readline API: state == 0 means it's a new word
char* gss_command_generator(const char* text, int state)
{
        static int i, len;
	char *name;
	
	if (!state) {
	        i = 0;
		len = strlen(text);
	}
  
	// Return names in turn
	while ((name = command_names[i++]) != 0) {
		if (strncmp(name, text, len) == 0)
		return (strdup(name));
	}

	return ((char *)NULL);
}

char **gss_command_completion(const char *text, int start, int end)
{
        // don't attempt any default (filesystem path) completion
        rl_attempted_completion_over = 1;
	// only attempt to complete commands when starting completion of something from begin of input line
	if (start > 0) return NULL;
	return rl_completion_matches(text, gss_command_generator);
}
#endif

int main(int argc, char **argv)
{
#ifdef HAVE_READLINE_H
        rl_attempted_completion_function = gss_command_completion;
#endif
	char *home_directory = getenv("HOME");
	char config_path[MAX_PATH];
	char config_path_suffix[] = ".config/gnusocialshell/gnusocialshell.conf";
	int l = strnlen(home_directory, MAX_PATH);
	int m = strlen(config_path_suffix);
	snprintf(config_path, l + m + 2, "%s/%s", home_directory, config_path_suffix);
	int ret = 0;
	int vflag = _FALSE;
	int hflag = _FALSE;
	int fflag = _FALSE;
	int verify_credentials = _TRUE;
	int i;
	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "--version") == 0) || (strcmp(argv[i], "-v") == 0)) {
			vflag = _TRUE;
		}
		else if ((strcmp(argv[i], "--debug") == 0) || (strcmp(argv[i], "-d") == 0)) {
			loglevel = LOG_DEBUG;
		}
		else if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
			hflag = _TRUE;
		}
		else if (((strcmp(argv[i], "--config") == 0) || (strcmp(argv[i], "-c") == 0))) {
			if ((i+1) < argc) {
				strncpy(config_path, argv[i+1], MAX_PATH);
				i++;
			}
			else {
				printf("%s: missing a file as an argument\n", argv[0]);
				fflag = _TRUE;
			}
		}
		else if ((strcmp(argv[i], "--no-verify-account") == 0) || (strcmp(argv[i], "-nva") == 0)) {
			verify_credentials = _FALSE;
		}
		else {
			printf("%s: unrecognized option '%s'\n", argv[0], argv[i]);
			printf("Try '%s --help' for more information\n", argv[0]);
			fflag = _TRUE;
		}
	}
	if (vflag) {
		version();
	}
	if (hflag) {
		help(&argv[0][0]);
	}
	if (!vflag && !hflag && !fflag) {
		if ((ret = loadConfig(config_path)) == ALL_OK) {
			if (!verify_credentials) {
				gss_shell();
			}
			else if (verify_account(main_account) != -1) {
				gss_shell();
			}
		}
	}
	return ret;
}

void version()
{
	printf("GnuSocialShell v%s\n", VERSION);
}

void help(char *prog)
{
	printf("Usage: %s [OPTION]\n", prog);
	printf("--help, -h\t\t\tPrints this help\n");
	printf("--debug, -d\t\t\tPrints extra debug messages\n");
	printf("--version, -v\t\t\tPrints GnuSocialShell version\n");
	printf("--config, -c [FILE]\t\tUse FILE as configuration file\n");
	printf("--no-verify-account, -nva\tIf you use this option GnuSocialShell won't check if your credentials are correct\n");
	printf("\nWritten by Daniel Martin (a.k.a DalmeGNU) (dalmemail _AT_ amaya.tk)\n\n");
}

static unsigned int parse(char *cmdline, char **argv, unsigned int maxArgv)
{
    unsigned int argc;
    
    for (argc = 0; argc < maxArgv && *cmdline; argc++) {
        while (*cmdline && *cmdline == ' ')
            cmdline++;

        argv[argc] = cmdline;

        while (*cmdline && *cmdline != ' ')
            cmdline++;

        if (*cmdline) *cmdline++ = '\0';
    }
    
    argv[argc] = '\0';
    return argc;
}

static int cmpCmdString(char *str1, char *str2)
{
	int result = 0;
	if ((strlen(str1) == strlen(str2)) && !strcmp(str1, str2)) result = 1;
	return result;
}

int executeCommand(char *cmdline)
{
	char fullCmdStr[MAX_CMD];
	strcpy(fullCmdStr, cmdline);
	extern struct gss_account main_account;
	int argc = 0;
	char *argv[MAX_ARGV];
	argc = parse(cmdline, argv, MAX_ARGV);
	int i = 0;
	int result = 1;
	struct account_info info;
	if (argc > 0) {
		if (cmpCmdString(argv[0], "/help")) {
			if (argc == 2) {
				help_(argv[1]);
			}
			else if (argc == 1) {
				help_(NULL);
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/me")) {
			info = get_my_account_info(main_account, &result);
			if (!result) {
				print_user_info(info);
			}
		}
		else if (cmpCmdString(argv[0], "/post")) {
			if (argc > 1) {
				send_status(main_account, &fullCmdStr[6]);
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/favorite") || cmpCmdString(argv[0], "/fav")) {
			if (argc == 2) {
				favorite(main_account, atoi(argv[1]));
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/unfavorite") || cmpCmdString(argv[0], "/unfav")) {
			if (argc == 2) {
				unfavorite(main_account, atoi(argv[1]));
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/status")) {
			if (argc == 2) {
				struct status status_ = search_by_id(main_account, atoi(argv[1]), &result);
				if (!result) {
					print_status(status_);
				}
				else printf("Error: Status with ID '%s' not found\n", argv[1]);
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/delete") || cmpCmdString(argv[0], "/del")) {
			if (argc == 2) {
				delete_status_by_id(main_account, atoi(argv[1]));
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/reply")) {
			if (argc > 2) {
				int msgpos = 7;
				while (fullCmdStr[msgpos++] != ' ');
				answer_status_by_id(main_account, atoi(argv[1]), &fullCmdStr[msgpos]);
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/favorites") || cmpCmdString(argv[0], "/favs")) {
			if (argc == 2) {
				int n_status = atoi(argv[1]);
				if (n_status > 0) {
					struct status *status_list = read_timeline(main_account, FAVORITES, n_status);
					for (i = 0; i < n_status; i++) {
						if (status_list[i].id != 0) {
							print_status(status_list[i]);
						}
					}
					free(status_list);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/mentions") || cmpCmdString(argv[0], "/replies")) {
			if (argc == 2) {
				int n_status = atoi(argv[1]);
				if (n_status > 0) {
					struct status *status_list = read_timeline(main_account, MENTIONS, n_status);
					for (i = 0; i < n_status; i++) {
						if (status_list[i].id != 0) {
							print_status(status_list[i]);
						}
					}
					free(status_list);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/home") || cmpCmdString(argv[0], "/personal")) {
			if (argc == 2) {
				int n_status = atoi(argv[1]);
				if (n_status > 0) {
					struct status *status_list = read_timeline(main_account, HOME_TIMELINE, n_status);
					for (i = 0; i < n_status; i++) {
						if (status_list[i].id != 0) {
							print_status(status_list[i]);
						}
					}
					free(status_list);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/public")) {
			if (argc == 2) {
				int n_status = atoi(argv[1]);
				if (n_status > 0) {
					struct status *status_list = read_timeline(main_account, PUBLIC_TIMELINE, n_status);
					for (i = 0; i < n_status; i++) {
						if (status_list[i].id != 0) {
							print_status(status_list[i]);
						}
					}
					free(status_list);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/user")) {
			if (argc == 3) {
				int n_status = atoi(argv[2]);
				char user_timeline[96];
				snprintf(user_timeline, 96, "statuses/friends_timeline.xml&screen_name=%s", argv[1]);
				if (n_status > 0) {
					struct status *status_list = read_timeline(main_account, user_timeline, n_status);
					for (i = 0; i < n_status; i++) {
						if (status_list[i].id != 0) {
							print_status(status_list[i]);
						}
					}
					free(status_list);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help user' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/rt") || cmpCmdString(argv[0], "/repeat")) {
			if (argc == 2) {
				retweet(main_account, atoi(argv[1]), 1);
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/userinfo") || cmpCmdString(argv[0], "/uinfo")) {
			if (argc == 2) {
				char send_screen_name[79];
				snprintf(send_screen_name, 79, "&screen_name=%s", argv[1]);
				info = get_user_info(main_account, send_screen_name);
				if (info.screen_name[0] != '\0') {
					print_user_info(info);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/followers") || cmpCmdString(argv[0], "/listfollowers")) {
			if (argc <= 2) {
					info = get_my_account_info(main_account, &result);
					int followers_to_read = info.followers;
					if (argc == 2) followers_to_read = atoi(argv[1]);
					if (!result) {
						print_users_array_info(main_account, FOLLOWERS, followers_to_read);
					}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/friends") || cmpCmdString(argv[0], "/listfriends")) {
			if (argc <= 2) {
				info = get_my_account_info(main_account, &result);
				int friends_to_read = info.friends;
				if (argc == 2) friends_to_read = atoi(argv[1]);
				if (!result) {
					print_users_array_info(main_account, FRIENDS, friends_to_read);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/group")) {
			if (argc == 3) {
				int n_status = atoi(argv[2]);
				char g_timeline[256];
				snprintf(g_timeline, 256, "statusnet/groups/timeline/%s.xml", argv[1]);
				if (n_status > 0) {
					struct status *status_list = read_timeline(main_account, g_timeline, n_status);
					for (i = 0; i < n_status; i++) {
						if (status_list[i].id != 0) {
							print_status(status_list[i]);
						}
					}
					free(status_list);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/groups") || cmpCmdString(argv[0], "/listgroups")) {
			if (argc <= 2) {
				int n_groups = get_number_of_groups(main_account);
				if (argc == 2) n_groups = atoi(argv[1]);
				if (n_groups > 0) {
					struct little_group_info *groups = list_groups(main_account, n_groups, USER_GROUPS);
					for (i = 0; i < n_groups; i++) {
						if (groups[i].id != 0) {
							print_little_group_info(groups[i]);
						}
					}
					free(groups);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/groupinfo") || cmpCmdString(argv[0], "/ginfo")) {
			if (argc == 2) {
				struct group_info group = get_group_info(main_account, atoi(argv[1]));
				if (group.id > 0) {
					print_group_info(group);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/groupjoin") || cmpCmdString(argv[0], "/gjoin")) {
			if (argc == 2) join_group(main_account, atoi(argv[1]));
			else printf("Error: Invalid usage, see '/help' for details\n");
		}
		else if (cmpCmdString(argv[0], "/groupleave") || cmpCmdString(argv[0], "/gleave")) {
			if (argc == 2) leave_group(main_account, atoi(argv[1]));
			else printf("Error: Invalid usage, see '/help' for details\n");
		}
		else if (cmpCmdString(argv[0], "/groupdirectory")) {
			if (argc == 2) {
				int n_groups = atoi(argv[1]);
				if (n_groups > 0) {
					struct little_group_info *groups = list_groups(main_account, n_groups, SERVER_GROUPS);
					for (i = 0; i < n_groups; i++) {
						if (groups[i].id != 0) {
							print_little_group_info(groups[i]);
						}
					}
					free(groups);
				}
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/subscribe") || cmpCmdString(argv[0], "/sub")) {
			if (argc == 2) {
				follow_user(main_account, argv[1]);
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/unsubscribe") || cmpCmdString(argv[0], "/unsub")) {
			if (argc == 2) {
				unfollow_user(main_account, argv[1]);
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmpCmdString(argv[0], "/context")) {
			if (argc == 2) {
				int n_status = 0;
				int sid = atoi(argv[1]);
				struct status status_[5];
				do {
					status_[n_status] = search_by_id(main_account, sid, &result);
					sid = status_[n_status].in_reply_to_id;
					n_status++;
				} while(n_status < 5 && status_[n_status-1].in_reply_to_id && !result);
				if (n_status > 1 || !result) {
					for (i = (n_status-1); i >= 0; i--) {
						print_status(status_[i]);
					}
				}
				else printf("Error: Status with ID '%s' not found\n", argv[1]);
			}
			else {
				printf("Error: Invalid usage, see '/help context' for details\n");
			}
		}
		else if (!cmpCmdString(argv[0], "/quit")) {
			printf("Command '%s' not found\n", argv[0]);
		}
	}
	return strcmp(cmdline, "/quit");
}

void gss_shell()
{
	extern struct gss_account main_account;
#ifdef HAVE_READLINE_H
	char *input;
	char prompt[MAX_PROMPT];
	snprintf(prompt, MAX_PROMPT,
		 "@%s@%s-> ", main_account.user, main_account.server); 
#else
	char cmdline[MAX_CMD];
#endif
	printf("Type '/help' to get a list of commands\n\n");
	do {
#ifdef HAVE_READLINE_H
	        input = readline(prompt);
		if (!input) {
		  // Quit on failure (which means EOF)
		  printf("\n");
		  break;
		}

		rl_bind_key('\t',rl_complete);

		add_history(input);
#else
		printf("@%s@%s-> ", main_account.user, main_account.server);
		if( fgets(cmdline, MAX_CMD, stdin) == NULL) {
		  // Quit on failure (which means EOF)
		  printf("\n");
		  break;
		}

		cmdline[MAX_CMD-1] = '\0';
		/* Delete the newline character: */
		cmdline[strlen(cmdline)-1] = '\0';
#endif
	} while(
#ifdef HAVE_READLINE_H
		executeCommand(input) != 0
#else
		executeCommand(cmdline) != 0
#endif
		);
#ifdef HAVE_READLINE_H
	free(input);
#endif
}
