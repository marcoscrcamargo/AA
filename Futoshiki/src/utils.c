/* Victor Forbes - 9293394 */

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