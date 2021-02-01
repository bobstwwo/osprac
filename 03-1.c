#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
  pid_t pid, ppid;
  int a = 0;

	while(1){
	if(a==4){
	   break;
	}else{
	  fork();
	  //
	  // If the new process is created successfully,
	  // two processes start working in pseudo-parallel from this point: the old and the new one.
	  //
	  // Before the following expression is executed,
	  // the value of the variable a in both processes is 0.
	  //
	  a = a+1;
	}
  }
	  pid  = getpid();
	  ppid = getppid();

	  printf("PID = %d, PPID = %d, RESULT = %d\n", (int)pid, (int)ppid, a);

  return 0;
}
