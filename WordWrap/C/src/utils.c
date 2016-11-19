#include <stdlib.h>

#include "utils.h"

void **malloc_matrix(int n, int m, int size){
	void **matrix = (void **)malloc(n * sizeof(void *));
	int i;

	for (i = 0; i < n; i++){
		matrix[i] = malloc(m * size);
	}

	return matrix;
}

void free_matrix(void **matrix, int n){
	int i;

	for (i = 0; i < n; i++){
		free(matrix[i]);
	}

	free(matrix);
}

int min(int a, int b){
	return (a < b ? a : b);
}