#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // open input file
    FILE *raw_file = fopen(argv[1], "r");
    if (raw_file == NULL)
    {
        printf("Could not open %s.\n", argv[1]);
        return 1;
    }

    // buffer for holding blocks of data
    BYTE buffer[512];

    // variable for storing the image number
    int image_num = 0;

    // variable for storing the output file
    FILE *img_file = NULL;

    // read 512 bytes at a time
    while (fread(buffer, 1, 512, raw_file) == 512)
    {
        // check if the block is the start of a new JPEG
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // close previous image file if one is open
            if (img_file != NULL)
            {
                fclose(img_file);
            }

            // create a new image file
            char filename[8];
            sprintf(filename, "%03i.jpg", image_num);
            img_file = fopen(filename, "w");
            image_num++;
        }

        // write buffer to image file if one is open
        if (img_file != NULL)
        {
            fwrite(buffer, 1, 512, img_file);
        }
    }

    // close any remaining files
    if (img_file != NULL)
    {
        fclose(img_file);
    }
    fclose(raw_file);

    return 0;
}