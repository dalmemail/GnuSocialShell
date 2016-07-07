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
#include <string.h>
#include "gnusocialshell.h"
#include "loadConfig.h"

#define VERSION "0.1"
#define MAX_PATH 128
#define _FALSE 0
#define _TRUE 1
#define ALL_OK 0

void version();
void help();

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
			printf("%s\n%s\n%s\n%s\n", main_account.protocol, main_account.user, main_account.password, main_account.server);
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
