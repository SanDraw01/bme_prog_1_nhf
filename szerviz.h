#ifndef SZERVIZ_H_INCLUDED
#define SZERVIZ_H_INCLUDED

typedef struct Ugyfel{
    char* vezeteknev;
    char* keresztnev;
    char* telefonszam;
} Ugyfel;
typedef struct Javitas{
    char* nev;
    int ar;
    char* datum;
} Javitas;
typedef struct Javitasok{
    Javitas adat;
    struct Javitasok* kov;
} Javitasok;
typedef struct Auto{
    char* rendszam;
    char* marka;
    char* tipus;
    int evjarat;
    char* muszaki;
    Ugyfel tulaj;
    Javitasok* szerviz_tortenet;
} Auto;

typedef struct Bejegyzes{
    Auto adat;
    struct Bejegyzes* kov;
} Bejegyzes;

char* beolvas(char* str);
Bejegyzes *bejegyzes_felvetele(Bejegyzes* eleje, Auto uj_auto);
Bejegyzes* bejegyzesTorlese(Bejegyzes* eleje);
Bejegyzes* szervizBeolvas(Bejegyzes* szerviz);
void AutokListazasa(Bejegyzes* szerviz);
void SzervizMentes(Bejegyzes* szerviz);
Auto* kereses(Bejegyzes* szerviz);
Bejegyzes* uj_auto_felvetele(Bejegyzes* szerviz);
void listaFelszabaditas(Bejegyzes* szerviz);
bool autoLetezik(Bejegyzes* szerviz, char* rendszam);
void hamarosanLejaroMuszakik(Bejegyzes* szerviz);
#endif // SZERVIZ_H_INCLUDED
