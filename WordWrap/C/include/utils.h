#ifndef UTILS_H
#define UTILS_H

#define and &&
#define or ||
#define not !

/* Allocates a matrix. */
void **malloc_matrix(int, int, int);

/* Frees a matrix. */
void free_matrix(void **, int);

/* Returns the minimum value between two integers. */
int min(int, int);

#endif