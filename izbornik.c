#include "servis.h"

static const char* naslov =
"=== EVIDENCIJA SERVISA BICIKALA ===";

typedef enum Izbornik
{
    DODAJ = 1,
    ISPISI,
    AZURIRAJ,
    OBRISI,
    PRETRAZI,
    SORTIRAJ,
    BACKUP_FILE,
    IZLAZ

} Izbornik;

void glavniIzbornik(void)
{
    int izbor = 0;

    do
    {
        printf("\n%s\n", naslov);

        printf("1. Dodaj radni nalog\n");
        printf("2. Ispisi naloge\n");
        printf("3. Azuriraj nalog\n");
        printf("4. Obrisi nalog\n");
        printf("5. Pretrazi naloge\n");
        printf("6. Sortiraj naloge\n");
        printf("7. Backup datoteke\n");
        printf("8. Izlaz\n");

        printf("Odabir: ");
        scanf("%d", &izbor);

        CLEAR_BUFFER();

        switch(izbor)
        {
            case DODAJ:
                dodajNalog();
                break;

            case ISPISI:
                ispisiNaloge();
                break;

            case AZURIRAJ:
                azurirajNalog();
                break;

            case OBRISI:
                obrisiNalog();
                break;

            case PRETRAZI:
                pretraziNaloge();
                break;

            case SORTIRAJ:
                sortirajNaloge();
                break;

            case BACKUP_FILE:
                backupDatoteke();
                break;

            case IZLAZ:
                printf("Program zavrsava.\n");
                break;

            default:
                printf("Neispravan odabir!\n");
        }

    } while(izbor != IZLAZ);
}
