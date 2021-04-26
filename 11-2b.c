#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255

int main(void)
{
  int msqid;
  char pathname[]="11-2a.c";
  key_t  key;
  int i, len, maxlen;

  struct mymsgbuf
  {
    long mtype;
    struct
    {
       short sinfo;
       float finfo;
    } info;
  } mybuf;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }

  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  for(i=1; i<=5; i++) {
    maxlen = sizeof(mybuf.info);
//принимаем тип 1
    if (( len = msgrcv(msqid, &mybuf, maxlen, 1, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    
    printf("Second program got from first: message type = %ld, sinfo = %i, finfo = %f\n",
 mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
  }


  printf("Second program got all the messages.\n");



//SEND
/* Send information */

  for (i = 0; i < 5; i++) {
    
    mybuf.mtype = 2; //тип = 2
    mybuf.info.sinfo = i*2 + 1;
    mybuf.info.finfo = (i*2 + 1)/100.0;
    len = sizeof(mybuf.info);
    
    if (msgsnd(msqid, &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }


  printf("Second program sent all the messages it wanted.\n");

  return 0;
}
