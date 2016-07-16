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
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <termios.h>
#include "lib/gnusocial.h"

#define _FALSE 0
#define _TRUE 1

int get_size(char *path);
char *read_file(char *path);
int linecounter(char *c);
int parseConfig(char *to_search, char *output, char **lines, int n_lines);

int loadConfig(char *ConfigFilePath)
{
	extern struct gss_account main_account;
	int ret = 0;
	int result;
	char *raw_config = read_file(ConfigFilePath);
	int n_lines;
	int i;
	int x;
	if (raw_config != NULL) {
		n_lines = linecounter(raw_config);
		char **lines = (char**) malloc(n_lines * sizeof(char*));
		lines[0] = &raw_config[0];
		for (i = 1, x = 0; raw_config[x] != '\0'; x++) {
			if (raw_config[x] == '\n') {
				raw_config[x] = '\0';
				if (i < n_lines) {
					lines[i] = &raw_config[x+1];
				}
				i++;
			}
		}
		if ((result = parseConfig("protocol", main_account.protocol, lines, n_lines)) < ret) {
			printf("Error: Bad config file\n");
			ret = result;
		}
		if ((result = parseConfig("user", main_account.user, lines, n_lines)) < ret) {
			printf("Error: Bad config file\n");
			ret = result;
		}
		if ((result = parseConfig("server", main_account.server, lines, n_lines)) < ret) {
			printf("Error: Bad config file\n");
			ret = result;
		}
		if ((result = parseConfig("password", main_account.password, lines, n_lines)) < 0 && ret == 0) {
			printf("Type your password: ");
			struct termios term, term_orig;
			tcgetattr(STDIN_FILENO, &term);
			term_orig = term;
			term.c_lflag &= ~ECHO;
			tcsetattr(STDIN_FILENO, TCSANOW, &term);
			scanf("%49s",main_account.password);
			tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);
			printf("\n");
		}
	}
	else {
		ret = -1;
	}
	return ret;
}

int parseConfig(char *to_search, char *output, char **lines, int n_lines)
{
	int final_pos = 0;
	int exists = _FALSE;
	int i;
	unsigned int to_search_size = strlen(to_search);
	int ret = -1;
	for (i = 0; i < n_lines && !exists; i++) {
		unsigned int x;
		unsigned int z = 0;
		if (lines[i][0] == '#') {
			continue;
		}
		for (x = 0; lines[i][z] != '\0' && i < n_lines; z++) {
			if (to_search[x] == lines[i][z] && !exists) {
				if (++x == to_search_size) {
					exists = _TRUE;
					final_pos = z;
				}
			}
			else if (!exists) {
				x = 0;
			}
		}
		if (exists) {
			if (lines[i][final_pos+1] == '=' && lines[i][final_pos+2] != ';') {
				final_pos += 2;
				int y;
				for (y = 0; lines[i][final_pos] != ';' && lines[i][final_pos] != '\0'; final_pos++) {
					output[y++] = lines[i][final_pos];
				}
				output[y] = '\0';
				ret = 0;
			}
			else {
				ret = -1;
			}
		}
	}
	return ret;
}

char *read_file(char *path)
{
	int fd;
	int file_size;
	char *data;
	int ret;
	if ((file_size = get_size(path)) >= 0) {
		data = malloc(file_size);
		if ((fd = open(path, O_RDONLY)) < 0) {
			printf("Error al abrir el archivo '%s'\n", path);
			data = NULL;
		}
		else {
			if ((ret = read(fd, data, file_size)) < 0) {
				printf("Error leyendo '%s': %s\n", path, strerror(errno));
				data = NULL;
			}
			else if ((ret = close(fd)) < 0) {
				printf("Error leyendo '%s': %s\n", path, strerror(errno));
				data = NULL;
			}
		}
	}
	else {
		printf("Error al abrir el archivo '%s'\n", path);
		data = NULL;
	}
	return data;
}

int linecounter(char *c)
{
	int n_lines = 0;
	int i;
	for (i = 0; c[i] != '\0'; i++) {
		if (c[i] == '\n') {
			n_lines++;
		}
	}
	return n_lines;
}

int get_size(char *path)
{
	struct stat st;
	int ssize;
	if ((stat(path, &st)) < 0) {
		ssize = -1;
	}
	else {
		ssize = st.st_size;
	}
	return ssize;
}
