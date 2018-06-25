#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

int PORT = 8000; //default

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("\033[1;33mUsage: ./httpd [OPTION]... ./DIR\033[0m\n");
        printf("\033[1;33mSample: ./httpd --port 8000 ./site\033[0m\n");
        exit(EXIT_SUCCESS);
    }
    if(argc >= 2) {
        if( strcmp(argv[1], "-p") == 0 ||
            strcmp(argv[1], "--port") == 0) {
            assert(argc == 4);
            PORT = atoi(argv[2]);
            printf("-p %d\n", PORT);
        }else if(strcmp(argv[1], "-h") == 0 ||
                 strcmp(argv[1], "--help") == 0) {
            assert(argc == 2);
            printf("-h\n");
        }
    }
    return 0;
}
