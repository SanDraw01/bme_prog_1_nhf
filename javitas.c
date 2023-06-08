#include <stdio.h>
#include <stdbool.h>
#include "szerviz.h"
#include "debugmalloc.h"

/* Hozz�f�zi az �j elemet a l�ncolt lista v�g�hez */
Javitasok* javitas_felvetele_listaba(Javitasok* eleje, Javitas uj_javitas){
    Javitasok *uj;
    uj = (Javitasok*) malloc(sizeof(Javitasok));
    uj->adat = uj_javitas;
    uj->kov = NULL;

    if (eleje == NULL) {
       eleje = uj;
    }
    else {
       Javitasok *mozgo = eleje;
       while (mozgo->kov != NULL)
          mozgo = mozgo->kov;
       mozgo->kov = uj;
    }
    return eleje;
}
// Rendszam alapjan autohoz rendeli az uj jav�tast
void JavitasHozzaadasa(Bejegyzes* szerviz){
    Auto* keresett = kereses(szerviz);

    char* javitas_neve = (char*) malloc(sizeof(char));
    char* javitas_ideje = (char*) malloc(sizeof(char));
    int javitas_ara = 0;

    if(keresett != NULL){
        printf("Adja meg a jav�t�s nev�t (T�bb sz� eset�n _ -el elv�lasztva): ");
        javitas_neve = beolvas(javitas_neve);
        printf("Adja meg a jav�tas d�tumat (����/HH/NN): ");
        javitas_ideje = beolvas(javitas_ideje);
        printf("Adja meg a jav�t�s �r�t: ");
        scanf("%d", &javitas_ara);

        Javitas uj_javitas;

        uj_javitas.nev = strdup(javitas_neve);
        uj_javitas.datum = strdup(javitas_ideje);
        uj_javitas.ar = javitas_ara;

        keresett->szerviz_tortenet = javitas_felvetele_listaba(keresett->szerviz_tortenet,uj_javitas);
        printf("Jav�t�s sikeresen hozz�adva a %s rendsz�m� aut�hoz!\n", keresett->rendszam);

    }
    else{
        printf("Nincs ilyen aut� a nyilv�ntart�sban!\n");
    }

    free(javitas_neve);
    free(javitas_ideje);
}
// Kiirja egy adott rendszamu aut� jav�t�sait
void JavitasokListazasa(Bejegyzes* szerviz){
    Auto* keresett = kereses(szerviz);
    if(keresett != NULL){
        if(keresett->szerviz_tortenet != NULL){
            printf("Az aut� szervizt�rt�nete: \n");
            Javitasok *mozgo;
            for (mozgo = keresett->szerviz_tortenet; mozgo != NULL; mozgo = mozgo->kov){
                printf("%s %d %s\n", mozgo->adat);
            }
        }
        else{
            printf("Az auto m�g nem volt szervizelve!\n");
        }
    }
    else{
        printf("Nincs ilyen aut� a nyilv�ntart�sban!\n");
    }
}
void JavitasFelszabaditas(Javitasok* szerviztortenet){
    Javitasok* iter = szerviztortenet;
    while (iter != NULL) {
        Javitasok *kov = iter->kov; /* k�vetkez� elem */
        free(iter);
        iter = kov;
    }
}

