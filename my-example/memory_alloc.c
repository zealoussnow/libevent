// Last Update:2015-10-19 18:00:11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[])
{
	int *p = NULL;
	int i;
	p = (int *)malloc(sizeof(int));
	p[0] = 1;

	printf("%p\n", p);
	int *q = realloc(p, 4);
	printf("%p\n", q);
	q[1] = 2;
	q[2] = 3;
	for (i = 0; i < 4; i++)
	{
		printf("%d\n", p[i]);
	}
	
	return 0;
}
