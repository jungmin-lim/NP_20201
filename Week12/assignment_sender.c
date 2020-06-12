#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TTL 64
#define BUF_SIZE 1024

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char *argv[]){
    struct sockaddr_in mul_adr;
    pid_t pid;
    int time_live = TTL;
    int send_sock;
    int i;
    char buf[BUF_SIZE];

    char message1[] = "Message1: Hi!";
    char message2[] = "Message2: I'm another UDP host!";
    char message3[] = "Message3: Nice to meet you";

    if(argc != 7){
        fprintf(stdout, "Usage: %s <IP> <port1> <port2> <port3> <port4> <port5>", argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET, SOCK_DGRAM, 0);

    for(i = 2; i < 7; ++i){
        pid = fork();
        if(pid == 0){
            break;
        }
    }
    if(pid > 0) {
        close(send_sock);
        return 0;
    }

    memset(&mul_adr, 0, sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[i]));

    setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (void *)&time_live, sizeof(time_live));


    sprintf(buf, "%s\n", message1);
    sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_adr, sizeof(mul_adr));
    sprintf(buf, "%s\n", message2);
    sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_adr, sizeof(mul_adr));
    sprintf(buf, "%s\n", message3);
    sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr *)&mul_adr, sizeof(mul_adr));

    close(send_sock);
    return 0;
}