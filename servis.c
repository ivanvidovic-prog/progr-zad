#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h> // KONCEPT 22: Upravljanje s pogreškama pomoću errno

#include "servis.h"
#include "pomocno.h"
#include "zajednicko.h"

// KONCEPT 6: Primjena ključne riječi static za globalne varijable na razini modula
static int lokalniZapisnikOperacija = 0;

// KONCEPT 12: Upotreba pokazivača & KONCEPT 14: Zaštita parametara pomoću 'const'
static void IspisiJedanNalog(const RadniNalog* const nalogPokazivac)
{
    if (nalogPokazivac == NULL) return;

    printf("\n==================================================\n");
    printf(" RADNI NALOG ID: %d\n", nalogPokazivac->idServisa);
    printf("--------------------------------------------------\n");
    printf(" Klijent         : %s\n", nalogPokazivac->imeKlijenta);
    printf(" Model bicikla   : %s\n", nalogPokazivac->modelBicikla);
    printf(" Opis kvara      : %s\n", nalogPokazivac->opisKvara);
    printf(" Ugradeni dijelovi: %s\n", nalogPokazivac->ugradeniDijelovi);
    printf(" Cijena servisa  : %.2f EUR (%.2f KN)\n", nalogPokazivac->cijenaServisa, EUR_U_KN(nalogPokazivac->cijenaServisa));
    printf(" Iznos PDV-a (25%%): %.2f EUR\n", IzracunajPorez(nalogPokazivac->cijenaServisa));
    printf(" Status naloga   : ");

    switch (nalogPokazivac->trenutniStatus)
    {
    case STATUS_ZAPRIMLJEN: printf("ZAPRIMLJEN\n"); break;
    case STATUS_U_TIJEKU:    printf("U TIJEKU SERVISIRANJA\n"); break;
    case STATUS_ZAVRSEN:    printf("ZAVRSEN I NAPLACEN\n"); break;
    default:                printf("NEPOZNAT STATUS\n"); break;
    }
    printf("==================================================\n");
}

// Pomoćna funkcija za provjeru jedinstvenosti ID-a
static int DaLiIdVecPostoji(int provjeriId)
{
    CvorServisa* glava = UcitajDatotekuUListu();
    CvorServisa* trenutni = glava;
    int pronaden = 0;

    while (trenutni != NULL)
    {
        if (trenutni->podaci.idServisa == provjeriId)
        {
            pronaden = 1;
            break;
        }
        trenutni = trenutni->iduci;
    }

    OslobodiMemorijuListe(&glava);
    return pronaden;
}

// KONCEPT 21: Implementacija funkcije za kopiranje datoteka (Backup sustav)
int KopirajBazuPodataka(const char* IzvornaDatoteka, const char* CiljnaDatoteka)
{
    // KONCEPT 19: Datoteke, provjera pokazivača i zatvaranje datoteke
    FILE* izvor = fopen(IzvornaDatoteka, "rb");
    if (izvor == NULL)
    {
        // Ako izvorna datoteka još ne postoji, nema se što kopirati (prvo pokretanje)
        return 0;
    }

    FILE* cilj = fopen(CiljnaDatoteka, "wb");
    if (cilj == NULL)
    {
        // KONCEPT 22: Upravljanje s pogreškama upotrebom perror() funkcije
        perror("[GRESKA] Neuspjelo kreiranje sigurnosne kopije baze podataka");
        fclose(izvor);
        return -1;
    }

    char spremnikBloka[512];
    size_t procitaniBajtovi;

    // Čitanje i pisanje blokova podataka (demonstracija sigurnog rukovanja tokovima)
    while ((procitaniBajtovi = fread(spremnikBloka, 1, sizeof(spremnikBloka), izvor)) > 0)
    {
        fwrite(spremnikBloka, 1, procitaniBajtovi, cilj);
    }

    fclose(izvor);
    fclose(cilj);
    return 1;
}

