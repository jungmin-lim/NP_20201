#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_routine(FILE *readfp, char *buf){
    while(1){
        if(fgets(buf, BUF_SIZE-1, readfp) == NULL){
            return;
        }
        buf[strlen(buf)] = '\0';
        fprintf(stdout, "Message from Server : %s", buf);
    }
}

void write_routine(FILE *writefp, char *buf){
    while(1){
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")){
            shutdown(fileno(writefp), SHUT_WR);
            return;
        }
        fputs(buf, writefp);
        fflush(writefp);
    }
}

int main(int argc, char *argv[]){
    FILE *readfp, *writefp;
    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;
    
    if(argc != 3){
        fprintf(stdout, "Usage : %s <IP> <PORT> \n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("connect() error");
    }

    readfp = fdopen(sock, "r");
    writefp = fdopen(sock, "w");

    pid = fork();
    if(pid == 0){
        read_routine(readfp, buf);
    }
    else{
        write_routine(writefp, buf);
    }

    fclose(writefp);
    fclose(readfp);
    return 0;
}