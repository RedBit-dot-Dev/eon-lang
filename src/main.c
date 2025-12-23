#include "include/io.h"
#include "include/eon.h"
#include "include/lexer.h"
#include "include/arena.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Please specify input file\n");
        return 1;
    }

    char* file_contents = eonReadFile(argv[1]);

    // capacity size I pulled right out of my ass
    arena_main = createArena(MiB(1));
    eonCompile(file_contents);

    free(file_contents);

    char* args1[] = {"clang", "IR.ll", NULL};
    runProgram(args1[0], args1);

    // TODO: uncomment this
    // deletes IR.ll
    //char* args2[] = {"rm", "IR.ll", NULL};
    //runProgram(args2[0], args2);

    return 0;
}