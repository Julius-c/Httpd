#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

int port = 8000; //default
void server(int port, char *file) {
    printf("%d %s\n", port, file);
}

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("\033[1;33mUsage: ./httpd [OPTION]... ./DIR\033[0m\n");
        printf("\033[1;33mSample: ./httpd --port 8000 ./site\033[0m\n");
        exit(EXIT_SUCCESS);
    }
    if(argc >= 2) {
        if( (strcmp(argv[1], "-p") == 0 ||
            strcmp(argv[1], "--port") == 0) && argc == 4) {
            port = atoi(argv[2]);
            server(port, argv[3]);
        }else if( (strcmp(argv[1], "-h") == 0 ||
                  strcmp(argv[1], "--help") == 0) && argc == 2) {
        }else{
            printf("Unknown command '%s'\n", argv[1]);
        }
    }
    return 0;
}
