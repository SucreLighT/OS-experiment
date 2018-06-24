#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
	for( i = 1; i < argc; i++) //i = 0, start with the second parameter
	{
		printf("%s", argv[i]);
		if(i < (argc - 1))
		printf(" ");
//		else
//		printf("");
	}
	printf("\n");
	return 0;
}

