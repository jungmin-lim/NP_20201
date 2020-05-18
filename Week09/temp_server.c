#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>
#include <sys/select.h>

#define BUF_SIZE 1024
#define MAX_CLIENT 100

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    struct timeval timeout;
    int str_len;
    fd_set read_fds, tmp_fds;
    socklen_t addr_sz;
    int fd, fd_num;
    char buf[BUF_SIZE];
    int i, max_fd;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if((bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) == -1){
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }

    FD_ZERO(&read_fds);
    FD_SET(serv_sock, &read_fds);
    max_fd = serv_sock;

    while(1){
        tmp_fds = read_fds;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(max_fd+1, &tmp_fds, 0, 0, &timeout)) == -1){
            break;
        }
        if(fd_num == 0){
            continue;
        }

        for(fd = 0; fd < max_fd+1; ++fd){
            if(FD_ISSET(fd, &tmp_fds)){
                if(fd == serv_sock){
                    addr_sz = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_sz);
                    FD_SET(clnt_sock, &read_fds);

                    if(max_fd < clnt_sock) max_fd = clnt_sock;
                    printf("connected client : %d \n", clnt_sock);
                }
                else{
                    memset(buf, 0, sizeof(buf));
                    str_len = read(fd, buf, BUF_SIZE);

                    if(str_len > 0){
                        
                    }
                }
            }
        }
    }
}