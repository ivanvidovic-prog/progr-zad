#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "izbornik.h"

// KONCEPT 10: Izbornik / podizbornici kroz tekstualno sučelje
void PrikaziGlavniIzbornik(void)
{
    printf("\n==================================================\n");
    printf("        EVIDENCIJA SERVISA BICIKALA - GLAVNI\n");
    printf("==================================================\n");
    printf("1. Dodaj novi radni nalog (Create)\n");
    printf("2. Prikazi sve radne naloge (Read)\n");
    printf("3. Uredi postojeci nalog (Update)\n");
    printf("4. Obrisi radni nalog (Delete)\n");
    printf("5. Podizbornik: Statistika\n");
    printf("6. Podizbornik: Pretrazivanje\n");
    printf("7. Podizbornik: Sortiranje naloga\n");
    printf("0. Izlaz iz programa\n");
    printf("==================================================\n");
}

void PrikaziIzbornikStatistike(void)
{
    printf("\n--------------------------------------------------\n");
    printf(" PODIZBORNIK: STATISTIKA PODATAKA\n");
    printf("--------------------------------------------------\n");
    printf("1. Ukupan broj registriranih servisa\n");
    printf("2. Ukupni financijski prihod (EUR)\n");
    printf("3. Prosjecna cijena servisa\n");
    printf("0. Povratak u glavni izbornik\n");
    printf("--------------------------------------------------\n");
}

void PrikaziIzbornikPretrazivanja(void)
{
    printf("\n--------------------------------------------------\n");
    printf(" PODIZBORNIK: PRETRAZIVANJE RADNIH NALOGA\n");
    printf("--------------------------------------------------\n");
    printf("1. Pretrazi prema jedinstvenom ID-u (Linearno)\n");
    printf("2. Pretrazi prema imenu klijenta (Linearno)\n");
    printf("3. Brzo pretrazivanje prema ID-u (Ugradeni bsearch)\n");
    printf("0. Povratak u glavni izbornik\n");
    printf("--------------------------------------------------\n");
}

void PrikaziIzbornikSortiranja(void)
{
    printf("\n--------------------------------------------------\n");
    printf(" PODIZBORNIK: SORTIRANJE BAZA PODATAKA\n");
    printf("--------------------------------------------------\n");
    printf("1. Sortiraj naloge po ID-u uzlazno (Ugradeni qsort)\n");
    printf("2. Sortiraj naloge po cijeni uzlazno (Rekurzivni QuickSort)\n");
    printf("3. Sortiraj naloge po imenu klijenta abecedno (Ugradeni qsort)\n");
    printf("0. Povratak u glavni izbornik\n");
    printf("--------------------------------------------------\n");
}