/*Copyright © 2016 - Marcos Cesar Ribeiro de Camargo (9278045), Victor Luiz Roquete Forbes (9293394)

This file is part of Futoshiki.

Futoshiki is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Futoshiki is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Futoshiki.  If not, see <http://www.gnu.org/licenses/>.*/

#include <stdlib.h>
#include <string.h>

#include "utils.h"

void swap(void *a, void *b, int size){
	void *c;

	// Se forem regiões da memória diferentes.
	if (a != b){
		c = malloc(size);

		memcpy(c, a, size);
		memcpy(a, b, size);
		memcpy(b, c, size);

		free(c);
	}
}

int max(int a, int b){
	return a > b ? a : b;
}