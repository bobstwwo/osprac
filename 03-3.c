#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
  printf("Argc = %d",argc);
  for(int i = 0; i<argc; i++){
	printf("Element of array argv = %d",i);
	printf("\n");
        puts(argv[i]);
	printf("\n");
	}
  for(int i = 0; i<argc; i++){
	printf("Element of array envp = %d",i);
	printf("\n");
        puts(envp[i]);
	printf("\n");
	}
  return 0;
}
