#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char *message);

int main(int argc, char *argv[]){
    int sock;
    int selection, id, num, i;
    char message[BUF_SIZE];
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) ==  -1){
        error_handling("connect() error");
    }

    memset(message, 0, BUF_SIZE);
    read(sock, message, BUF_SIZE);
    fprintf(stdout, "Server: %s\n", message);

    // write student id
    fprintf(stdout, "Client: ");
    scanf("%d", &id);
    write(sock, &id, sizeof(id));
        
    // read course info
    memset(message, 0, BUF_SIZE);
    read(sock, message, BUF_SIZE);
    fputs(message, stdout);

    // choose course id 
    fprintf(stdout, "Client: ");
    scanf("%d", &id);
    write(sock, &id, sizeof(id));

    // read score
    memset(message, 0, BUF_SIZE);
    read(sock, message, BUF_SIZE);
    fprintf(stdout, "%s\n", message);

    close(sock);
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}