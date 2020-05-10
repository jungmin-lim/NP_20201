#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define MAX_STUDENT 10
#define MAX_COURSE 10

typedef struct student{
    int student_id;
    int course_count;
    int course_id[MAX_COURSE];
    char course_name[MAX_COURSE][50];
    int score[MAX_COURSE];
}STUDENT;

// students info : {student_id, course_count, course_id, course_name, score}
STUDENT students[MAX_STUDENT] = {
    {2012345678, 3, {1, 2, 3}, {"course 1", "course 2", "course 3"}, {70, 80, 90}},
    {2017116186, 4, {1, 2, 3, 4}, {"course 0", "course 1", "course 2", "course 3", "course 4"}, {60, 70, 80, 90}},
    {2019012345, 2, {1, 2}, {"course 5", "course 6"}, {90, 100}}
};

void error_handling(char *message);

int main(int argc, char *argv[]){
    int serv_sock, clnt_sock;
    int selection, id, num, i;
    char message[BUF_SIZE], str[20];
    struct sockaddr_in serv_adr, clnt_adr;
    socklen_t clnt_adr_sz;

    if(argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
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
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        fputs("New client connected...\n", stdout);

        strcpy(message, "Enter your student ID");
        write(clnt_sock, message, strlen(message));

        // read student id from client
        read(clnt_sock, &id, sizeof(id));
        fprintf(stdout, "Client: %d\n", id);

        // search student info
        i = 0;
        while(students[i].student_id != id){
            i++;
        }
        num = i;

        // write available courses
        message[0] = '\0';
        for(i = 0; i < students[num].course_count; ++i){
            sprintf(str, "%d ", students[num].course_id[i]);
            strcat(message, str);
            strcat(message, students[num].course_name[i]);
            strcat(message, "\n");
        }
        write(clnt_sock, message, strlen(message));

        strcpy(message, "choose your course number\n\0");
        write(clnt_sock, message, strlen(message));

        // read selected course
        read(clnt_sock, &id, sizeof(id));
        fprintf(stdout, "Client: %d\n", id);

        // write course number and score
        i = 0;
        while(students[num].course_id[i] != id){
            i++;
        }
        message[0] = '\0';
        strcat(message, students[num].course_name[i]);
        sprintf(str, " %d", students[num].score[i]);
        strcat(message, str);
        strcat(message, "\0");

        write(clnt_sock, message, strlen(message));

        // close client socket
        close(clnt_sock);
    }
    return 0;
}

void get_studentinfo(){
    FILE *f;

    f = fopen("info.txt", "r");
}

void error_handling(char *message){
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}