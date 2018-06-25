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
static char html[1 << 20];

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
//    static char response[1 << 20];
//    sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\n%s\r\n", filesize, index);

    while((conn = accept(servfd, (struct sockaddr *)&client_addr, &length)) != -1) {
		const char response[] = 
			"HTTP/1.1 200 OK\r\n"
			"Content-Length: 11\r\n"
			"\r\n"
			"Fuck You Elton\n";

		int len = write(conn, response, sizeof(response));
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
    
    DIR *site = NULL;
    struct dirent *entry;
    char *pwd = get_current_dir_name();
    char path[128];
    sscanf(argv[3], "./%s", path);
    assert((site = opendir(path)) != NULL);
    while((entry = readdir(site)) != NULL) {
        if(strcmp(entry->d_name, "index.html") == 0) {
/*            sprintf(pwd, "%s/%s/%s", pwd, path, entry->d_name);
            FILE *fp = fopen(pwd, "r");
            fseek(fp, 0L, SEEK_END);
            int filesize = ftell(fp);
            fseek(fp, 0L, SEEK_SET);
            assert(fread(html, 1, filesize, fp) > 0);
            fclose(fp);*/
        }
        if(entry->d_type & DT_DIR) {
            if(strcmp(entry->d_name, ".") == 0
                || strcmp(entry->d_name, "..") == 0)
                continue;
        }
    }

    if(argc >= 2) {
        if( (strcmp(argv[1], "-p") == 0 ||
            strcmp(argv[1], "--port") == 0) && argc == 4) {
            servport = atoi(argv[2]);
            server(servport, argv[3]);
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