// KONCEPT 16: Koristiti dinamičko zauzimanje memorije za složene tipove podataka (Vezana lista)
// KONCEPT 17: Koristiti funkcije malloc(), calloc(), realloc(), free()
CvorServisa* UcitajDatotekuUListu(void)
{
    FILE* datoteka = fopen(DATOTEKA_SERVISA, "rb");
    if (datoteka == NULL)
    {
        return NULL;
    }

    CvorServisa* glava = NULL;
    CvorServisa* rep = NULL;
    RadniNalog tempNalog;

    while (fread(&tempNalog, sizeof(RadniNalog), 1, datoteka) == 1)
    {
        // KONCEPT 17: Korištenje malloc() za dinamičku alokaciju čvora liste
        CvorServisa* noviCvor = (CvorServisa*)malloc(sizeof(CvorServisa));
        if (noviCvor == NULL)
        {
            perror("[KRITICNA GRESKA] Dinamicka alokacija memorije nije uspjela");
            fclose(datoteka);
            return glava;
        }

        noviCvor->podaci = tempNalog;
        noviCvor->iduci = NULL;

        if (glava == NULL)
        {
            glava = noviCvor;
            rep = noviCvor;
        }
        else
        {
            rep->iduci = noviCvor;
            rep = noviCvor;
        }
    }

    fclose(datoteka);
    return glava;
}

void ZapisiListuUDatoteku(CvorServisa* glavaListe)
{
    FILE* datoteka = fopen(DATOTEKA_SERVISA, "wb");
    if (datoteka == NULL)
    {
        perror("[GRESKA] Neuspjelo otvaranje datoteke za zapis");
        return;
    }

    CvorServisa* trenutni = glavaListe;
    while (trenutni != NULL)
    {
        fwrite(&(trenutni->podaci), sizeof(RadniNalog), 1, datoteka);
        trenutni = trenutni->iduci;
    }

    fclose(datoteka);
}

// KONCEPT 18: Sigurno brisanje memorije, provjera pokazivača, anuliranje prostora i pokazivača
void OslobodiMemorijuListe(CvorServisa** glavaListe)
{
    if (glavaListe == NULL || *glavaListe == NULL) return;

    CvorServisa* trenutni = *glavaListe;
    CvorServisa* iduciCvor = NULL;

    while (trenutni != NULL)
    {
        iduciCvor = trenutni->iduci;

        // KONCEPT 18: Anuliranje sadržaja prije oslobađanja radi apsolutne sigurnosti
        memset(trenutni, 0, sizeof(CvorServisa));

        free(trenutni);
        trenutni = iduciCvor;
    }

    // KONCEPT 18: Anuliranje samog glavnog pokazivača kako bi se spriječio "wild pointer"
    *glavaListe = NULL;
}

