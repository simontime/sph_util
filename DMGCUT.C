#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    char buffer[1000];
    FILE *in, *out;
    long int start, length;

    if (argc < 4)
    {
        printf("\nUsage : %s <input file> <output file> <start> [<length>]", argv[0]);
        return 1;
    }

    if ((in = fopen(argv[1], "rb")) == NULL)
    {
        printf("\nInput file not found!!\n");
        return 1;
    }

    start  = atol(argv[3]);
    length = (argc != 4) ? atol(argv[4]) : 0;

    if ((out = fopen(argv[2], "wb")) == NULL)
    {
        printf("\nOutput file open error!!\n");
        return 1;
    }

    fseek(in, 0, SEEK_END);

    length = ftell(in) - start;

    /* BUG! On 16-bit systems 'int' is a 16-bit data type, and 3 was declared as an int.
     * This means only the bottom 16 bits of the length get written.
     * In practise this wasn't an issue;
     * Game.com titles that used long sections of speech split them up into multiple separate samples.
     * Fix: 3 -> 3L. */
    fputc((length + 3) >> 16, out);
    fputc((length + 3) >> 8,  out);
    fputc((length + 3) >> 0,  out);

    fseek(in, start, SEEK_SET);

    /* R/W 1000 bytes at a time. */
    while (length >= 1000)
    {
        fread(buffer, 1, 1000, in);
        fwrite(buffer, 1, 1000, out);

        length -= 1000;
    }

    /* R/W remainder. */
    if (length)
    {
        fread(buffer, 1, length, in);
        fwrite(buffer, 1, length, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}