#include <stdio.h>

int main()
{
    int choice;

    printf("==============================\n");
    printf("         FILE ZIPPER\n");
    printf("==============================\n");
    printf("1. Compress File\n");
    printf("2. Decompress File\n");
    printf("3. Exit\n");
    printf("==============================\n");

    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
            printf("Compress File selected.\n");
            break;

        case 2:
            printf("Decompress File selected.\n");
            break;

        case 3:
            printf("Exiting File Zipper...\n");
            break;

        default:
            printf("Invalid choice.\n");
    }

    return 0;
}