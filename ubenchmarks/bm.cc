#include<stdio.h> 
#include<process.h>
#include<stdlib.h>

int main()
{
	register int a = 0;
	register int b = 0;
	register int d = 0;
	int c,i;

	for(int i=0;i<10000;i++)
	{
		c = rand();
		if(c%2)
			a=a+1;
		else
			b=b+1;

		d = a%10+b%10;

		a = a / 10;
		b = b / 10;

        a = a/b;
        a = a+b;
        b = b/a;
        b++;
        a -= b;
        a = rand();
        b = rand();


	}

	printf("The values are %d, %d \n",a,b);


	return 0;
}
