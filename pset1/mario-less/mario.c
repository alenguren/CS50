#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        // Taker user input for the hashes
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

// For each row
    for (int i = 0; i < n; i++)
    {
        // For each column
        for (int j = 0; j < n; j++)
        {
            // Print the blankspaces
            if (i + j < n - 1)
            {
                printf(" ");
            }
            // Print the hashes
            else
            {
                printf("#");
            }
        }

        //Move to next row
        printf("\n");

    }
}