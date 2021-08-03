#include <stdio.h>

int main()
{
	int i = 0;

	int n = 5;
	int fork2;


	while(i < n)
	{
		fork2 = (i + 1)% n ;
		printf("i = %d, fork2 = %d\n", i, fork2);
		i++;
	}
}