// KONCEPT 23: Implementacija - CREATE (S automatskim predlaganjem kvarova i dijelova)
void KreirajRadniNalog(void)
{
    printf("\n>>> DODAVANJE NOVOG RADNOG NALOGA <<<\n");
    printf("Unesite jedinstveni ID novog servisa: ");
    int noviId = DohvatiValidiraniCijeliBroj(1, 99999);

    if (DaLiIdVecPostoji(noviId))
    {
        printf("\n[UPOZORENJE] Radni nalog s tim ID-em vec postoji u bazi!\n");
        return;
    }

    CvorServisa* noviCvor = (CvorServisa*)malloc(sizeof(CvorServisa));
    if (noviCvor == NULL)
    {
        printf("[GRESKA] Neuspjela alokacija memorije.\n");
        return;
    }

    noviCvor->podaci.idServisa = noviId;

    printf("Ime i prezime klijenta: ");
    SigurnoUcitajString(noviCvor->podaci.imeKlijenta, MAKS_NAZIV);

    printf("Model i marka bicikla: ");
    SigurnoUcitajString(noviCvor->podaci.modelBicikla, MAKS_NAZIV);

    // --- AUTOMATSKI IZBORNIK ZA BRZI ODABIR KVAROVA I DIJELOVA ---
    printf("\nOdaberite tipicni kvar s popisa (ili izaberite rucni unos):\n");
    printf("1. Probusena guma (Zamjena unutarnje gume)\n");
    printf("2. Istrosene kocnice (Zamjena kocionih obloga/pakni)\n");
    printf("3. Puknut ili rastegnut lanac (Zamjena lanca i podmazivanje)\n");
    printf("4. Rucni unos (Sami upisujete kvar i dijelove)\n");
    printf("Vas odabir (1-4): ");
    int odabirKvara = DohvatiValidiraniCijeliBroj(1, 4);

    float predlozenaCijena = 0.0f;

    if (odabirKvara == 1)
    {
        strcpy(noviCvor->podaci.opisKvara, "Probusena straznja guma na biciklu.");
        strcpy(noviCvor->podaci.ugradeniDijelovi, "Unutarnja guma Continental 28\"");
        predlozenaCijena = 15.0f;
        printf("[INFO] Automatski dodano: Guma Continental (15.00 EUR)\n");
    }
    else if (odabirKvara == 2)
    {
        strcpy(noviCvor->podaci.opisKvara, "Kocnice slabo hvataju, istrosenost materijala.");
        strcpy(noviCvor->podaci.ugradeniDijelovi, "Kocione pakne Shimano V-Brake (par)");
        predlozenaCijena = 25.0f;
        printf("[INFO] Automatski dodano: Pakne Shimano (25.00 EUR)\n");
    }
    else if (odabirKvara == 3)
    {
        strcpy(noviCvor->podaci.opisKvara, "Lanac preskace i rastegnut je preko limita.");
        strcpy(noviCvor->podaci.ugradeniDijelovi, "Lanac KMC X9 + sprej za podmazivanje");
        predlozenaCijena = 35.0f;
        printf("[INFO] Automatski dodano: Lanac KMC X9 (35.00 EUR)\n");
    }
    else // Ručni unos
    {
        printf("Detaljan opis kvara: ");
        SigurnoUcitajString(noviCvor->podaci.opisKvara, MAKS_OPIS);

        printf("Popis ugradenih dijelova: ");
        SigurnoUcitajString(noviCvor->podaci.ugradeniDijelovi, MAKS_OPIS);
    }

    printf("Cijena obavljenog servisa u EUR (Predlozeno: %.2f): ", predlozenaCijena);
    noviCvor->podaci.cijenaServisa = DohvatiValidiraniRealniBroj(0.0f, 10000.0f);

    printf("Odaberite pocetni status (0-Zaprimljen, 1-U tijeku, 2-Zavrsen): ");
    noviCvor->podaci.trenutniStatus = (StatusServisa)DohvatiValidiraniCijeliBroj(0, 2);
    noviCvor->iduci = NULL;

    CvorServisa* glava = UcitajDatotekuUListu();
    if (glava == NULL)
    {
        glava = noviCvor;
    }
    else
    {
        CvorServisa* privremena = glava;
        while (privremena->iduci != NULL)
        {
            privremena = privremena->iduci;
        }
        privremena->iduci = noviCvor;
    }

    // Radimo backup baze podataka prije upisa novog stanja na disk
    KopirajBazuPodataka(DATOTEKA_SERVISA, SIGURNOSNA_KOPIJA_DATOTEKE);

    ZapisiListuUDatoteku(glava);
    OslobodiMemorijuListe(&glava);

    lokalniZapisnikOperacija++;
    AzurirajGlobalneVarijable();
    printf("\n[USPJEH] Novi radni nalog je uspjesno kreiran i spremljen!\n");
}

