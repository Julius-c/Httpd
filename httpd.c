#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int servfd = -1;
int servport = 8000; //default
#define BUFSIZE 1<<20
#define true 1
#define false 0

void sigint_handler(int signum) {
    printf("\nReceive Keyboard Interrupt, Close Server.\n");
    close(servfd);
    exit(EXIT_SUCCESS);
}

void server(int servport, char *dir) {
    servfd = socket(AF_INET, SOCK_STREAM, 0);
    signal(SIGINT, sigint_handler);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(servport);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    assert(bind(servfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != -1);

    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    int conn = -1;
    
    listen(servfd, 50);

    while((conn = accept(servfd, (struct sockaddr *)&client_addr, &length)) != -1) {
        char request[BUFSIZE];
        int recb = recv(conn, request, BUFSIZE, 0);
        request[recb] = '\0';
        char method[BUFSIZE], url[BUFSIZE];
        sscanf(request, "%s %s", method, url);
        printf("%s %s\n", method, url);
    
        char html[BUFSIZE];
        char response[BUFSIZE];
        char pwd[BUFSIZE];
        if(strcmp(url, "/") == 0)
            sprintf(pwd, "%s/index.html", dir);
        else sprintf(pwd, "%s%s", dir, url);

        int size;
        int fd = open(pwd, O_RDONLY);
        if(fd == -1) {
            sprintf(response, 
			    "HTTP/1.1 200 OK\r\n"
			    "Content-Length: 350\r\n"
			    "\r\n"
			    "<html> \
                <head> \
                <title>Hello World</title> \
                </head> \
                <body> \
                <center> \
                <hr> \
                <h1> OSMINILAB </h1> \
                <h2> 404 </h2> \
                <font size = 2> \
                Welcome to HTTPD! \
                <br></br> \
                161240003 \
                <br></br> \
                CHEN Cheng \
                </font> \
                </center> \
                </body> \
                </html>\r\n");
            }
        else {
            size = lseek(fd, 0, SEEK_END);
            lseek(fd, 0, SEEK_SET);
            read(fd, html, size);
            sprintf(response,
                "HTTP/1.1 200 OK\r\n"
			    "Content-Length: %d\r\n"
			    "\r\n", size);
        }
 		assert( send(conn, response, strlen(response), 0) > 0);
        assert( send(conn, html, size, 0) > 0);
		close(conn);
    }
}

static struct{
    char *name;
    char *description;
} cmd_table [] = {
    { "--help or -h", "Display informations about all supported commands." },
    { "--port or -p", "Set up the listening port." }
};
#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

int main(int argc, char *argv[]) {
    if(argc == 1) {
        printf("\033[1;33mUsage: ./httpd [OPTION]... ./DIR\033[0m\n");
        printf("\033[1;33mSample: ./httpd --port 8000 ./site\033[0m\n");
        exit(EXIT_SUCCESS);
    }
    
    if(argc >= 2) {
        if( (strcmp(argv[1], "-p") == 0 ||
            strcmp(argv[1], "--port") == 0) && argc == 4) {
            servport = atoi(argv[2]);
            char dir[BUFSIZE];
            sscanf(argv[3], "./%s", dir);
            server(servport, dir);
        }else if( (strcmp(argv[1], "-h") == 0 ||
                  strcmp(argv[1], "--help") == 0) && argc == 2) {
            for(int i = 0; i < NR_CMD; i ++)
                printf("\033[1;36m%s\033[0m \033[1m->\033[0m \033[1;45m%s\033[0m\n", cmd_table[i].name, cmd_table[i].description);
        }else{
            printf("\033[1;31mUnknown or Incomplete command '%s'\033[0m\n", argv[1]);
        }
    }
    return 0;
}
