// Resizes a BMP file for 1 <= n <= 100

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "bmp.h"

// *******************Prototypes********************

// ********************Declarations********************
#define num_args 4

int main(int argc, char *argv[])
{
    char *promptBadInput = "Usage: ./resize n infile outfile\n";

    // ensure proper usage
    if (argc != num_args)
    {
        fprintf(stderr, "%s\n", promptBadInput);
        printf("%s\n", promptBadInput);
        return 1;
    }

    // Check if first arg is all digits.
    int arglen = strlen(argv[1]);
    for (int i = 0; i < arglen; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("%s", promptBadInput);
            return 1;
        }
    }

    // Check if first arg is positive integer <= 100
    int n = atoi(argv[1]);

    if ((n < 1) || (n > 100))
    {
        printf("%s", promptBadInput);
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // update biWidth and biHeight for output header
    // biWidth: width of image, in pixels (excluding padding)
    // biHeight: height of image, in pixels
    // therefore, outfile's biWidth and biHeight = infile's biWidth * n, and biHieght * n, respectively.
    BITMAPINFOHEADER bi_out = bi;
    BITMAPFILEHEADER bf_out = bf;

    bi_out.biWidth = n * bi.biWidth;
    bi_out.biHeight = n * bi.biHeight;
    int padding_out = (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi_out.biSizeImage = ((sizeof(RGBTRIPLE) * bi_out.biWidth) + padding_out) * abs(bi_out.biHeight);

    bf_out.bfSize = bi_out.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;


    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        RGBTRIPLE currentLine[bi_out.biWidth];
        int pixelCounter = 0;

        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile n times (the resize amount)
            for (int k = 0; k < n; k++)
            {
                currentLine[pixelCounter].rgbtRed = triple.rgbtRed;
                currentLine[pixelCounter].rgbtGreen = triple.rgbtGreen;
                currentLine[pixelCounter].rgbtBlue = triple.rgbtBlue;
                pixelCounter++;
            }

        }

        // write RGBTRIPLE array to file, then padding
        for (int j = 0; j < n; j++)
        {
            pixelCounter = 0;
            for (int k = 0; k < bi_out.biWidth; k++)
            {

                fwrite(&currentLine[pixelCounter], sizeof(RGBTRIPLE), 1, outptr);
                pixelCounter++;
            }

            for (int k = 0; k < padding_out; k++)
            {
                fputc(0x00, outptr);
            }
        }


        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
