#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n = 50;
    int *p = &n;
    int c = n;
    printf("%p\n", p);
    printf("%i\n", *p);
}