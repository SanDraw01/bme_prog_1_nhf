#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "debugmalloc.h"

// Dinamikusan beolvas egy stringet az stdin-rõl
char* beolvas(char* str){
    int n = 1;
    while(scanf("%c",&str[n-1]) == 1 && str[n-1] != '\n'){
        str = (char*) realloc(str, ++n*sizeof(char));
    }
    str = (char*) realloc(str, ++n*sizeof(char));
    str[n-2] = '\0';

    return str;
}
/* STRUKTÚRÁK */
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

/* LÁNCOLT LISTA -> Autók láncolt listája*/
typedef struct Bejegyzes{
    Auto adat;
    struct Bejegyzes* kov;
} Bejegyzes;

// Hozzáad egy elemet a láncolt lista végéhez
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
/* A paraméterként kapott listában rendszám szerint megkeres egy elemet, és ha a felhasználó megerõsíti, törli azt. */
Bejegyzes* bejegyzesTorlese(Bejegyzes* eleje){
    char* rendszam = (char*) malloc(sizeof(char));
    char choice;
    printf("Adja meg a törlendõ autó rendszámát: ");
    rendszam = beolvas(rendszam);
    char* file_name[13];
    strcat(file_name, rendszam);
    strcat(file_name, ".txt");
    printf("Biztosan törli? (I/N) ");
    scanf("%c", &choice);

    if(toupper(choice) != 'I'){ // Ha N vagy bármi más a bemenet akkor megszakítja a törlést
        printf("Törlés megszakítva!\n");
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
    printf("Autó sikeresen törölve a nyilvántartásból!\n");
    return eleje;
}
/* A <rendszam>.txt-bõl beolvassa az adott rendszámú autó javításait*/
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
/* A szerviz.txt fájlból beolvassa az adatokat*/
Bejegyzes* szervizBeolvas(Bejegyzes* szerviz){
    FILE* fp = fopen("szerviz.txt", "r");
    char line[100];
    Auto uj_auto;
    Ugyfel tulaj;
    if(fp == NULL){
        printf("Nincs ilyen fájl!\n");
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
/* Végig megy a paraméterként kapott listán és kiírja a képernyõre soronként az összes autó tulajdonságát*/
void AutokListazasa(Bejegyzes* szerviz){
    if(szerviz == NULL){
        printf("Nincs autó a nyilvántartásban!\n");
    }
    else{
        Bejegyzes *mozgo;
        for (mozgo = szerviz; mozgo != NULL; mozgo = mozgo->kov){
            printf("%s %s %s %d %s %s %s %s\n", mozgo->adat);
        }
    }
}
/* Elmenti a szerviz adatait egy fajlba amibõl késõbb betudom olvasni*/
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
        printf("Nincs elmentendõ adat!\n");
    }

}
/* A <rendszam>.txt-be beírja az adott rendszámú autó javításait*/
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
/* Megkeres egy autót a kapott listában rendszám alapján (bekéri a rendszámot) */
Auto* kereses(Bejegyzes* szerviz){
    Bejegyzes* mozgo;
    char* rendszam = (char*) malloc(sizeof(char));
    printf("Adja meg a keresett autó rendszámát: ");
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
// Paraméterként kapott rendszám alapján eldönti, hogy az autó létezik e vagy sem
bool autoLetezik(Bejegyzes* szerviz, char* rendszam){
    Bejegyzes* mozgo;

    for (mozgo = szerviz; mozgo != NULL; mozgo = mozgo->kov){
        if(strcmp(mozgo->adat.rendszam,rendszam) == 0){
            return true;
        }
    }
    return false;
}
/* Bekéri az új autó regisztrálásához szükséges adatokat, majd hozzáadja azt a kapott láncolt listához*/
Bejegyzes* uj_auto_felvetele(Bejegyzes* szerviz){

    char* vezetek_nev = (char*) malloc(sizeof(char));
    char* kereszt_nev = (char*) malloc(sizeof(char));
    char* telefonszam = (char*) malloc(sizeof(char));
    char* rendszam = (char*) malloc(sizeof(char));
    char* marka = (char*) malloc(sizeof(char));
    char* tipus = (char*) malloc(sizeof(char));
    char* vizsga_lejarati_ideje = (char*) malloc(sizeof(char));
    int evjarat = 0;


    printf("Adja meg a tulaj vezeték nevét: ");
    vezetek_nev = beolvas(vezetek_nev);

    printf("Adja meg a tulaj kereszt nevét: ");

    kereszt_nev = beolvas(kereszt_nev);
    printf("Adja meg a tulaj telefonszámát: ");

    telefonszam = beolvas(telefonszam);
    printf("Adja meg az autó rendszámát (ABC-123): ");

    rendszam = beolvas(rendszam);
    printf("Adja meg az autó márkáját: ");

    marka = beolvas(marka);
    printf("Adja meg az autó típusát: ");

    tipus = beolvas(tipus);
    printf("Adja meg a mûszaki lejárati idejét (ÉÉÉÉ/HH/NN): ");

    vizsga_lejarati_ideje = beolvas(vizsga_lejarati_ideje);
    printf("Adja meg az autó evjáratát: ");
    scanf("%d", &evjarat);

    if(autoLetezik(szerviz, rendszam)){
        printf("Ez az autó már szerepel a nyilvántartásban!");
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

        printf("Új autó sikeresen felvéve!\n");

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
/* Felszabadítja a láncolt listát */
void listaFelszabaditas(Bejegyzes* szerviz){
    Bejegyzes* iter = szerviz;
    while (iter != NULL) {
        Bejegyzes *kov = iter->kov;
        free(iter);
        iter = kov;
    }
}
/*
    Végigmegy a listán, összehasonlítja az aktuális dátumot a listában levõ autóéval
    Ha az eredmény negatív -> mûszaki lejárt
    Ha az eredmény pozitív és 30nál kisebb -> a mûszaki hamarosan lejár
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
            printf("Lejárt mûszaki: %s %s %s %d %s %s %s %s\n", mozgo->adat);
        }
        else if(auto_napok - mostani_datum_napok <= 30){
            printf("Hamarosan lejáró mûszaki: %s %s %s %d %s %s %s %s\n", mozgo->adat);
        }
    }
}
