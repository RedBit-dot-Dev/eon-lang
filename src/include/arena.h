#ifndef EON_ARENA_H
#define EON_ARENA_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define KiB(n) ((uint64_t)(n) << 10)
#define MiB(n) ((uint64_t)(n) << 20)
#define GiB(n) ((uint64_t)(n) << 30)

typedef struct _arena {
    uint64_t offset;
    uint64_t capacity;
    char* memory;
    char* pos;
} Arena;

Arena* createArena(uint64_t capacity);

char* arenaPush(Arena* arena, size_t size, bool zero_out);

char* arenaPushString(Arena* arena, int size, char* string);

void arenaFree(Arena* arena);

#endif