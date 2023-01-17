#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Check for command line args
    if (argc != 2)
    {
        printf("Usage: ./read infile\n");
        return 1;
    }

    // Create buffer to read into
    char buffer[7];

    // Create array to store pointers to plate numbers
    char **plates;

    FILE *infile = fopen(argv[1], "r");

    int idx = 0;
    int capacity = 8;

    plates = (char **)malloc(capacity * sizeof(char *));

    while (fread(buffer, 1, 7, infile) == 7)
    {
        // Replace '\n' with '\0'
        buffer[6] = '\0';

        // Allocate memory for plate number
        plates[idx] = (char *)malloc(7 * sizeof(char));

        // Copy plate number from buffer to dynamically allocated memory
        strcpy(plates[idx], buffer);
        idx++;

        // reallocate if array is full
        if (idx == capacity)
        {
            capacity *= 2;
            plates = (char **) realloc(plates, capacity * sizeof(char *));
        }
    }

    for (int i = 0; i < idx; i++)
    {
        printf("%s\n", plates[i]);
    }

    for (int i = 0; i < idx; i++)
    {
        free(plates[i]);
    }
    free(plates);
    fclose(infile);
}