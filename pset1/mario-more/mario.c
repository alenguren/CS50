#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n, i, j, k;
    do
    {
        // Taker user input for the hashes
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

// For each row
    for (i = 0; i < n; i++)
    {
        for (k = 0; k < n - i - 1; k++) // to print spaces
        {
            printf(" ");
        }
        for (j = 0; j <= i; j++) // to print the left pyramid
        {
            printf("#");
        }
        printf("  ");
        for (j = 0; j <= i; j++) // to print the right pyramid
        {
            printf("#");
        }
        printf("\n");
    }
}