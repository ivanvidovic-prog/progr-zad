#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "pomocno.h"

// KONCEPT 5: Imenovanje identifikatora - camelCase za lokalne varijable, PascalCase za funkcije
// KONCEPT 6: Primjena ključne riječi static za globalne varijable unutar datoteke (lokalni brojač čišćenja)
static int brojacCiscenjaBuffera = 0;

void OcistiUnosniSpremnik(void)
{
    int znak;
    while ((znak = getchar()) != '\n' && znak != EOF);
    brojacCiscenjaBuffera++;
}

void SigurnoUcitajString(char* CiljniSpremnik, const unsigned int MaksDuljina)
{
    // KONCEPT 14: Zaštita parametara unutar funkcije od NULL pokazivača
    if (CiljniSpremnik == NULL || MaksDuljina == 0)
    {
        return;
    }

    fgets(CiljniSpremnik, MaksDuljina, stdin);
    CiljniSpremnik[strcspn(CiljniSpremnik, "\n")] = '\0';
}

int DohvatiValidiraniCijeliBroj(const int MinVrijednost, const int MaxVrijednost)
{
    int unesenaVrijednost;
    int rezultatSkeniranja;

    while (1)
    {
        rezultatSkeniranja = scanf("%d", &unesenaVrijednost);
        OcistiUnosniSpremnik();

        if (rezultatSkeniranja == 1 && unesenaVrijednost >= MinVrijednost && unesenaVrijednost <= MaxVrijednost)
        {
            return unesenaVrijednost;
        }
        printf("Neispravan unos! Molimo unesite cijeli broj izmedu %d i %d: ", MinVrijednost, MaxVrijednost);
    }
}

float DohvatiValidiraniRealniBroj(const float MinVrijednost, const float MaxVrijednost)
{
    float unesenaVrijednost;
    int rezultatSkeniranja;

    while (1)
    {
        rezultatSkeniranja = scanf("%f", &unesenaVrijednost);
        OcistiUnosniSpremnik();

        if (rezultatSkeniranja == 1 && unesenaVrijednost >= MinVrijednost && unesenaVrijednost <= MaxVrijednost)
        {
            return unesenaVrijednost;
        }
        printf("Neispravan unos! Molimo unesite realni broj izmedu %.2f i %.2f: ", MinVrijednost, MaxVrijednost);
    }
}