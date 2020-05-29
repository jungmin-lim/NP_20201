#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]){
    int fds[2], str_len;
    char str1[] = "Who are you?";
    char str2[] = "Thnak you for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds);
    pid = fork();

    if(pid == 0){
        write(fds[1], str1, strlen(str1));
        sleep(2);
        str_len = read(fds[0], buf, BUF_SIZE);
        buf[str_len] = '\0';
        fprintf(stdout, "Child Process Output : %s\n", buf);
    }
    else{
        str_len = read(fds[0], buf, BUF_SIZE);
        buf[str_len] = '\0';
        fprintf(stdout, "Parent Process Output : %s\n", buf);
        write(fds[1], str2, strlen(str2));
        sleep(3);
    }

    return 0;
}