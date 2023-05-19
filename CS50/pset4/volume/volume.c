// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

//Function to copy header from input file to output file
void copy_header(FILE *input, FILE *output)
{
    // Allocate memory for header data
    uint8_t header[HEADER_SIZE];

    // Read header data from input file
    fread(header, sizeof(uint8_t), HEADER_SIZE, input);

    // Write header data to output file
    fwrite(header, sizeof(uint8_t), HEADER_SIZE, output);
}

//Function to copy header from input file to output file
void update_samples(FILE *input, FILE *output, float factor)
{
    // Sample data (assuming 16-bit samples)
    int16_t sample;

    // Read samples from input file and update them
    while (fread(&sample, sizeof(int16_t), 1, input) == 1)
    {
        // Scale the sample
        sample *= factor;

        // Write the updated sample to the output file
        fwrite(&sample, sizeof(int16_t), 1, output);
    }
}

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    //Copy header from input file to output file
    copy_header(input, output);

    //Read samples from input file and write updated data to output file
    update_samples(input, output, factor);

    // Close files
    fclose(input);
    fclose(output);
}
