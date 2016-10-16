/* Victor Forbes - 9293394 */

#ifndef UTILS_H
#define UTILS_H

#define true 1
#define false 0

#define and &&
#define or ||

typedef unsigned char bool;

/* Troca o valor de duas posições da memória diferentes. */
void swap(void *, void *, int);

/* Retorna o máximo entre dois inteiros. */
int max(int, int);

#endif