#include <math.h>
#include <string.h>

#include "math.h"
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //Look for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Change the pixel colour to an average of the 3 colour
            int avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //Look for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            //Change the position from the last row the first row
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //Create a copy of the image
    RGBTRIPLE copy[height][width];
    memcpy(copy, image, sizeof(RGBTRIPLE) * height * width);

    //Look for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;
            int count = 0;

            //Get the surrounding pixels
            for (int k = -1; k <= 1; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    //Check if the surrounding pixels are valid
                    if (i + k >= 0 && i + k < height && j + l >= 0 && j + l < width)
                    {
                        sumRed += copy[i + k][j + l].rgbtRed;
                        sumGreen += copy[i + k][j + l].rgbtGreen;
                        sumBlue += copy[i + k][j + l].rgbtBlue;
                        count++;
                    }
                }
            }
            //Get image value
            image[i][j].rgbtRed = round((double) sumRed / count);
            image[i][j].rgbtGreen = round((double) sumGreen / count);
            image[i][j].rgbtBlue = round((double) sumBlue / count);
        }
    }
    return;
}

//Edges efects on image
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //Create a copy of the image
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    // Create two kernels for detecting horizontal and vertical edges
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    //Stablih 0 to RGB X and Y values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int redX = 0;
            int greenX = 0;
            int blueX = 0;
            int redY = 0;
            int greenY = 0;
            int blueY = 0;

            //look to the surroundings pixels
            for (int x = 0; x < 3; x++)
            {
                for (int y = 0; y < 3; y++)
                {
                    // Check if the current position is valid
                    if (i - 1 + x < 0 || i - 1 + x > height - 1 || j - 1 + y < 0 || j - 1 + y > width - 1)
                    {
                        continue;
                    }

                    //Apply Gx to each colour
                    redX = redX + (image[i - 1 + x][j - 1 + y].rgbtRed * Gx[x][y]);
                    greenX = greenX + (image[i - 1 + x][j - 1 + y].rgbtGreen * Gx[x][y]);
                    blueX = blueX + (image[i - 1 + x][j - 1 + y].rgbtBlue * Gx[x][y]);

                    //Apply Gy to each colour
                    redY = redY + (image[i - 1 + x][j - 1 + y].rgbtRed * Gy[x][y]);
                    greenY = greenY + (image[i - 1 + x][j - 1 + y].rgbtGreen * Gy[x][y]);
                    blueY = blueY + (image[i - 1 + x][j - 1 + y].rgbtBlue * Gy[x][y]);
                }
            }
             // Calculate the final value for each colour
            int red = round(sqrt((redX * redX) + (redY * redY)));
            int green = round(sqrt((greenX * greenX) + (greenY * greenY)));
            int blue = round(sqrt((blueX * blueX) + (blueY * blueY)));

            // update RGB values if theses exceeds 255
            if (red > 255)
            {
                red = 255;
            }
            if (green > 255)
            {
                green = 255;
            }
            if (blue > 255)
            {
                blue = 255;
            }

        // Assign the new values to the temp image
        temp[i][j].rgbtRed = red;
        temp[i][j].rgbtGreen = green;
        temp[i][j].rgbtBlue = blue;
        }
    }

    //Copy new pixels into original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}