#ifndef EON_VECTOR_H
#define EON_VECTOR_H
#include <stdio.h>

typedef struct _vector {
    void** items;
    size_t used;
    size_t item_byte_size;
    size_t capacity;
} Vector;

Vector* createVector(size_t capacity);

void vectorPush(Vector* vector, void* item);

#endif