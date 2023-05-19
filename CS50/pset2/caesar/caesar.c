#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int main(int argc, string argv[])
{
    //Check that there is only one argument in command-line
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //Check if the input is a digit
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Useage ./caesar key\n");
            return 1;
        }
    }

    //Convert key to integer
    int k = atoi(argv[1]);

    //Get the plaintext from the user
    string plaintext = get_string("Plaintext: ");
    printf("Ciphertext: ");

    //Get the ciphertext
    for (int j = 0; j < strlen(plaintext); j++)
    {
        if (isupper(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 65 + k) % 26 + 65);
        }

        else if (islower(plaintext[j]))
        {
            printf("%c", (plaintext[j] - 97 + k) % 26 + 97);
        }

        else
        {
            printf("%c", plaintext[j]);
        }
    }
    printf("\n");
}
