#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]){
    int fds[2];
    char str[] = "Who are you?";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    pid = fork();

    if(pid == 0){
        write(fds[1], str, strlen(str));
    }
    else{
        read(fds[0], buf, BUF_SIZE);
        fputs(buf, stdout);
    }
    return 0;
}