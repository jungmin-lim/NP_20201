#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
 
#define BUF_SIZE 1024
#define MAX_GAME 9 
#define MAX_STR 20

void error_handling(char *buf);
 
int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;
    socklen_t adr_sz;

    int fd_max, str_len, fd_num, opponent;
    int i, j;

    char buf[BUF_SIZE];

    int connection[50], current = 4;
 
    if(argc!=2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
 
    serv_sock=socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family=AF_INET;
    serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
    serv_adr.sin_port=htons(atoi(argv[1]));
    
    if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");
 
    if(listen(serv_sock, 5)==-1)
        error_handling("listen() error");
 
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max=serv_sock;
 
    // init
    for(i = 0; i < 50; ++i){
        connection[i] = -2;
    }

    while(1) {
        cpy_reads=reads;
        timeout.tv_sec=5;
        timeout.tv_usec=5000;
        
        if((fd_num=select(fd_max+1, &cpy_reads, 0, 0, &timeout))==-1) {
            break;
        }
        if(fd_num==0) {
            continue;
        }
        for(i=0; i<fd_max+1; i++) {
            if(FD_ISSET(i, &cpy_reads)) {
                // client connection
                if(i==serv_sock) {
                    adr_sz=sizeof(clnt_adr);
                    clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    
                    FD_SET(clnt_sock, &reads);
                    if(clnt_sock >= 50){
                        sprintf(buf, "CONNECTION FULL. TRY LATER");
                        write(clnt_sock, buf, strlen(buf));
                        close(clnt_sock);
                        continue;
                    }
                    if(fd_max<clnt_sock)
                        fd_max=clnt_sock;
                    printf("connected client: %d \n", clnt_sock);

                    if((current == clnt_sock) || (connection[current] != -1)){
                        sprintf(buf, "WAITING OPPONENT");
                        write(clnt_sock, buf, strlen(buf));
                        connection[clnt_sock] = -1;
                        current = clnt_sock;
                    }
                    else{
                        connection[current] = clnt_sock;
                        connection[clnt_sock] = current;
                        sprintf(buf, "OPPONENT FOUND. GAME START!");
                        write(current, buf, strlen(buf));
                        write(clnt_sock, buf, strlen(buf));

                        current = clnt_sock;
                    }
                }
                else {
                    str_len=read(i, buf, BUF_SIZE);
                    buf[str_len] = '\0';
                    fprintf(stdout, "Movement of %d : %s\n", i, buf);

                    // client connection lost 
                    if(str_len <=0) {
                        FD_CLR(i, &reads);
                        close(i);
                        fprintf(stdout, "closed client: %d \n", i);

                        if(connection[i] != -1){
                            FD_CLR(connection[i], &reads);
                            close(connection[i]);
                            fprintf(stdout, "closed client: %d\n", connection[i]);
                            connection[i] = -1;
                            connection[connection[i]] = -1;
                        }
                    }

                    // game play
                    else {
                        write(connection[i], buf, strlen(buf));
                    }
                }
            }
        }
    }
    close(serv_sock);
    return 0;
}
 
void error_handling(char *buf) {
    fputs(buf, stderr);
    fputc('\n', stderr);
    exit(1);
}