// KONCEPT 23: Implementacija - READ
void IspisiSveRadneNaloge(void)
{
    CvorServisa* glava = UcitajDatotekuUListu();
    if (glava == NULL)
    {
        printf("\n[OBAVIJEST] Baza podataka je trenutno prazna.\n");
        return;
    }

    printf("\n >>> ISPIS SVIH EVIDENTIRANIH RADNIH NALOGA <<<\n");
    CvorServisa* trenutni = glava;
    int brojac = 0;

    while (trenutni != NULL)
    {
        IspisiJedanNalog(&(trenutni->podaci));
        trenutni = trenutni->iduci;
        brojac++;
    }

    OslobodiMemorijuListe(&glava);
    printf("\nUkupno ispisano naloga iz memorijske liste: %d\n", brojac);
}

// KONCEPT 23: Implementacija - UPDATE
void AzurirajRadniNalog(void)
{
    printf("\nUnesite ID radnog naloga koji zelite izmijeniti: ");
    int ciljaniId = DohvatiValidiraniCijeliBroj(1, 99999);

    CvorServisa* glava = UcitajDatotekuUListu();
    CvorServisa* trenutni = glava;
    int pronaden = 0;

    while (trenutni != NULL)
    {
        if (trenutni->podaci.idServisa == ciljaniId)
        {
            pronaden = 1;
            printf("\n--- Pronadeni nalog prije izmjene ---");
            IspisiJedanNalog(&(trenutni->podaci));

            printf("\nUnesite NOVO ime klijenta: ");
            SigurnoUcitajString(trenutni->podaci.imeKlijenta, MAKS_NAZIV);

            printf("Unesite NOVI model bicikla: ");
            SigurnoUcitajString(trenutni->podaci.modelBicikla, MAKS_NAZIV);

            printf("Unesite NOVI opis kvara: ");
            SigurnoUcitajString(trenutni->podaci.opisKvara, MAKS_OPIS);

            printf("Unesite NOVE ugradene dijelove: ");
            SigurnoUcitajString(trenutni->podaci.ugradeniDijelovi, MAKS_OPIS);

            printf("Unesite NOVU cijenu servisa (EUR): ");
            trenutni->podaci.cijenaServisa = DohvatiValidiraniRealniBroj(0.0f, 10000.0f);

            printf("Unesite NOVI status (0-Zaprimljen, 1-U tijeku, 2-Zavrsen): ");
            trenutni->podaci.trenutniStatus = (StatusServisa)DohvatiValidiraniCijeliBroj(0, 2);
            break;
        }
        trenutni = trenutni->iduci;
    }

    if (pronaden)
    {
        // Sigurnosna kopija prije prepisivanja datoteke
        KopirajBazuPodataka(DATOTEKA_SERVISA, SIGURNOSNA_KOPIJA_DATOTEKE);
        ZapisiListuUDatoteku(glava);
        printf("\n[USPJEH] Radni nalog ID %d uspjesno je azuriran!\n", ciljaniId);
    }
    else
    {
        printf("\n[GRESKA] Radni nalog s ID-em %d nije pronaden.\n", ciljaniId);
    }

    OslobodiMemorijuListe(&glava);
    AzurirajGlobalneVarijable();
}

// KONCEPT 23: Implementacija - DELETE & KONCEPT 21: Korištenje funkcija remove() i rename()
void ObrisiRadniNalog(void)
{
    printf("\nUnesite ID radnog naloga koji zelite OBRISATI: ");
    int ciljaniId = DohvatiValidiraniCijeliBroj(1, 99999);

    FILE* staraDatoteka = fopen(DATOTEKA_SERVISA, "rb");
    if (staraDatoteka == NULL)
    {
        printf("\n[GRESKA] Datoteka baze ne postoji, brisanje je nemoguce.\n");
        return;
    }

    FILE* novaDatoteka = fopen(PRIVREMENA_DATOTEKA, "wb");
    if (novaDatoteka == NULL)
    {
        fclose(staraDatoteka);
        return;
    }

    RadniNalog privremeni;
    int obrisano = 0;

    while (fread(&privremeni, sizeof(RadniNalog), 1, staraDatoteka) == 1)
    {
        if (privremeni.idServisa == ciljaniId)
        {
            obrisano = 1;
            continue;
        }
        fwrite(&privremeni, sizeof(RadniNalog), 1, novaDatoteka);
    }

    fclose(staraDatoteka);
    fclose(novaDatoteka);

    if (obrisano)
    {
        // Prije brisanja originala radimo sigurnosnu kopiju
        KopirajBazuPodataka(DATOTEKA_SERVISA, SIGURNOSNA_KOPIJA_DATOTEKE);

        // KONCEPT 21: Primjena funkcija remove i rename
        remove(DATOTEKA_SERVISA);
        rename(PRIVREMENA_DATOTEKA, DATOTEKA_SERVISA);
        printf("\n[USPJEH] Radni nalog ID %d uspjesno uklonjen iz sustava!\n", ciljaniId);
    }
    else
    {
        remove(PRIVREMENA_DATOTEKA);
        printf("\n[OBAVIJEST] Radni nalog s ID-em %d nije pronaden.\n", ciljaniId);
    }

    AzurirajGlobalneVarijable();
}

