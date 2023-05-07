#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }
    // Remember filenames
    char *infile = argv[1];

    // Open input file
    FILE *raw_file = fopen(infile, "r");
    if (raw_file == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    //create a new type to store a byte of data
    typedef uint8_t BYTE;
    BYTE *buffer = (BYTE*) malloc(BLOCK_SIZE * sizeof(BYTE));
    int counter = 0;
    char *outjpg = (char*) malloc(sizeof(char) * 8);
    if (outjpg == NULL)
    {
        return 2;
    }

    FILE *jpg_file = NULL;

    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // first jpg file
            if (counter == 0)
            {
                sprintf(outjpg, "%03i.jpg", counter);
                outjpg[7] = '\0';
                jpg_file = fopen(outjpg, "w");
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, jpg_file);
                counter++;
            }
            //another jpg files
            if (counter > 0)
            {
                fclose(jpg_file);
                sprintf(outjpg, "%03i.jpg", counter);
                outjpg[7] = '\0';
                jpg_file = fopen(outjpg, "w");
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, jpg_file);
                counter++;
            }
        }
        //we have already found a JPEG file and we are currently writing to it
        else if (jpg_file != NULL)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, jpg_file);
        }
    }

    fclose(raw_file);
    fclose(jpg_file);
    free(outjpg);
    free(buffer);

    return 0;
}