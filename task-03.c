#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/sem.h>
#include <stdlib.h>

int main()
{
    struct sembuf mybuf;
    int     semid;
    key_t   key;
    char    pathname[] = "task-03.c";

    int     fd[2], result;
    size_t  size;
    char    resstring[14];

    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if ((key = ftok(pathname, 0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666)) < 0) {
        printf("Semaphore not found.\n");
        if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
            printf("Can\'t get semid\n");
            exit(-1);
        }
        printf("Created successfully.\n");
    }

    int n;
    scanf("%d", &n);

    result = fork();

    if (result < 0) {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {

        /* Parent process */

        for (int i = 0; i < n; ++i) {
            size = write(fd[1], "Hello, world!", 14);

            if (size != 14) {
                printf("Can\'t write all string to pipe\n");
                exit(-1);
            } else {
                printf("Parent wrote string.\n");
            }

            if (close(fd[1]) < 0) {
                printf("parent: Can\'t close pipe writing\n");
                exit(-1);
            }

            mybuf.sem_op = -1;
            mybuf.sem_flg = 0;
            mybuf.sem_num = 0;

            semop(semid, &mybuf, 1);

            size = read(fd[0], resstring, 14);
            if (size < 0) {
                printf("Parent can\'t read string\n");
                exit(-1);
            }

            printf("Parent read: %s\n", resstring);

            if (close(fd[0]) < 0) {
                printf("parent: Can\'t close pipe reading\n");
                exit(-1);
            }

            printf("Parent exit\n");
        }
    } else {

        /* Child process */
        for (int i = 0; i < n; ++i){
            size = read(fd[0], resstring, 14);
            if (size < 0) {
                printf("Can\'t read string\n");
                exit(-1);
            }
            printf("Child read: %s\n", resstring);

            if (close(fd[0]) < 0) {
                printf("child: Can\'t close pipe reading\n"); 			exit(-1);
            }

            for (int i = 0; i < 13 / 2; i++) {
                char t = resstring[i];
                resstring[i] = resstring[13 - i - 1];
                resstring[13 - i - 1] = t;
            }

            size = write(fd[1], resstring, 14);

            mybuf.sem_op = 1;
            mybuf.sem_flg = 0;
            mybuf.sem_num = 0;

            semop(semid, &mybuf, 1);

            if (size != 14) {
                printf("Child can\'t write all string\n");
                exit(-1);
            } else {
                printf("Child wrote string.\n");
            }

            if (close(fd[1]) < 0) {
                printf("child: Can\'t close pipe writing\n"); 			exit(-1);
            }

            printf("Child exit.\n");
        }
    }

    return 0;
}