void UpravljajIzbornikomStatistike(void)
{
    int odabir;
    AzurirajGlobalneVarijable();

    do
    {
        PrikaziIzbornikStatistike();
        printf("Odaberite opciju statistike: ");
        odabir = DohvatiValidiraniCijeliBroj(0, 3);

        switch ((StatistikaOpcija)odabir)
        {
        case STAT_UKUPAN_BROJ:
            printf("\n>> Ukupan broj registriranih servisa u bazi: %d\n", ukupnoServisa);
            break;
        case STAT_UKUPAN_PRIHOD:
            printf("\n>> Ukupni ostvareni prihod: %.2f EUR\n", ukupniPrihod);
            printf(">> Protuvrijednost u bivsim kunama: %.2f KN\n", EUR_U_KN(ukupniPrihod));
            break;
        case STAT_PROSJEK:
            if (ukupnoServisa > 0)
            {
                printf("\n>> Prosjecna cijena po jednom radnom nalogu: %.2f EUR\n", ukupniPrihod / ukupnoServisa);
            }
            else
            {
                printf("\n>> Baza podataka je prazna, nemoguce izracunati prosjek.\n");
            }
            break;
        case STAT_NATRAG:
            printf("\nPovratak u glavni izbornik...\n");
            break;
        }
    } while (odabir != STAT_NATRAG);
}

// KONCEPT 26: Pokazivači na funkcije (Komparacija po ID-u za qsort i bsearch)
int UsporediPoIDu(const void* a, const void* b)
{
    const RadniNalog* nalogA = (const RadniNalog*)a;
    const RadniNalog* nalogB = (const RadniNalog*)b;
    return (nalogA->idServisa - nalogB->idServisa);
}

// Pokazivač na funkciju: Komparacija po imenu abecedno za qsort
int UsporediPoImenu(const void* a, const void* b)
{
    const RadniNalog* nalogA = (const RadniNalog*)a;
    const RadniNalog* nalogB = (const RadniNalog*)b;
    return strcmp(nalogA->imeKlijenta, nalogB->imeKlijenta);
}

