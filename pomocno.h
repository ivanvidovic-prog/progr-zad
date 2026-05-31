#ifndef POMOCNO_H
#define POMOCNO_H

// KONCEPT 14: Zaštita parametara kod svih funkcija (korištenje const za ulaze)
void OcistiUnosniSpremnik(void);
void SigurnoUcitajString(char* CiljniSpremnik, const unsigned int MaksDuljina);
int DohvatiValidiraniCijeliBroj(const int MinVrijednost, const int MaxVrijednost);
float DohvatiValidiraniRealniBroj(const float MinVrijednost, const float MaxVrijednost);

#endif // POMOCNO_H