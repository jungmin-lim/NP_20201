#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include "chess.h"
 
#define BUF_SIZE 100
 
void error_handling(char *message);
 
 
int main(int argc, char *argv[]) {
    int sock;
    pid_t pid;
    char buf[BUF_SIZE];
    struct sockaddr_in serv_adr;
    
    int str_len, result;
    int player = 1, state = 1;
    int i;

    if(argc!=3) {
        printf("Usage : %s <IP> <port> \n", argv[0]);
        exit(1);
    }

    sock=socket(PF_INET, SOCK_STREAM, 0);  
    if(sock == -1){
        error_handling("socket() error!\n");
        exit(1);
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
    serv_adr.sin_port=htons(atoi(argv[2]));
    
    if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr))==-1){
        error_handling("connect() error!\n");
    }
    else{
        fprintf(stdout, "Connected.....\n");
    }
 
    str_len = read(sock, buf, BUF_SIZE);
    if(str_len <= 0){
        error_handling("CONNECTION LOST!");
    }

    buf[str_len] = '\0';
    fprintf(stdout, "%s\n", buf);

    // TO DO : GAME START
    if(!strcmp(buf, "WAITING OPPONENT")){
        state = -1; // START NEXT
        player = -1;

        str_len = read(sock, buf, BUF_SIZE);
        if(str_len <= 0){
            error_handling("CONNECTION LOST!");
        }
        buf[str_len] = '\0';
        fprintf(stdout, "%s\n", buf);
    }

    else {
        state *= 1; // START FIRST
        player *= 1;
    }

    while(1) {
        // Wait for opponent
        if(state == -1){
            print_board();
            str_len = read(sock, buf, BUF_SIZE-1);
            buf[str_len] = '\0';
            if(str_len <= 0){
                fprintf(stderr, "connection lost! game over\n");
                close(sock);
                return 1;
            }
            // MOVE
            else{
                result = input(buf, player*state);
                print_board();
                if(result == -1){
                    fprintf(stdout, "INVALID MOVE! NEED DEBUG\n");
                    fprintf(stdout, "buf: %s\n", buf);
                    fprintf(stdout, "PLAYER = %d STATE = %d", player, state);
                    return 0;
                }
                else if(result == 100){
                    fprintf(stdout, "YOU LOSE!\n");
                    close(sock);
                    return 0;
                }
                state = 1;
            }
        }

        // Player's turn
        else{ 
            while(1){
                print_board();
                fprintf(stdout, "INPUT YOUR MOVE: ");
                scanf("%s", buf);

                // MOVE
                result = input(buf, player*state);
                if(result == -1){
                    fprintf(stdout, "INVALID MOVE! TRY AGAIN\n");
                    sleep(2);
                    continue;
                }
                print_board();
                if(write(sock, buf, strlen(buf))== -1){
                    fprintf(stderr, "connection lost! game over\n");
                    close(sock);
                    return 1;
                }
                if(result == 100){
                    fprintf(stdout, "YOU WON!\n");
                    close(sock);
                    return 0;
                }
                state = -1;
                break;
            }
        }
    }

    close(sock);
    return 0;
}
 
void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}