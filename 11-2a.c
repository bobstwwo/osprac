#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
  int msqid;      // IPC descriptor for the message queue
  char pathname[]="11-2a.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len, maxlen;      // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
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
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  /* Send information */

  for (i = 0; i < 5; i++) {
    
    mybuf.mtype = 1; //тип = 1
    mybuf.info.sinfo = i*2;
    mybuf.info.finfo = i*2/100.0;
    len = sizeof(mybuf.info);
    
    if (msgsnd(msqid, &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }
  }

  
  printf("First program sent all the messages it wanted.\n");


//GET MESSAGES
  for(i = 0; i < 5; i++) {
   
    maxlen = sizeof(mybuf.info);
//принимаем тип 2
    if (( len = msgrcv(msqid, &mybuf, maxlen, 2, 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }

    printf("First program got from second: message type = %ld, sinfo = %i, finfo = %f\n",
 mybuf.mtype, mybuf.info.sinfo, mybuf.info.finfo);
  }
  
  printf("First program got all the messages.\n");

  return 0;
}
