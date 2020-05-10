#include <stdio.h>
#include <unistd.h>
int main(){
    pid_t pid;
    if(fork() || (pid = fork())){
        pid = fork();
    }
    printf("1 ");
    return 0;
}