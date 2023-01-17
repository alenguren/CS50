#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Prompt user for X
    int x = get_int("x: ");

    // Prompt user for Y
    int y = get_int("y: ");

    // Divide x by y

    float z = (float) x / (float) y;

    // Perform adition
    printf("%.50f\n", z);
}