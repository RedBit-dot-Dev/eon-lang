#include "include/error_codes.h"
#include "include/arena.h"
#include "include/lexer.h"
#include "include/parser.h"
#include "include/symbol_table.h"
#include "include/semantic_analysis.h"
#include "include/ir_gen.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>

unsigned int getFileSize(FILE* file) {

    fpos_t original_pos;

    if (fgetpos(file, &original_pos) != 0) {
        printf("Could not get file position (fgetpos), error %d\n", errno);
        exit(ERROR_FILE_POSITION);
    }

    fseek(file, 0, SEEK_END);
    unsigned int out = ftell(file);

    if (fsetpos(file, &original_pos) != 0) {
        printf("Could not set file position (fsetpos), error %d\n", errno);
        exit(ERROR_FILE_POSITION);
    }

    return out;
}

char* readFile(char* path, unsigned int* length_ptr) {

    FILE* file = fopen(path, "r");

    if (!file) {
        printf("Could not open file at '%s'\n", path);
        exit(ERROR_IMPROPER_PATH);
    }

    unsigned int size = getFileSize(file);
    char* contents = (char*) malloc(size + 1); // +1 because of '\0'  
    if (!contents) {
        printf("Insufficient memory to allocate contents\n");
        exit(ERROR_INSUFFICIENT_MEMORY_RAM);
    }

    size_t bytes_read = fread(contents, 1, size, file);

    if (bytes_read != size) {
        free(contents);
        fclose(file);
        exit(ERROR_FILE_SIZE_MISMATCH);
    }

    contents[size] = '\0';
    *length_ptr = size;
    fclose(file);

    return contents;
}

void runProgram(const char* path, char* const args[]) {

    pid_t pid = fork();

    if (pid == -1) {
        printf("fork failed\n");
        exit(1);
    }

    // is child process
    else if (pid == 0) {
        execvp(path, args);
        perror("execvp");
        exit(1);
    }

    // is parent, waiting for child
    else {
        waitpid(pid, NULL, 0);
    }

    return;
}

Arena* arena_main;

Symbol_Table* symbol_table_main;

int main(int argc, char** argv) {

    // TODO: handle flags
    if (argc < 2) {
        printf("USAGE: %s <path_to_file>\n", argv[0]);
        exit(ERROR_IMPROPER_USAGE);
    }

    unsigned int src_length;
    char* source_code = readFile(argv[1], &src_length);

    // Size I just pulled out of my ass
    arena_main = createArena(MiB(1));

    Lexer* lexer = createLexer(source_code, src_length);
    Parser* parser = createParser(lexer);
    AST* root = parseProgram(parser);

    free(source_code);

    symbol_table_main = createSymbolTable(NULL);
    analyseProgram(root);

    //printSymbolTable(symbol_table_main, 0);

    FILE* ir_file = fopen("IR.ll", "w");
    writeProgram(root, ir_file);
    fclose(ir_file);

    char* args1[] = {"clang", "IR.ll", NULL};
    runProgram(args1[0], args1);

    //symbolTableFree(symbol_table_main);

    arenaFree(arena_main);

    return 0;
}