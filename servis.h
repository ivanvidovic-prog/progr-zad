#ifndef SERVIS_H
#define SERVIS_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

// KONCEPT 15: Koristiti statički zauzeta polja (Makro konstante za fiksne duljine nizova)
#define MAKS_NAZIV 50
#define MAKS_OPIS 200
#define MAKS_STATICKI_LIMIT 500
#define SIGURNOSNA_KOPIJA_DATOTEKE "servisi_backup.dat"

// KONCEPT 9: Ako su funkcije jednostavne koristiti makro funkcije ili inline funkcije
#define EUR_U_KN(x) ((x) * 7.53450f)

// KONCEPT 9: Primjena inline funkcije za brzi izračun poreza (PDV 25%)
static inline float IzracunajPorez(const float Iznos)
{
    return Iznos * 0.25f;
}

// KONCEPT 4: Primjena typedef s enum tipovima & KONCEPT 11: Enum za upravljanje izbornikom
typedef enum GlavniIzbornikOpcija
{
    IZBORNIK_IZLAZ = 0,
    IZBORNIK_KREIRAJ,
    IZBORNIK_CITAJ,
    IZBORNIK_AZURIRAJ,
    IZBORNIK_BRISI,
    IZBORNIK_STATISTIKA,
    IZBORNIK_PRETRAZI,
    IZBORNIK_SORTIRAJ
} GlavniIzbornikOpcija;

typedef enum StatistikaOpcija
{
    STAT_NATRAG = 0,
    STAT_UKUPAN_BROJ,
    STAT_UKUPAN_PRIHOD,
    STAT_PROSJEK
} StatistikaOpcija;

typedef enum PretrazivanjeOpcija
{
    PRETRAGA_NATRAG = 0,
    PRETRAGA_LINEARNA_ID,
    PRETRAGA_LINEARNA_IME,
    PRETRAGA_BSEARCH_ID
} PretrazivanjeOpcija;

typedef enum SortiranjeOpcija
{
    SORT_NATRAG = 0,
    SORT_QSORT_ID,
    SORT_REKURZIVNI_CIJENA,
    SORT_QSORT_IME
} SortiranjeOpcija;

// KONCEPT 3: Odabir konkretnih složenih tipova podataka (Enum unutar strukture)
typedef enum StatusServisa
{
    STATUS_ZAPRIMLJEN = 0,
    STATUS_U_TIJEKU,
    STATUS_ZAVRSEN
} StatusServisa;

// KONCEPT 3: Odabir konkretnih složenih tipova podataka & KONCEPT 4: Typedef sa strukturama
typedef struct RadniNalog
{
    // KONCEPT 2: Odabir konkretnih primitivnih tipova podataka (int i float)
    int idServisa;
    char imeKlijenta[MAKS_NAZIV];
    char modelBicikla[MAKS_NAZIV];
    char opisKvara[MAKS_OPIS];
    char ugradeniDijelovi[MAKS_OPIS];
    float cijenaServisa;
    StatusServisa trenutniStatus;
} RadniNalog;

// DOPUNSKI KONCEPT 1: Jednostruko ili dvostruko povezani popis (Čvor liste)
typedef struct CvorServisa
{
    RadniNalog podaci;
    struct CvorServisa* iduci;
} CvorServisa;

// KONCEPT 13: Generalna upotreba funkcija (Deklaracije CRUD operacija)
void KreirajRadniNalog(void);
void IspisiSveRadneNaloge(void);
void AzurirajRadniNalog(void);
void ObrisiRadniNalog(void);

// Izborničke pod-funkcije i obrada podataka
void UpravljajIzbornikomStatistike(void);
void UpravljajIzbornikomPretrazivanja(void);
void UpravljajIzbornikomSortiranja(void);

// Funkcije za rad s dinamičkom memorijom i vezanom listom
CvorServisa* UcitajDatotekuUListu(void);
void ZapisiListuUDatoteku(CvorServisa* glavaListe);
void OslobodiMemorijuListe(CvorServisa** glavaListe);

// KONCEPT 21: Implementacija funkcije za kopiranje datoteka (Backup sustav)
int KopirajBazuPodataka(const char* IzvornaDatoteka, const char* CiljnaDatoteka);

// Komparacijske funkcije za qsort i bsearch
int UsporediPoIDu(const void* a, const void* b);
int UsporediPoImenu(const void* a, const void* b);

#endif // SERVIS_H