#include <stdio.h>

int add(int x, int b)
{
    int result = 0;
    result = x + b;
    return result;
}

int main() 
{
    int a = 5;
    int b = 6;

    int sum = add(a,b);

    printf("\n\n SUM = %d\n\n", sum);
    return 0;
}