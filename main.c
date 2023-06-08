#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
#endif
#include "szerviz.h"
#include "javitas.h"
#include "debugmalloc.h"


void menuMegjelenites(){
        printf("\n");
        printf("(1) Autó felvétele\n");
        printf("(2) Autó törlése\n");
        printf("(3) Keresés rendszám alapján\n");
        printf("(4) Javítás felvétele\n");
        printf("(5) Szerviztörténet\n");
        printf("(6) Mûszaki vizsgák (esedékes / lejárt)\n");
        printf("(7) Összes autó kilistázása\n");
        printf("(x) Kilépés\n");
}
int menu(Bejegyzes* szerviz){
    char char_choice[1];
    char debug;
	int int_choice;
	char exit_choice;

    while(1){
        menuMegjelenites();
        printf("Választott menüpont száma: ");
        scanf("%s", char_choice);
		int_choice = atoi(char_choice);

        if((int)char_choice[0] == 120 || (int)char_choice[0] == 88){ // Ha x vagy X jön a bemenetre kilépek

            printf("Biztosan kilép? (I/N) ");
            scanf("%c", &debug);
            scanf("%c", &exit_choice);

            if(toupper(exit_choice) == 'I'){ // Ha N vagy bármi más a bemenet akkor megszakítja a törlést
                SzervizMentes(szerviz); // Elmentem fájlba a dolgokat
                return 0;
            }
        }
        system("cls");
        switch(int_choice){
            case 1: // Auto felvétele
                scanf("%c", &debug);
                szerviz = uj_auto_felvetele(szerviz);
                break;
            case 2: // Auto torlese
                scanf("%c", &debug);
                szerviz = bejegyzesTorlese(szerviz);
                break;
            case 3:
                scanf("%c", &debug);
                Auto* keresett_auto = kereses(szerviz);
                if(keresett_auto==NULL){
                    printf("Nincs ilyen auto\n");
                }
                else{
                    printf("Autó rendszáma: %s\n", keresett_auto->rendszam);
                    printf("Autó márkája: %s\n", keresett_auto->marka);
                    printf("Autó típusa: %s\n", keresett_auto->tipus);
                    printf("Mûszaki lejárati ideje: %s\n", keresett_auto->muszaki);
                    printf("Autó évjárata: %d\n", keresett_auto->evjarat);
                    printf("Tulaj neve: %s %s\n", keresett_auto->tulaj.vezeteknev, keresett_auto->tulaj.keresztnev);
                    printf("Tulaj telefonszáma: %s\n", keresett_auto->tulaj.telefonszam);
                }
                break;
            case 4:
                printf("Javitas felvetele\n");
                scanf("%c", &debug);
                JavitasHozzaadasa(szerviz);
                break;
            case 5:
                printf("Szerviztortenet\n");
                scanf("%c", &debug);
                JavitasokListazasa(szerviz);
                break;
            case 6:
                printf("Mûszaki vizsgák\n");
                hamarosanLejaroMuszakik(szerviz);
                break;
            case 7:
                printf("Autók a nyilvántartásban:\n");
                AutokListazasa(szerviz);
                break;
            default:
                break;
        }
    }

    return 0;
}
int main()
{
    #ifdef _WIN32
        SetConsoleCP(1250);
        SetConsoleOutputCP(1250);
    #endif

    Bejegyzes* szerviz = NULL;
    szerviz = szervizBeolvas(szerviz);
    menu(szerviz);


    return 0;
}
