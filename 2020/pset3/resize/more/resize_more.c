// Resizes a BMP file for 0 < float <= 100

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "bmp.h"

// *******************Prototypes********************

// ********************Declarations********************
#define num_args 4
#define tol 1E-7

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

    // Get first argument as a double
    double f;
    sscanf(argv[1], "%lf", &f);
    printf("%f\n",f);

    // Check if double value is in the range (0,100]
    if ((f <= 0+tol) || (f > 100+tol))
    {
        printf("%s", promptBadInput);
        return 1;
    }

    // Handling of very small f
    // If the float is too small, quit
    if (f < 0.01 + tol)
    {
        printf("%s", promptBadInput);
        return 1;
    }

    // TODO: fix for f > 1=
    double n;
    if (f >= 1-tol)
    {
        n = f;
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

    // TODO: handle fractional f
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

    // TODO: branch here based on if f < 1.0

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
            // TODO: update this to read if f < 0 (e.g. every 1/f bit, f < bi.biWidth)
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile n times (the resize amount)
            // TODO: handle writing if f < 1
            for (int k = 0; k < n; k++)
            {
                currentLine[pixelCounter].rgbtRed = triple.rgbtRed;
                currentLine[pixelCounter].rgbtGreen = triple.rgbtGreen;
                currentLine[pixelCounter].rgbtBlue = triple.rgbtBlue;
                pixelCounter++;
            }

        }

        // write RGBTRIPLE array to file, then padding
        // TODO: update this to handle f < 1.0
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
