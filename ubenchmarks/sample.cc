#include <stdio.h>

int main()
{
    register int b = 5;
    //register int *d;
    register int a = 7;

    //(int i=0;i<1000;i++)
    if(a%2 == 0) 
        a++;
    else
        b++;
    
    //reconverge
    register int c = a+b;
    c++;
    a = b-c;
    b++;

    printf("%d %d\n",a,b);
    return 0;
}