void UpravljajIzbornikomPretrazivanja(void)
{
    int odabir;
    do
    {
        PrikaziIzbornikPretrazivanja();
        printf("Odaberite opciju pretrazivanja: ");
        odabir = DohvatiValidiraniCijeliBroj(0, 3);

        if (odabir == PRETRAGA_NATRAG) continue;

        CvorServisa* glava = UcitajDatotekuUListu();
        if (glava == NULL)
        {
            printf("\n[OBAVIJEST] Baza je prazna. Pretrazivanje nemoguce.\n");
            continue;
        }

        if (odabir == PRETRAGA_LINEARNA_ID)
        {
            printf("Unesite ID trazenog servisa: ");
            int trazeniId = DohvatiValidiraniCijeliBroj(1, 99999);
            int pronadeno = 0;

            CvorServisa* trenutni = glava;
            while (trenutni != NULL)
            {
                if (trenutni->podaci.idServisa == trazeniId)
                {
                    printf("\n[REZULTAT] Nalog pronaden linearnom pretragom:\n");
                    IspisiJedanNalog(&(trenutni->podaci));
                    pronadeno = 1;
                    break;
                }
                trenutni = trenutni->iduci;
            }
            if (!pronadeno) printf("\nNema zapisa s trazenim ID-em.\n");
        }
        else if (odabir == PRETRAGA_LINEARNA_IME)
        {
            printf("Unesite ime klijenta ili dio imena: ");
            char trazenoIme[MAKS_NAZIV];
            SigurnoUcitajString(trazenoIme, MAKS_NAZIV);
            int brojacMecova = 0;

            CvorServisa* trenutni = glava;
            while (trenutni != NULL)
            {
                if (strstr(trenutni->podaci.imeKlijenta, trazenoIme) != NULL)
                {
                    IspisiJedanNalog(&(trenutni->podaci));
                    brojacMecova++;
                }
                trenutni = trenutni->iduci;
            }
            printf("\nUkupno pronadeno rezultata za trazeni pojam: %d\n", brojacMecova);
        }
        // KONCEPT 24: Pretraživanje - obavezna ugrađena funkcija bsearch()
        else if (odabir == PRETRAGA_BSEARCH_ID)
        {
            RadniNalog statickoPolje[MAKS_STATICKI_LIMIT];
            int n = 0;
            CvorServisa* temp = glava;

            while (temp != NULL && n < MAKS_STATICKI_LIMIT)
            {
                statickoPolje[n++] = temp->podaci;
                temp = temp->iduci;
            }

            // Bsearch zahtijeva sortirano polje
            qsort(statickoPolje, n, sizeof(RadniNalog), UsporediPoIDu);

            printf("Unesite ID servisa za brzu bsearch pretragu: ");
            RadniNalog kljucPretrage;
            kljucPretrage.idServisa = DohvatiValidiraniCijeliBroj(1, 99999);

            // KONCEPT 24 & KONCEPT 26
            RadniNalog* rezultat = (RadniNalog*)bsearch(&kljucPretrage, statickoPolje, n, sizeof(RadniNalog), UsporediPoIDu);

            if (rezultat != NULL)
            {
                printf("\n[USPJEH] Pronaden nalog upotrebom standardne bsearch() funkcije:\n");
                IspisiJedanNalog(rezultat);
            }
            else
            {
                printf("\nNalog s trazenim ID-em nije pronaden u sortiranoj bazi.\n");
            }
        }

        OslobodiMemorijuListe(&glava);

    } while (odabir != PRETRAGA_NATRAG);
}

// KONCEPT 25: Rekurzije (Rekurzivna implementacija QuickSort algoritma po cijeni servisa)
static void RekurzivniQuickSortCijena(RadniNalog polje[], int lijevo, int desno)
{
    if (lijevo >= desno) return;

    float pivot = polje[desno].cijenaServisa;
    int i = lijevo - 1;

    for (int j = lijevo; j < desno; j++)
    {
        if (polje[j].cijenaServisa <= pivot)
        {
            i++;
            RadniNalog privremena = polje[i];
            polje[i] = polje[j];
            polje[j] = privremena;
        }
    }
    RadniNalog privremena = polje[i + 1];
    polje[i + 1] = polje[desno];
    polje[desno] = privremena;

    int indeksPivota = i + 1;

    // Rekurzivni pozivi funkcije za lijevu i desnu stranu pod-polja
    RekurzivniQuickSortCijena(polje, lijevo, indeksPivota - 1);
    RekurzivniQuickSortCijena(polje, indeksPivota + 1, desno);
}

