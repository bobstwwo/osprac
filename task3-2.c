#include <sys/types.h>
#include <unistd.h>
#include <sys/msg.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <stdlib.h>


int main() {
    int msqid;
    char pathname[] = "task3-2.c";
    key_t key;
    int len, max;

    struct clientmsgbuf {
        long mtype;
        struct {
            pid_t pid;
            float message;
        } info;
    } clientbuf;

    struct sermsgbuf {
        long mtype;
        struct {
            float message;
        } info;
    } serverbuf;

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can't generate key\n");
        exit(-1);
    }

    if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
        printf("Can't get msqid\n");
        exit(-1);
    }

    while (1) {
        max = sizeof(clientbuf.info);
        if (len = msgrcv(msqid, (struct clientmsgbuf *) &clientbuf, max, 1, 0) < 0) {
            printf("Can't receive message from queue\n");
            exit(-1);
        }
        printf("Client with pid = %d is sending number = %f\n", clientbuf.info.pid, clientbuf.info.message);
     
        serverbuf.mtype = clientbuf.info.pid;
        
        serverbuf.info.message = clientbuf.info.message*clientbuf.info.message;
        len = sizeof(serverbuf.info);

        if (msgsnd(msqid, (struct sermsgbuf *) &serverbuf, len, 0) < 0) {
            printf("Can't send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }
        printf("Done!\n");
    }
    return 0;
}
