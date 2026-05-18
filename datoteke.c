#include "servis.h"

void backupDatoteke(void)
{
    FILE* source =
        fopen(DATOTEKA, "rb");

    if(source == NULL)
    {
        perror("Source");
        return;
    }

    FILE* destination =
        fopen(BACKUP, "wb");

    if(destination == NULL)
    {
        perror("Destination");

        fclose(source);

        return;
    }

    char znak;

    while((znak = fgetc(source)) != EOF)
    {
        fputc(znak, destination);
    }

    if(feof(source))
    {
        printf("Backup uspjesan!\n");
    }

    fclose(source);
    fclose(destination);
}
