#include "include/vector.h"
#include <assert.h>

Vector* createVector(size_t item_byte_size) {

    Vector* vector = malloc(sizeof(Vector));
    assert(vector && "Could not allocate memory for the vector");

    vector->items = NULL;
    vector->item_byte_size = item_byte_size;
    vector->capacity = 0;
    vector->used = 0;

    return vector;
}

void vectorPush(Vector* vector, void* item) {

    if (!vector->used) {
        vector->items = calloc(1, vector->item_byte_size);
        assert(vector->items && "Could not allocate memory for vector initialization");
    }

    else if (vector->used > vector->capacity - 1) {
        vector->capacity *= 2;
        vector->items = realloc(vector->items, (vector->item_byte_size * vector->capacity));
        assert(vector->items && "Could not reallocate memory for vector push");
    }

    vector->items[vector->used++] = item;

    return;
}