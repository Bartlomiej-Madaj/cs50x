#include <stdio.h>
#include <stdlib.h>

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

    // Create array to store plate numbers
    char *plates[8];

    FILE *infile = fopen(argv[1], "r");

    int idx = 0;

    while (fread(buffer, 1, 7, infile) == 7)
    {
        // Replace '\n' with '\0'
        buffer[6] = '\0';
        // int *p = &buffer;

        // Save plate number in array
        char *temp = malloc(6);
        for (int i = 0; i < 7; i++)
        {
            temp[i] = buffer[i];
        }
        // printf("%p\n", &buffer);
        plates[idx] = temp;
        idx++;
        free(temp);
        // for (int i = 0; i < 8; i++)
        // {
        //     printf("%s\n", plates[i]);
        // }
    }

    for (int i = 0; i < 8; i++)
    {
        printf("%s\n", plates[i]);
    }
}