void UpravljajIzbornikomSortiranja(void)
{
    int odabir;
    do
    {
        PrikaziIzbornikSortiranja();
        printf("Odaberite vrstu sortiranja (0-3): ");
        odabir = DohvatiValidiraniCijeliBroj(0, 3);

        if (odabir == SORT_NATRAG) continue;

        CvorServisa* glava = UcitajDatotekuUListu();
        if (glava == NULL)
        {
            printf("\n[OBAVIJEST] Baza je prazna, sortiranje nije moguce.\n");
            continue;
        }

        RadniNalog lokalnoPolje[MAKS_STATICKI_LIMIT];
        int duljinaPolja = 0;
        CvorServisa* trenutni = glava;

        while (trenutni != NULL && duljinaPolja < MAKS_STATICKI_LIMIT)
        {
            lokalnoPolje[duljinaPolja++] = trenutni->podaci;
            trenutni = trenutni->iduci;
        }

        // KONCEPT 20: Korištenje naprednih funkcija datoteka fseek(), ftell() i rewind()
        FILE* testFile = fopen(DATOTEKA_SERVISA, "rb");
        if (testFile != NULL)
        {
            fseek(testFile, 0, SEEK_END);
            long velicinaDatoteke = ftell(testFile);
            rewind(testFile);
            fclose(testFile);
            printf("\n[INFO] Trenutna velicina binarne datoteke na disku iznosi: %ld bajtova.\n", velicinaDatoteke);
        }

        // KONCEPT 23 & 26: Ugrađeni qsort po ID-u
        if (odabir == SORT_QSORT_ID)
        {
            qsort(lokalnoPolje, duljinaPolja, sizeof(RadniNalog), UsporediPoIDu);
            printf("\n[USPJEH] Podaci sortirani po ID-u (Ugrađeni qsort).\n");
        }
        // KONCEPT 25: Rekurzija kroz QuickSort po cijeni
        else if (odabir == SORT_REKURZIVNI_CIJENA)
        {
            RekurzivniQuickSortCijena(lokalnoPolje, 0, duljinaPolja - 1);
            printf("\n[USPJEH] Podaci sortirani po CIJENI uzlazno (Rekurzivni QuickSort).\n");
        }
        // KONCEPT 23 & 26: Ugrađeni qsort abecedno po imenu klijenta
        else if (odabir == SORT_QSORT_IME)
        {
            qsort(lokalnoPolje, duljinaPolja, sizeof(RadniNalog), UsporediPoImenu);
            printf("\n[USPJEH] Podaci sortirani abecedno po imenu klijenta (Ugrađeni qsort).\n");
        }

        printf("\n--- REZULTATI NAKON SORTIRANJA ---\n");
        for (int i = 0; i < duljinaPolja; i++)
        {
            IspisiJedanNalog(&lokalnoPolje[i]);
        }

        printf("\nZelite li trajno pohraniti ovaj novi poredak u datoteku? (1-DA, 0-NE): ");
        int spremiSve = DohvatiValidiraniCijeliBroj(0, 1);
        if (spremiSve == 1)
        {
            CvorServisa* novaGlava = NULL;
            CvorServisa* zadnji = NULL;

            for (int i = 0; i < duljinaPolja; i++)
            {
                CvorServisa* noviCvor = (CvorServisa*)malloc(sizeof(CvorServisa));
                if (noviCvor != NULL)
                {
                    noviCvor->podaci = lokalnoPolje[i];
                    noviCvor->iduci = NULL;
                    if (novaGlava == NULL)
                    {
                        novaGlava = noviCvor;
                        zadnji = noviCvor;
                    }
                    else
                    {
                        zadnji->iduci = noviCvor;
                        zadnji = noviCvor;
                    }
                }
            }
            // Radimo backup i ovdje prije trajnog preslagivanja datoteke na disku
            KopirajBazuPodataka(DATOTEKA_SERVISA, SIGURNOSNA_KOPIJA_DATOTEKE);
            ZapisiListuUDatoteku(novaGlava);
            OslobodiMemorijuListe(&novaGlava);
            printf("[POHRANJENO] Novi poredak je uspjesno zapisan.\n");
        }

        OslobodiMemorijuListe(&glava);
    } while (odabir != SORT_NATRAG);
}