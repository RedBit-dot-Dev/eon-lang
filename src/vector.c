#include "include/vector.h"
#include "include/error_codes.h"
#include <stdlib.h>

Vector* createVector(size_t item_byte_size) {

    Vector* vector = malloc(sizeof(Vector));
    if (!vector) {
        printf("Insufficient memory to allocate vector\n");
        exit(ERROR_INSUFFICIENT_MEMORY_RAM);
    }

    vector->items = NULL;
    vector->item_byte_size = item_byte_size;
    vector->capacity = 0;
    vector->used = 0;

    return vector;
}

void vectorPush(Vector* vector, void* item) {

    if (!vector->used) {
        vector->items = calloc(1, vector->item_byte_size * sizeof(void*));
        if (!vector->items) {
            printf("Insufficient memory to allocate vector items array\n");
            exit(ERROR_INSUFFICIENT_MEMORY_RAM);
        }
    }

    else if (vector->used > vector->capacity - 1) {
        vector->capacity *= 2;
        vector->items = realloc(vector->items, (sizeof(void*) * vector->item_byte_size * vector->capacity));
        if (!vector->items) {
            printf("Insufficient memory to allocate vector items array\n");
            exit(ERROR_INSUFFICIENT_MEMORY_RAM);
        }
    }

    vector->items[vector->used++] = item;

    return;
}