#include "servis.h"

float izracunUkupnogTroska(
    const RadniNalog* nalog
)
{
    if(nalog == NULL)
    {
        return 0.0f;
    }

    float suma = nalog->cijenaRada;

    for(int i = 0; i < nalog->brojDijelova; i++)
    {
        suma += nalog->dijelovi[i].cijena;
    }

    suma += izracunPDV(suma);

    return suma;
}

void dodajNalog(void)
{
    FILE* fp = fopen(DATOTEKA, "ab");

    if(fp == NULL)
    {
        perror("Greska");
        return;
    }

    RadniNalog novi;

    printf("ID: ");
    scanf("%d", &novi.id);

    CLEAR_BUFFER();

    printf("Vlasnik: ");
    fgets(novi.vlasnik, MAX_NAZIV, stdin);

    novi.vlasnik[
        strcspn(novi.vlasnik, "\n")
    ] = 0;

    printf("Model bicikla: ");
    fgets(novi.modelBicikla,
          MAX_NAZIV,
          stdin);

    novi.modelBicikla[
        strcspn(novi.modelBicikla, "\n")
    ] = 0;

    printf("Opis kvara: ");
    fgets(novi.opisKvara,
          MAX_OPIS,
          stdin);

    novi.opisKvara[
        strcspn(novi.opisKvara, "\n")
    ] = 0;

    printf("Broj dijelova: ");
    scanf("%d", &novi.brojDijelova);

    CLEAR_BUFFER();

    novi.dijelovi = (Dio*)calloc(
        novi.brojDijelova,
        sizeof(Dio)
    );

    if(novi.dijelovi == NULL)
    {
        fclose(fp);
        return;
    }

    for(int i = 0;
        i < novi.brojDijelova;
        i++)
    {
        printf("Naziv dijela: ");

        fgets(
            novi.dijelovi[i].naziv,
            MAX_NAZIV,
            stdin
        );

        novi.dijelovi[i].naziv[
            strcspn(
                novi.dijelovi[i].naziv,
                "\n"
            )
        ] = 0;

        printf("Cijena dijela: ");

        scanf(
            "%f",
            &novi.dijelovi[i].cijena
        );

        CLEAR_BUFFER();
    }

    printf("Cijena rada: ");
    scanf("%f", &novi.cijenaRada);

    novi.status = ZAPRIMLJEN;

    novi.ukupanTrosak =
        izracunUkupnogTroska(&novi);

    fwrite(
        &novi,
        sizeof(RadniNalog),
        1,
        fp
    );

    fwrite(
        novi.dijelovi,
        sizeof(Dio),
        novi.brojDijelova,
        fp
    );

    free(novi.dijelovi);
    novi.dijelovi = NULL;

    fclose(fp);

    printf("Nalog dodan!\n");
}

RadniNalog* ucitajNaloge(void)
{
    FILE* fp = fopen(DATOTEKA, "rb");

    if(fp == NULL)
    {
        return NULL;
    }

    rewind(fp);

    int count = 0;

    while(1)
    {
        RadniNalog temp;

        size_t procitano =
            fread(
                &temp,
                sizeof(RadniNalog),
                1,
                fp
            );

        if(procitano != 1)
        {
            break;
        }

        fseek(
            fp,
            sizeof(Dio) *
            temp.brojDijelova,
            SEEK_CUR
        );

        count++;
    }

    rewind(fp);

    brojNaloga = count;

    RadniNalog* polje =
        (RadniNalog*)malloc(
            count *
            sizeof(RadniNalog)
        );

    if(polje == NULL)
    {
        fclose(fp);
        return NULL;
    }

    for(int i = 0; i < count; i++)
    {
        fread(
            &polje[i],
            sizeof(RadniNalog),
            1,
            fp
        );

        polje[i].dijelovi =
            (Dio*)malloc(
                polje[i].brojDijelova *
                sizeof(Dio)
            );

        fread(
            polje[i].dijelovi,
            sizeof(Dio),
            polje[i].brojDijelova,
            fp
        );
    }

    fclose(fp);

    return polje;
}

void ispisiNaloge(void)
{
    RadniNalog* polje =
        ucitajNaloge();

    if(polje == NULL)
    {
        printf("Nema podataka!\n");
        return;
    }

    for(int i = 0;
        i < brojNaloga;
        i++)
    {
        printf("\n====================\n");

        printf("ID: %d\n",
               polje[i].id);

        printf("Vlasnik: %s\n",
               polje[i].vlasnik);

        printf("Model: %s\n",
               polje[i].modelBicikla);

        printf("Opis: %s\n",
               polje[i].opisKvara);

        printf("Ukupno: %.2f\n",
               polje[i].ukupanTrosak);

        for(int j = 0;
            j < polje[i].brojDijelova;
            j++)
        {
            printf(
                "- %s %.2f EUR\n",
                polje[i].dijelovi[j].naziv,
                polje[i].dijelovi[j].cijena
            );
        }
    }

    oslobodiMemoriju(polje);
}

void spremiNaloge(
    RadniNalog* polje
)
{
    FILE* fp = fopen(DATOTEKA, "wb");

    if(fp == NULL)
    {
        return;
    }

    for(int i = 0;
        i < brojNaloga;
        i++)
    {
        fwrite(
            &polje[i],
            sizeof(RadniNalog),
            1,
            fp
        );

        fwrite(
            polje[i].dijelovi,
            sizeof(Dio),
            polje[i].brojDijelova,
            fp
        );
    }

    fclose(fp);
}

void oslobodiMemoriju(
    RadniNalog* polje
)
{
    if(polje == NULL)
    {
        return;
    }

    for(int i = 0;
        i < brojNaloga;
        i++)
    {
        free(polje[i].dijelovi);
        polje[i].dijelovi = NULL;
    }

    free(polje);
    polje = NULL;
}
