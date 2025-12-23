#ifndef EON_ARENA_H
#define EON_ARENA_H
#include <stdint.h>
#include <stdbool.h>
#include "macros.h"

typedef struct _arena {
    void* memory;
    void* pos;
    uint64_t capacity;
    uint64_t int_pos;
} Arena;

Arena* createArena(uint64_t capacity);
void* arenaPush(Arena* arena, int size, bool zero_out);
char* arenaPushString(Arena* arena, int size, char* string);
void arenaFree(Arena* arena);

/* typedef struct _arena {
    uint64_t capacity;
    uint64_t pos;
} Arena;

Arena* createArena(uint64_t capacity);

void* arenaPush(Arena* arena, int size, bool zero_out);

char* arenaPushString(Arena* arena, int size, char* string);

void arenaFree(Arena* arena); */

#endif