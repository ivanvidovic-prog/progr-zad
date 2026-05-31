#define _CRT_SECURE_NO_WARNINGS
#include "zajednicko.h"
#include "servis.h"
#include <stdio.h>

// KONCEPT 8: Primjena stvarnog definiranja globalnih varijabli u .c datoteci
int ukupnoServisa = 0;
float ukupniPrihod = 0.0f;

// Pomoćna funkcija za sinkronizaciju statističkih brojača izravno iz datoteke
void AzurirajGlobalneVarijable(void)
{
    // KONCEPT 19: Datoteke, provjera pokazivača i zatvaranje datoteke
    FILE* datoteka = fopen(DATOTEKA_SERVISA, "rb");
    if (datoteka == NULL)
    {
        ukupnoServisa = 0;
        ukupniPrihod = 0.0f;
        return;
    }

    RadniNalog privremeni;
    ukupnoServisa = 0;
    ukupniPrihod = 0.0f;

    while (fread(&privremeni, sizeof(RadniNalog), 1, datoteka) == 1)
    {
        ukupnoServisa++;
        ukupniPrihod += privremeni.cijenaServisa;
    }

    fclose(datoteka);
}