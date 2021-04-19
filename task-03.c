#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
    /*
    * Семафор инициализируется с единицы
    *	
    * Родитель - D(0,1) => in for loop:
    * запись => A(0,1) => Z => D(0,1) => чтение
    * Ребенок - in for loop:
    * D(0,2) => чтение => запись => A(0,1)
    */
    char pathname[] = "task-03.c";
    int result;		
    int fd[2];
    int communications_count; //N
    key_t key; // Ключ IPC
    int semid; // Дескриптор IPC для массива семафоров IPC    
    size_t size;
    char resstring[15];
    struct sembuf mybuf; // Структура для задания операций над семафором

    scanf("%d", &communications_count);
    if (pipe(fd) < 0) {
        printf("Can\'t open pipe\n");
        exit(-1);
    }
    if ((key = ftok(pathname, 0)) < 0) {
        printf("Impossible to generate the key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Error: impossible to create the semaphore\n");
        exit(-1);
    }
    mybuf.sem_num = 0;
    mybuf.sem_op = 1;
    mybuf.sem_flg = 0;
    if (semop(semid, &mybuf, 1) < 0) {
        printf("Can\'t set semaphore value as 1\n");
        exit(-1);
    }
    result = fork();
    if (result < 0) {
        printf("Can\'t fork the child process. Terminating\n");
        exit(-1);
    }
    else if (result > 0) { /* Процесс родитель */
        mybuf.sem_num = 0;
        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;
        if (semop(semid, &mybuf, 1) < 0) {
            printf("Can\'t decline semaphore value by 1\n");
            exit(-1);
        }

        for (int i = 0; i < communications_count; ++i) {
            size = write(fd[1], "Hello, child!", 14);
            if (size != 14) {
                printf("Parent: unable to write all the chars to the pipe\n");
                exit(-1);
            }
            mybuf.sem_num = 0;
            mybuf.sem_op = 2;
            mybuf.sem_flg = 0;

            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly in program A\n");
                exit(-1);
            }
            mybuf.sem_num = 0;
            mybuf.sem_op = 0;
            mybuf.sem_flg = 0;
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly in program A\n");
                exit(-1);
            }
	    mybuf.sem_num = 0;
            mybuf.sem_op = -1;
            mybuf.sem_flg = 0;
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly in program A\n");
                exit(-1);
            }
            size = read(fd[0], resstring, 15);
            if (size < 0) {
                printf("Can\'t read string from pipe\n");
                exit(-1);
            }
            printf("Parent: read the string: %s\n", resstring);
        }
        printf("Parent process finished job\n");
    }
    else { /* Процесс ребенок */       
        for (int i = 0; i < communications_count; ++i) {
            mybuf.sem_num = 0;
            mybuf.sem_op = -2;
            mybuf.sem_flg = 0;
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly\n");
                exit(-1);
            }
            size = read(fd[0], resstring, 14);
            if (size < 0) {
                printf("Child: Impossible to read the string from the pipe\n");
                exit(-1);
            }
            printf("Child: read the string: %s\n", resstring);
            size = write(fd[1], "Hello, parent!", 15);
            if (size != 15) {
                printf("Child: unable to write all the chars to the pipe\n");
                exit(-1);
            }    
            mybuf.sem_num = 0;
            mybuf.sem_op = 1;
            mybuf.sem_flg = 0;
            if (semop(semid, &mybuf, 1) < 0) {
                printf("Can\'t enter the critical section properly\n");
                exit(-1);
            }
        }
        if (close(fd[0]) < 0) {
            printf("Child: Can\'t close reading side of pipe\n"); 
            exit(-1);
        }
        if (close(fd[1]) < 0) {
            printf("Child: Can\'t close writing side of pipe\n"); 
            exit(-1);
        }
    }
    return 0;
}
