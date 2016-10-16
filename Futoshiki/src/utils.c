/* Victor Forbes - 9293394 */

#include <stdlib.h>
#include <string.h>

#include "utils.h"

void swap(void *a, void *b, int size){
	void *c;

	if (a != b){
		c = malloc(size);

		memcpy(c, a, size);
		memcpy(a, b, size);
		memcpy(b, c, size);

		free(c);
	}
}