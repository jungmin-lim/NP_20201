#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

#define BUF_SIZE 40

void error_handling(char *message);

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock, str_len;
    char buf[BUF_SIZE], name[BUF_SIZE];
    struct sockaddr_in serv_adr, clnt_adr;
    struct hostent *host;
    socklen_t clnt_adr_sz;

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }

    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }

    clnt_adr_sz = sizeof(clnt_adr);
    while(1){
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
        printf("new client connected...\n");
        while(1){
            str_len = read(clnt_sock, &buf, BUF_SIZE);
            memcpy(name, buf, str_len);
            name[str_len] = '\0';
            if(!strcmp(name, "q") || !strcmp(name, "Q")){
                break;
            }

            host = gethostbyname(name);
            if(!host){
                write(clnt_sock, "gethost...error", strlen("gethost...error"));
            }
            else{
                write(clnt_sock, inet_ntoa(*(struct in_addr*)host->h_addr_list[0]), strlen(inet_ntoa(*(struct in_addr*)host->h_addr_list[0])));
            }
        }
        close(clnt_sock);
        printf("client disconnected...\n");
    }

    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}