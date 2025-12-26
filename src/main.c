#include "include/error_codes.h"
#include "include/arena.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

long getFileSize(FILE* file) {

    fpos_t original_pos;

    if (fgetpos(file, &original_pos) != 0) {
        printf("Could not get file position (fgetpos), error %d\n", errno);
        exit(ERROR_FILE_POSITION);
    }

    fseek(file, 0, SEEK_END);
    long out = ftell(file);

    if (fsetpos(file, &original_pos) != 0) {
        printf("Could not set file position (fsetpos), error %d\n", errno);
        exit(ERROR_FILE_POSITION);
    }

    return out;
}

char* readFile(char* path) {

    FILE* file = fopen(path, "r");

    if (!file) {
        printf("Could not open file at '%s'\n", path);
        exit(ERROR_IMPROPER_PATH);
    }

    long size = getFileSize(file);
    char* contents = (char*) malloc(size + 1); // +1 because of '\0'  
    assert(contents && "Could not allocate file contents");
    size_t bytes_read = fread(contents, 1, size, file);

    if (bytes_read != size) {
        free(contents);
        fclose(file);
        exit(ERROR_FILE_SIZE_MISMATCH);
    }

    contents[size] = '\0';
    fclose(file);

    return contents;
}

Arena* arena_main;

int main(int argc, char** argv) {

    // TODO: handle flags
    if (argc < 2) {
        printf("USAGE: %s <path_to_file>\n", argv[0]);
        exit(ERROR_IMPROPER_USAGE);
    }

    char* source_code = readFile(argv[1]);

    // Size I just pulled out of my ass
    arena_main = createArena(MiB(1));

    free(source_code);

    arenaFree(arena_main);

    return 0;
}