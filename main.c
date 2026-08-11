#include <stdio.h>

int main()
{
    FILE *file;
    char ch;
    int frequency[256] = {0};
    int i;

    file = fopen("sample.txt", "r");

    if (file == NULL)
    {
        printf("Could not open the file.\n");
        return 1;
    }

    /* Read the file and count characters */
    while ((ch = fgetc(file)) != EOF)
    {
        frequency[(unsigned char)ch]++;
    }

    fclose(file);

    /* Display character frequencies */
    printf("Character Frequencies:\n\n");

    for (i = 0; i < 256; i++)
    {
        if (frequency[i] > 0)
        {
            if (i == ' ')
                printf("[space] : %d\n", frequency[i]);
            else if (i == '\n')
                printf("[newline] : %d\n", frequency[i]);
            else
                printf("%c : %d\n", i, frequency[i]);
        }
    }

    return 0;
}