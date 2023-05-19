#include <cs50.h>
#include <stdio.h>


int main(void)
{
    //Prompt user for name
    string name = get_string("What is your name?\n");

    // say hello to user
    printf("Hello, %s, nice to meet you!\n", name);
}