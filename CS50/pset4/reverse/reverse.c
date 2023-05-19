#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "wav.h"

int check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    // TODO #1
    if (argc != 3)
    {
        printf("Usage: ./reverse input.wav output.wav\n");
        return 1;
    }

    // Open input file for reading
    // TODO #2
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Input is not a WAV file.\n");
        return 1;
    }

    // Read header into an array
    // TODO #3
    WAVHEADER header;
    fread(&header, sizeof(WAVHEADER), 1, input_file);

    // Use check_format to ensure WAV format
    // TODO #4
    if (!check_format(header))
    {
        printf("Error: Not a WAV file.\n");
        return 1;
    }

    // Open output file for writing
    // TODO #5
    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL)
    {
        printf("Error: Unable to open output file.\n");
        return 1;
    }

    // Write header to file
    // TODO #6
    fwrite(&header, sizeof(WAVHEADER), 1, output_file);

    // Use get_block_size to calculate size of block
    // TODO #7
    int block_size = get_block_size(header);

    // Write reversed audio to file
    // TODO #8
    fseek(input_file, block_size, SEEK_END);

    while (ftell(input_file) - block_size > sizeof(header))
    {
        fseek(input_file, -2 * block_size, SEEK_CUR);
        BYTE c[block_size];
        fread(&c, block_size, 1, input_file);
        fwrite(&c, block_size, 1, output_file);
    }


    // Close opened files
    fclose(input_file);
    fclose(output_file);
}

int check_format(WAVHEADER header)
{
    // TODO #4
    // Check if the input file is a WAV file
    if (header.chunkID[0] != 'R' || header.chunkID[1] != 'I' || header.chunkID[2] != 'F' || header.chunkID[3] != 'F' ||
        header.format[0] != 'W' || header.format[1] != 'A' || header.format[2] != 'V' || header.format[3] != 'E')
    {
        printf("Input is not a WAV file.\n");
        return 0;
    }
    return 1;
}
int get_block_size(WAVHEADER header)
{
    // TODO #7
    int block_size = header.numChannels * (header.bitsPerSample / 8);
    return block_size;
}
