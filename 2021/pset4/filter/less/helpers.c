#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <cs50.h>

// prototypes
bool isValidPixel(int x, int y, int offset_x, int offset_y, int height, int width);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE original_bit;
    RGBTRIPLE gscale;
    int grey_value;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            original_bit = image[i][j];
            grey_value = round(((float) original_bit.rgbtBlue + original_bit.rgbtGreen + original_bit.rgbtRed) / 3);
            gscale.rgbtBlue = grey_value;
            gscale.rgbtGreen = grey_value;
            gscale.rgbtRed = grey_value;
            image[i][j] = gscale;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE originalRed;
    BYTE originalBlue;
    BYTE originalGreen;
    int sepiaRed;
    int sepiaGreen;
    int sepiaBlue;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //original_bit = image[i][j];
            originalRed = image[i][j].rgbtRed;
            originalGreen = image[i][j].rgbtGreen;
            originalBlue = image[i][j].rgbtBlue;
            sepiaRed = round((0.393 * originalRed) + (0.769 * originalGreen) + (0.189 * originalBlue));
            sepiaGreen = round((0.349 * originalRed) + (0.686 * originalGreen) + (0.168 * originalBlue));
            sepiaBlue = round((0.272 * originalRed) + (0.534 * originalGreen) + (0.131 * originalBlue));
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE buffer[width];
    int offset;
    for (int i = 0; i < height; i++)
    {
        // copy line into a buffer
        for (int j = 0; j < width; j++)
        {
            buffer[j] = image[i][j];
        }

        // reverse order of pixels from buffer
        // index for buffer is [0, width - 1]
        for (int j = width; j > 0; j--)
        {
            offset = width - j;
            image[i][j - 1] = buffer[offset];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy_image[height][width];

    // create copy of image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy_image[i][j] = image[i][j];
        }
    }

    // for every pixel in the image, we need to get the blur grid rgb values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int nPixels = 0;
            float redSum = 0;
            float greenSum = 0;
            float blueSum = 0;

            // for each pixel, need to find valid surrounding pixels and take
            // the average rgb values for each color.
            for (int offset_x = -1; offset_x < 2; offset_x++)
            {
                for (int offset_y = -1; offset_y < 2; offset_y++)
                {
                    if (!isValidPixel(j, i, offset_x, offset_y, height, width))
                    {
                        continue;
                    }
                    redSum += copy_image[i + offset_y][j + offset_x].rgbtRed;
                    greenSum += copy_image[i + offset_y][j + offset_x].rgbtGreen;
                    blueSum += copy_image[i + offset_y][j + offset_x].rgbtBlue;
                    nPixels++;
                }
            }

            image[i][j].rgbtRed = round(redSum / nPixels);
            image[i][j].rgbtGreen = round(greenSum / nPixels);
            image[i][j].rgbtBlue = round(blueSum / nPixels);

        }
    }
    return;
}


// functions

/*
 * isValidPixel: check if a pixel within a blur grid is valid based on
 *    coordinates of pixel and offset.
 *    This includes the 'original pixel' (offset_x and offset_y = 0)
 * INPUTS:
 *   x (req, int): x coordinate of original pixel
 *   y (req, int): y coordinate of original pixel
 *   offset_x (req, int): x coordinate offset from original pixel
 *   offset_y (req, int): y coordinate offset from original pixel
 *   height (req, int): max height of image. Indicates upper bound limit for y coordinate
 *   width (req, int): max width of image. Indicates upper bound limit for x coordinate
 * OUTPUTS:
 *   boolean - true: the neighboring pixel is within the original image
 *           - false: the neighboring pixel is not within the original image
 */

bool isValidPixel(int x, int y, int offset_x, int offset_y, int height, int width)
{
    // index for width is [0, width-1]
    if ((x + offset_x < 0) || (x + offset_x > width - 1))
    {
        return false;
    }

    // index for height is [0, height-1]
    if ((y + offset_y < 0) || (y + offset_y > height - 1))
    {
        return false;
    }
    return true;
}
