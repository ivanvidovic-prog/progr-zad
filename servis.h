#ifndef SERVIS_H
#define SERVIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define MAX_NAZIV 50
#define MAX_OPIS 200
#define DATOTEKA "servis.bin"
#define BACKUP "backup.bin"

#define CLEAR_BUFFER() while(getchar() != '\n')

static inline float izracunPDV(float iznos)
{
    return iznos * 0.25f;
}

typedef enum StatusServisa
{
    ZAPRIMLJEN = 1,
    U_SERVISU,
    ZAVRSEN

} StatusServisa;

typedef struct Dio
{
    char naziv[MAX_NAZIV];
    float cijena;

} Dio;

typedef struct RadniNalog
{
    int id;

    char vlasnik[MAX_NAZIV];
    char modelBicikla[MAX_NAZIV];
    char opisKvara[MAX_OPIS];

    Dio* dijelovi;
    int brojDijelova;

    float cijenaRada;
    float ukupanTrosak;

    StatusServisa status;

} RadniNalog;

typedef struct Cvor
{
    RadniNalog podatak;
    struct Cvor* next;

} Cvor;

extern int brojNaloga;

void glavniIzbornik(void);

void dodajNalog(void);
void ispisiNaloge(void);
void azurirajNalog(void);
void obrisiNalog(void);
void pretraziNaloge(void);
void sortirajNaloge(void);
void backupDatoteke(void);

void spremiNaloge(RadniNalog* polje);
RadniNalog* ucitajNaloge(void);

float izracunUkupnogTroska(const RadniNalog* nalog);

void oslobodiMemoriju(RadniNalog* polje);

int usporediPoID(const void* a, const void* b);
int usporediPoCijeni(const void* a, const void* b);

void rekurzivniIspis(Cvor* head);

#endif
