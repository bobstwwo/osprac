#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
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

    struct servermsgbuf {
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

    clientbuf.mtype = 1;
    clientbuf.info.pid = getpid();

    printf("Float number = ?\n");
    float a;
    scanf("%f", &a);

    len = sizeof(clientbuf.info);
    clientbuf.info.message = a;

    printf("Client with pid = %d is sending number = %f\n", clientbuf.info.pid, clientbuf.info.message);
    if (msgsnd(msqid, (struct clientmsgbuf *) &clientbuf, len, 0) < 0) {
        printf("Can't send message to queue\n");
        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
        exit(-1);
    }

    printf("Waiting the answer\n");

    max = sizeof(serverbuf.info);
    if (len = msgrcv(msqid, &serverbuf, max, getpid(), 0) < 0) {
        printf("Can't receive message from queue\n");
        exit(-1);
    }
    printf("Squared number = %f\n", serverbuf.info.message);

    return 0;

}
