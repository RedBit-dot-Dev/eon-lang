#include "include/io.h"
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

long eonGetFileSize(FILE* file) {

    if (!file) return 0;

    fpos_t original_pos;

    if (fgetpos(file, &original_pos) != 0) {
        printf("Could not get file position (fgetpos), error %d\n", errno);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    long out = ftell(file);

    if (fsetpos(file, &original_pos) != 0) {
        printf("Could not set file position (fsetpos), error %d\n", errno);
        return 0;
    }

    return out;
}

char* eonReadFile(char* path) {

    FILE* file = fopen(path, "r");

    if (!file) {
        printf("Could not open file at '%s'\n", path);
        return NULL;
    }

    long size = eonGetFileSize(file);
    char* contents = (char*) malloc(size + 1); // +1 because of '\0'  
    assert(contents && "Could not allocate file contents");
    size_t bytes_read = fread(contents, 1, size, file);
    
    if (bytes_read != size) {
        free(contents);
        fclose(file);
        return NULL;
    }
    
    contents[size] = '\0';
    fclose(file);

    return contents;
}