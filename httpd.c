#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

unsigned long PORT;

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("\033[1;33mUsage: ./httpd [OPTION]... ./DIR\033[0m\n");
        printf("\033[1;33mSample: ./httpd --port 8000 ./site\033[0m\n");
        exit(EXIT_SUCCESS);
    }
    if()
    return 0;
}
