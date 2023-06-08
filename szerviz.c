#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "debugmalloc.h"

// Dinamikusan beolvas egy stringet az stdin-r�l
char* beolvas(char* str){
    int n = 1;
    while(scanf("%c",&str[n-1]) == 1 && str[n-1] != '\n'){
        str = (char*) realloc(str, ++n*sizeof(char));
    }
    str = (char*) realloc(str, ++n*sizeof(char));
    str[n-2] = '\0';

    return str;
}
/* STRUKT�R�K */
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

/* L�NCOLT LISTA -> Aut�k l�ncolt list�ja*/
typedef struct Bejegyzes{
    Auto adat;
    struct Bejegyzes* kov;
} Bejegyzes;

// Hozz�ad egy elemet a l�ncolt lista v�g�hez
Bejegyzes *bejegyzes_felvetele(Bejegyzes* eleje, Auto uj_auto){
    Bejegyzes *uj;
    uj = (Bejegyzes*) malloc(sizeof(Bejegyzes));
    uj->adat = uj_auto;
    uj->kov = NULL;

    if (eleje == NULL) {
       eleje = uj;
    }
    else {
       Bejegyzes *mozgo = eleje;
       while (mozgo->kov != NULL)
          mozgo = mozgo->kov;
       mozgo->kov = uj;
    }
    return eleje;
}
/* A param�terk�nt kapott list�ban rendsz�m szerint megkeres egy elemet, �s ha a felhaszn�l� meger�s�ti, t�rli azt. */
Bejegyzes* bejegyzesTorlese(Bejegyzes* eleje){
    char* rendszam = (char*) malloc(sizeof(char));
    char choice;
    printf("Adja meg a t�rlend� aut� rendsz�m�t: ");
    rendszam = beolvas(rendszam);
    char* file_name[13];
    strcat(file_name, rendszam);
    strcat(file_name, ".txt");
    printf("Biztosan t�rli? (I/N) ");
    scanf("%c", &choice);

    if(toupper(choice) != 'I'){ // Ha N vagy b�rmi m�s a bemenet akkor megszak�tja a t�rl�st
        printf("T�rl�s megszak�tva!\n");
        free(rendszam);
        return eleje;
    }
    Bejegyzes *lemarado = NULL;
    Bejegyzes *mozgo = eleje;
    while (mozgo != NULL && strcmp(mozgo->adat.rendszam,rendszam) != 0) {
        lemarado = mozgo;
        mozgo = mozgo->kov;
    }

    if (mozgo == NULL) {

    } else if (lemarado == NULL) {
        Bejegyzes *ujeleje = mozgo->kov;
        free(mozgo);
        eleje = ujeleje;
    } else {
        lemarado->kov = mozgo->kov;
        free(mozgo);
    }

    free(rendszam);
    printf("Aut� sikeresen t�r�lve a nyilv�ntart�sb�l!\n");
    return eleje;
}
/* A <rendszam>.txt-b�l beolvassa az adott rendsz�m� aut� jav�t�sait*/
Javitas* javitasBeolvas(Javitas* javitasok, char* rendszam){
    char* file_name[13];
    strcat(file_name, rendszam);
    strcat(file_name, ".txt");

    FILE* fp = fopen(file_name, "r");
    if(fp == NULL){
        return NULL;
    }

    char line[100];
    Javitas uj_javitas;
    while (fgets(line, sizeof(line), fp)) {

       size_t len = strlen(line);
       if (len > 0 && line[len-1] == '\n') {
         line[--len] = '\0';
       }
       uj_javitas.nev = strdup(strtok(line, " "));
       uj_javitas.ar = atoi(strtok(NULL, " "));
       uj_javitas.datum = strdup(strtok(NULL, " "));

       javitasok = javitas_felvetele_listaba(javitasok, uj_javitas);
    }

    fclose(fp);

    return javitasok;
}
/* A szerviz.txt f�jlb�l beolvassa az adatokat*/
Bejegyzes* szervizBeolvas(Bejegyzes* szerviz){
    FILE* fp = fopen("szerviz.txt", "r");
    char line[100];
    Auto uj_auto;
    Ugyfel tulaj;
    if(fp == NULL){
        printf("Nincs ilyen f�jl!\n");
        return NULL;
    }
    while (fgets(line, sizeof(line), fp)) {
       Javitasok* szerviz_tortenet = NULL;
       size_t len = strlen(line);
       if (len > 0 && line[len-1] == '\n') {
         line[--len] = '\0';
       }

       uj_auto.rendszam = strdup(strtok(line, " "));
       uj_auto.marka = strdup(strtok(NULL, " "));
       uj_auto.tipus = strdup(strtok(NULL, " "));
       uj_auto.evjarat = atoi(strtok(NULL, " "));
       uj_auto.muszaki = strdup(strtok(NULL, " "));
       uj_auto.tulaj.vezeteknev = strdup(strtok(NULL, " "));
       uj_auto.tulaj.keresztnev = strdup(strtok(NULL, " "));
       uj_auto.tulaj.telefonszam = strdup(strtok(NULL, " "));

       szerviz_tortenet = javitasBeolvas(szerviz_tortenet, uj_auto.rendszam);
       uj_auto.szerviz_tortenet = szerviz_tortenet;
       szerviz = bejegyzes_felvetele(szerviz, uj_auto);
    }

    fclose(fp);

    return szerviz;
}
/* V�gig megy a param�terk�nt kapott list�n �s ki�rja a k�perny�re soronk�nt az �sszes aut� tulajdons�g�t*/
void AutokListazasa(Bejegyzes* szerviz){
    if(szerviz == NULL){
        printf("Nincs aut� a nyilv�ntart�sban!\n");
    }
    else{
        Bejegyzes *mozgo;
        for (mozgo = szerviz; mozgo != NULL; mozgo = mozgo->kov){
            printf("%s %s %s %d %s %s %s %s\n", mozgo->adat);
        }
    }
}
/* Elmenti a szerviz adatait egy fajlba amib�l k�s�bb betudom olvasni*/
void SzervizMentes(Bejegyzes* szerviz){
    if(szerviz != NULL){
        Bejegyzes* mozgo;
        FILE* fp = fopen("szerviz.txt","w");
        for (mozgo = szerviz; mozgo != NULL; mozgo = mozgo->kov){
            fprintf(fp,"%s %s %s %d %s %s %s %s\n", mozgo->adat);
            JavitasMentes(mozgo->adat.szerviz_tortenet, mozgo->adat.rendszam);
        }
        fclose(fp);
        printf("Adatok sikeresen elmentve!\n");
        listaFelszabaditas(szerviz);
    }
    else{
        printf("Nincs elmentend� adat!\n");
    }

}
/* A <rendszam>.txt-be be�rja az adott rendsz�m� aut� jav�t�sait*/
void JavitasMentes(Javitasok* szerviz_tortenet,char* rendszam){
    char* file_name[13];
    file_name[0] = '\0';
    strcat(file_name, rendszam);
    strcat(file_name, ".txt");

    if(szerviz_tortenet != NULL){
        FILE* fp = fopen(file_name, "w");
        Javitasok* mozgo;
        for (mozgo = szerviz_tortenet; mozgo != NULL; mozgo = mozgo->kov){
            fprintf(fp,"%s %d %s\n", mozgo->adat);
        }
        fclose(fp);
        JavitasFelszabaditas(szerviz_tortenet);
    }
}
/* Megkeres egy aut�t a kapott list�ban rendsz�m alapj�n (bek�ri a rendsz�mot) */
Auto* kereses(Bejegyzes* szerviz){
    Bejegyzes* mozgo;
    char* rendszam = (char*) malloc(sizeof(char));
    printf("Adja meg a keresett aut� rendsz�m�t: ");
    rendszam = beolvas(rendszam);
    for (mozgo = szerviz; mozgo != NULL; mozgo = mozgo->kov){
        if(strcmp(mozgo->adat.rendszam,rendszam) == 0){
            free(rendszam);
            return &mozgo->adat;
        }
    }
    free(rendszam);
    return NULL;
}
// Param�terk�nt kapott rendsz�m alapj�n eld�nti, hogy az aut� l�tezik e vagy sem
bool autoLetezik(Bejegyzes* szerviz, char* rendszam){
    Bejegyzes* mozgo;

    for (mozgo = szerviz; mozgo != NULL; mozgo = mozgo->kov){
        if(strcmp(mozgo->adat.rendszam,rendszam) == 0){
            return true;
        }
    }
    return false;
}
/* Bek�ri az �j aut� regisztr�l�s�hoz sz�ks�ges adatokat, majd hozz�adja azt a kapott l�ncolt list�hoz*/
Bejegyzes* uj_auto_felvetele(Bejegyzes* szerviz){

    char* vezetek_nev = (char*) malloc(sizeof(char));
    char* kereszt_nev = (char*) malloc(sizeof(char));
    char* telefonszam = (char*) malloc(sizeof(char));
    char* rendszam = (char*) malloc(sizeof(char));
    char* marka = (char*) malloc(sizeof(char));
    char* tipus = (char*) malloc(sizeof(char));
    char* vizsga_lejarati_ideje = (char*) malloc(sizeof(char));
    int evjarat = 0;


    printf("Adja meg a tulaj vezet�k nev�t: ");
    vezetek_nev = beolvas(vezetek_nev);

    printf("Adja meg a tulaj kereszt nev�t: ");

    kereszt_nev = beolvas(kereszt_nev);
    printf("Adja meg a tulaj telefonsz�m�t: ");

    telefonszam = beolvas(telefonszam);
    printf("Adja meg az aut� rendsz�m�t (ABC-123): ");

    rendszam = beolvas(rendszam);
    printf("Adja meg az aut� m�rk�j�t: ");

    marka = beolvas(marka);
    printf("Adja meg az aut� t�pus�t: ");

    tipus = beolvas(tipus);
    printf("Adja meg a m�szaki lej�rati idej�t (����/HH/NN): ");

    vizsga_lejarati_ideje = beolvas(vizsga_lejarati_ideje);
    printf("Adja meg az aut� evj�rat�t: ");
    scanf("%d", &evjarat);

    if(autoLetezik(szerviz, rendszam)){
        printf("Ez az aut� m�r szerepel a nyilv�ntart�sban!");
    }
    else{
        Auto uj_auto;

        uj_auto.rendszam = strdup(rendszam);
        uj_auto.marka = strdup(marka);
        uj_auto.tipus = strdup(tipus);
        uj_auto.muszaki = strdup(vizsga_lejarati_ideje);

        uj_auto.tulaj.vezeteknev = strdup(vezetek_nev);
        uj_auto.tulaj.keresztnev = strdup(kereszt_nev);
        uj_auto.tulaj.telefonszam = strdup(telefonszam);

        uj_auto.szerviz_tortenet = NULL;
        uj_auto.evjarat = evjarat;
        szerviz = bejegyzes_felvetele(szerviz, uj_auto);

        printf("%s %s %s %d %s %s %s %s\n", uj_auto);

        printf("�j aut� sikeresen felv�ve!\n");

        free(vezetek_nev);
        free(kereszt_nev);
        free(telefonszam);
        free(rendszam);
        free(marka);
        free(tipus);
        free(vizsga_lejarati_ideje);
    }

    return szerviz;
}
/* Felszabad�tja a l�ncolt list�t */
void listaFelszabaditas(Bejegyzes* szerviz){
    Bejegyzes* iter = szerviz;
    while (iter != NULL) {
        Bejegyzes *kov = iter->kov;
        free(iter);
        iter = kov;
    }
}
/*
    V�gigmegy a list�n, �sszehasonl�tja az aktu�lis d�tumot a list�ban lev� aut��val
    Ha az eredm�ny negat�v -> m�szaki lej�rt
    Ha az eredm�ny pozit�v �s 30n�l kisebb -> a m�szaki hamarosan lej�r
*/
void hamarosanLejaroMuszakik(Bejegyzes* szerviz){
    Bejegyzes* mozgo;
    int ev, honap, nap;
    char* datum[11];
    SYSTEMTIME t;
    GetLocalTime(&t);
    int mostani_datum_napok = t.wYear*365 + t.wMonth *30 + t.wDay;
    int auto_napok = 0;

    for(mozgo=szerviz; mozgo != NULL; mozgo = mozgo->kov){
        strcpy(datum, mozgo->adat.muszaki);

        ev = atoi(strtok(datum, "/")) * 365;
        honap = atoi(strtok(NULL, "/")) * 30;
        nap = atoi(strtok(NULL, "/"));

        auto_napok = ev+honap+nap;
        if(auto_napok - mostani_datum_napok < 0){
            printf("Lej�rt m�szaki: %s %s %s %d %s %s %s %s\n", mozgo->adat);
        }
        else if(auto_napok - mostani_datum_napok <= 30){
            printf("Hamarosan lej�r� m�szaki: %s %s %s %d %s %s %s %s\n", mozgo->adat);
        }
    }
}
