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
	
	int index = 0;
	while (envp[index] != NULL){
		printf("%s\n", envp[index++]);
		printf("\n");
	}
  return 0;
}
