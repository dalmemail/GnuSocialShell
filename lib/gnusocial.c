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

#include "gnusocial.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t save_xml(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

int parseXml(char *xml_data, int xml_data_size, char *tofind, int tofind_size, char *output, int output_size)
{
	int pos = 0;
	int act_pos = 0;
	int start_pos = 0;
	int ret = -1;
	int i;
	for (i = 0; i < output_size; i++) {
		output[i] = '\0';
	}
	for (i = pos; act_pos != tofind_size && i < xml_data_size; i++) {
		if (tofind[act_pos] == xml_data[i]) {
			act_pos++;
			if (act_pos == tofind_size) {
				pos = i;
				ret = 0;
			}
		}
		else {
			act_pos = 0;
		}
	}
	start_pos = pos+1;
	if (ret == 0) {
		for (i = 0; xml_data[start_pos+i] != '<' ||xml_data[start_pos+i+1] != '/'; i++) {
			output[i] = xml_data[start_pos+i];
		}
	}
	return ret;
}
