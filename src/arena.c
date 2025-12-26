#include "include/arena.h"
#include "include/error_codes.h"
#include <stdlib.h>
#include <string.h>

Arena* createArena(uint64_t capacity) {

    Arena* arena = malloc(sizeof(Arena));

    arena->capacity = capacity;
    arena->offset = 0;
    arena->memory = calloc(capacity, sizeof(char));
    arena->pos = arena->memory;

    return arena;
}

char* arenaPush(Arena* arena, size_t size, bool zero_out) {

    if (arena->offset + size > arena->capacity) {
        printf("Insufficient memory in arena to allocate item(s),\n");
        exit(ERROR_INSUFFICIENT_MEMORY_ARENA);
    }

    char* ptr = arena->pos;

    if (zero_out) memset(arena->pos, 0, size);

    arena->pos += size;
    arena->offset += size;

    return ptr;
}

char* arenaPushString(Arena* arena, int size, char* string) {

    char* ptr = arenaPush(arena, size, false);

    strcpy(ptr, string);

    return ptr;
}

void arenaFree(Arena* arena) {

    free(arena->memory);
    free(arena);

    return;
}