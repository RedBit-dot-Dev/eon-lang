#ifndef EON_IO_H
#define EON_IO_H
#include <stdio.h>

long eonGetFileSize(FILE* file);

char* eonReadFile(char* path);

#endif