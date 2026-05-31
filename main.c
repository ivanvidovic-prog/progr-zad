#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// KONCEPT 7: Organizacija izvornog koda kroz modularna zaglavlja
#include "izbornik.h"
#include "servis.h"
#include "pomocno.h"
#include "zajednicko.h"

int main(void)
{
    // KONCEPT 4: Upotreba deklariranog enum tipa za varijablu glavnog izbornika
    GlavniIzbornikOpcija izborKorisnika;

    // Inicijalno čitanje stanja baze s diska radi sinkronizacije globalnih brojača
    AzurirajGlobalneVarijable();

    do
    {
        PrikaziGlavniIzbornik();
        printf("Odaberite zeljenu opciju izbornika (0-7): ");

        // Siguran prihvat broja i pretvaranje u enum tip podatka
        izborKorisnika = (GlavniIzbornikOpcija)DohvatiValidiraniCijeliBroj(0, 7);

        // KONCEPT 10: Izbornik / podizbornici kroz jasnu switch-case strukturu
        switch (izborKorisnika)
        {
        case IZBORNIK_KREIRAJ:
            KreirajRadniNalog();
            break;

        case IZBORNIK_CITAJ:
            IspisiSveRadneNaloge();
            break;

        case IZBORNIK_AZURIRAJ:
            AzurirajRadniNalog();
            break;

        case IZBORNIK_BRISI:
            ObrisiRadniNalog();
            break;

        case IZBORNIK_STATISTIKA:
            UpravljajIzbornikomStatistike();
            break;

        case IZBORNIK_PRETRAZI:
            UpravljajIzbornikomPretrazivanja();
            break;

        case IZBORNIK_SORTIRAJ:
            UpravljajIzbornikomSortiranja();
            break;

        case IZBORNIK_IZLAZ:
            printf("\nHvala Vam na koristenju sustava za evidenciju servisa. Izlaz iz programa...\n");
            break;

        default:
            printf("\n[UPOZORENJE] Nepoznata i neobradena opcija.\n");
            break;
        }

    } while (izborKorisnika != IZBORNIK_IZLAZ);

    return 0;
}