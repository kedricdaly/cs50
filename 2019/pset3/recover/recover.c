#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

// Prototypes

// Declarations
#define num_args 2
const int BUFFERSIZE = 512;
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Input checks
    //char *promptBadInput = "Usage: ./recover image\n";

    // Check if correct number of arguments
    if (argc != num_args)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    // Check if we can open the file. If not, return 2 and quit
    char *filename = argv[1];
    FILE *image = fopen(filename, "r");

    if (image == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", filename);
        return 2;
    }
    //printf("Opened %s\n", filename);

    BYTE buffer[BUFFERSIZE / sizeof(BYTE)]; // 512 bytes / 1 bytes/BYTE = 528 BYTE
    int *blockSize;
    blockSize = (int *) malloc(sizeof(int) * 1);
    int jpgCount = 0;
    bool isFileOpen = false;
    // While there is another block (size of block is correct [512]
    //   and fread is successful, get the next block and put it in buffer
    do
    {
        *blockSize = fread(buffer, 1, BUFFERSIZE, image);
        BYTE firstByte = buffer[0];
        BYTE secondByte = buffer[1];
        BYTE thirdByte = buffer[2];
        BYTE fourthByte = buffer[3];
        bool lastBlock = (*blockSize != BUFFERSIZE);
        //printf("%#x%x%x%x\n", firstByte, secondByte, thirdByte, fourthByte);
        // order of bytes: 0xe[0-f]ffd8ff
        // Check the buffer for JPG start bytes
        // Check if first byte is in 0xe[0-f]
        // Check if next three bytes are 0xff 0xd8 0xff
        bool foundMagicNumbers = ((firstByte == 0xff) && (secondByte == 0xd8) && (thirdByte == 0xff)
                                  && ((fourthByte >= 0xe0) && (fourthByte <= 0xef)));
        if (foundMagicNumbers)
        {
            // If find, close any open files, then open a new file
            // we found a .JPG
            jpgCount++;
            char outFile[8];
            sprintf(outFile, "%03i.jpg", jpgCount - 1);
            FILE *outImage = fopen(outFile, "w");
            fwrite(buffer, 1, BUFFERSIZE, outImage);
            isFileOpen = true;
            fclose(outImage);
        }
        else
        {
            // write block to file, if we have a new jpg file open
            // (and loop to next block)
            if (isFileOpen)
            {
                char outFile[8];
                sprintf(outFile, "%03i.jpg", jpgCount - 1);
                FILE *outImage = fopen(outFile, "a");
                if (lastBlock)
                {
                    fwrite(buffer, 1, *blockSize, outImage);
                }
                else
                {
                    fwrite(buffer, 1, BUFFERSIZE, outImage);
                }
                fclose(outImage);
            }
        }

    }
    while (*blockSize == BUFFERSIZE);
    free(blockSize);
    blockSize = NULL;

    //printf("Found %i jpgs.\n", jpgCount);

    int filestatus = fclose(image);
    if (filestatus == 0)
    {
        //printf("Closed %s\n", filename);
    }
}
