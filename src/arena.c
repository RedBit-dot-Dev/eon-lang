#include "include/arena.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Arena* createArena(uint64_t capacity) {

    Arena* arena = (Arena*) malloc(sizeof(Arena));

    arena->memory = malloc(capacity);
    arena->pos = arena->memory;
    arena->capacity = capacity;
    arena->int_pos = 0;

    return arena;
}

void* arenaPush(Arena* arena, int size, bool zero_out) {

    arena->int_pos += size;

    if (arena->int_pos > arena->capacity) {
        printf("Out of memory\n");
        exit(1);
    }

    void* result = arena->pos;

    if (zero_out) memset(arena->pos, 0, size * sizeof(void*));

    arena->pos += size * sizeof(void*);

    return result;
}

char* arenaPushString(Arena* arena, int size, char* string) {

    char* str = (char*) arenaPush(arena, size, false);

    strcpy(str, string);

    return str;
}

void arenaFree(Arena* arena) {

    free(arena->memory);
    free(arena);

    return;
}


/* Arena* createArena(uint64_t capacity) {

    Arena* arena = (Arena*) malloc(capacity);

    arena->capacity = capacity;
    arena->pos = sizeof(Arena);

    return arena;
}

void* arenaPush(Arena* arena, int size, bool zero_out) {

    uint64_t pos_aligned = ALIGN_UP_POW2(arena->pos, ARENA_ALIGNMENT);
    uint64_t new_pos = pos_aligned + size;

    if (new_pos > arena->capacity) {
        printf("Out of memory\n");
        exit(1);
    }

    arena->pos = new_pos;

    uint8_t* out = (uint8_t*) arena + pos_aligned;

    if (zero_out) memset(out, 0, size);

    return out;
}

// parameter size is strlen + 1 (null terminator) 
char* arenaPushString(Arena* arena, int size, char* string) {

    char* str = (char*) arenaPush(arena, sizeof(char*) * size, false);

    strcpy(str, string);

    return str;
}

void arenaFree(Arena* arena) {
    free(arena);
} */