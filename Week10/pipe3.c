#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]){
    int fds1[2], fds2[2], str_len;
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds1), pipe(fds2);
    pid = fork();

    if(pid == 0){
        write(fds1[1], str1, strlen(str1));
        str_len = read(fds2[0], buf, BUF_SIZE);
        buf[str_len] = '\0';
        fprintf(stdout, "Child Process Output : %s\n", buf);
    }
    else{
        str_len = read(fds1[0], buf, BUF_SIZE);
        buf[str_len] = '\0';
        fprintf(stdout, "Parent Process Output : %s\n", buf);
        write(fds2[1], str2, strlen(str2));
        sleep(3);
    }

    return 0;
}