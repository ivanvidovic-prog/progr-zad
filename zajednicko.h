#ifndef ZAJEDNICKO_H
#define ZAJEDNICKO_H

// KONCEPT 11: Kod izbornika koristiti enum tipove ili makro simbole
#define DATOTEKA_SERVISA "servisi.dat"
#define PRIVREMENA_DATOTEKA "privremeni_servisi.dat"

// KONCEPT 8: Primjena extern ključne riječi za globalne varijable
extern int ukupnoServisa;
extern float ukupniPrihod;

void AzurirajGlobalneVarijable(void);

#endif // ZAJEDNICKO_H