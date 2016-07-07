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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "loadConfig.h"
#include "lib/gnusocial.h"
#include "gnusocialshell.h"

#define VERSION "0.5"
#define MAX_PATH 128
#define _FALSE 0
#define _TRUE 1
#define ALL_OK 0

void version();
void help();
void gss_shell();

struct gss_account main_account;

int main(int argc, char **argv)
{
	char config_path[MAX_PATH] = "./config\0";
	int ret = 0;
	int vflag = _FALSE;
	int hflag = _FALSE;
	int i;
	for (i = 1; i < argc; i++) {
		if ((strcmp(argv[i], "--version") == 0) || (strcmp(argv[i], "-v") == 0)) {
			vflag = _TRUE;
		}
		else if ((strcmp(argv[i], "--help") == 0) || (strcmp(argv[i], "-h") == 0)) {
			hflag = _TRUE;
		}
		else if (((strcmp(argv[i], "--config") == 0) || (strcmp(argv[i], "-c") == 0)) && (i+1) < argc) {
			strcpy(config_path, argv[i+1]);
		}
	}
	if (vflag) {
		version();
	}
	if (hflag) {
		help();
	}
	if (!vflag && !hflag) {
		if ((ret = loadConfig(config_path)) == ALL_OK) {
			printf("Type '/help' to get a list of commands\n\n");
			gss_shell();
		}
	}
	return ret;
}

void version()
{
	printf("GnuSocialShell v%s\n", VERSION);
}

void help()
{
	printf("Uso: ./gss [OPCION(es)]\n");
	printf("--help, -h\t\tMuestra esta ayuda\n");
	printf("--version, -v\t\tMuestra la version de GSS\n");
	printf("--config, -c [ARCHIVO]\tFija la ruta del archivo de configuracion\n");
	printf("\nEscrito por DalmeGNU (dalmemail _AT_ amaya.tk)\n\n");
}

/* compara una cadena de caracteres desde el caracter 0 hasta el caracter 'max'*/
int cmdcmp(char *a, char *b, int max)
{
	int ret = 0;
	int i;
	for (i = 0; i < max; i++) {
		if (a[i] != b[i]) {
			ret++;
		}
	}
	return ret;
}

void gss_shell()
{
	extern struct gss_account main_account;
	char cmdline[256];
	char *args;
	struct account_info info;
	do {
		printf("@%s@%s-> ", main_account.user, main_account.server);
		fgets(cmdline, 256, stdin);
		cmdline[strlen(cmdline)-1] = '\0';
		if (strcmp(cmdline, "/help") == 0) {
			help_command();
		}
		else if (strcmp(cmdline, "/me") == 0) {
			info = get_account_info(main_account);
			me_command(info);
		}
		else if (cmdcmp(cmdline, "/send", 5) == 0) {
			if (strlen(cmdline) >= 7) {
				args = &cmdline[6];
				send_status(main_account, args);
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmdcmp(cmdline, "/favorite", 9) == 0) {
			if (strlen(cmdline) >= 11) {
				args = &cmdline[10];
				favorite(main_account, atoi(args));
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmdcmp(cmdline, "/unfavorite", 11) == 0) {
			if (strlen(cmdline) >= 13) {
				args = &cmdline[12];
				unfavorite(main_account, atoi(args));
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmdcmp(cmdline, "/search", 7) == 0) {
			if (strlen(cmdline) >= 9) {
				args = &cmdline[8];
				search_by_id(main_account, atoi(args));
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (cmdcmp(cmdline, "/delete", 7) == 0) {
			if (strlen(cmdline) >= 9) {
				args = &cmdline[8];
				delete_status_by_id(main_account, atoi(args));
			}
			else {
				printf("Error: Invalid usage, see '/help' for details\n");
			}
		}
		else if (strcmp(cmdline, "/quit") != 0 && cmdline[0] == '/') {
			printf("Command '%s' not found\n", cmdline);
		}
	} while(strcmp(cmdline, "/quit") != 0);
}
