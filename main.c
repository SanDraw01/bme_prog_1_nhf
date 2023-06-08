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
        printf("(1) Aut� felv�tele\n");
        printf("(2) Aut� t�rl�se\n");
        printf("(3) Keres�s rendsz�m alapj�n\n");
        printf("(4) Jav�t�s felv�tele\n");
        printf("(5) Szervizt�rt�net\n");
        printf("(6) M�szaki vizsg�k (esed�kes / lej�rt)\n");
        printf("(7) �sszes aut� kilist�z�sa\n");
        printf("(x) Kil�p�s\n");
}
int menu(Bejegyzes* szerviz){
    char char_choice[1];
    char debug;
	int int_choice;
	char exit_choice;

    while(1){
        menuMegjelenites();
        printf("V�lasztott men�pont sz�ma: ");
        scanf("%s", char_choice);
		int_choice = atoi(char_choice);

        if((int)char_choice[0] == 120 || (int)char_choice[0] == 88){ // Ha x vagy X j�n a bemenetre kil�pek

            printf("Biztosan kil�p? (I/N) ");
            scanf("%c", &debug);
            scanf("%c", &exit_choice);

            if(toupper(exit_choice) == 'I'){ // Ha N vagy b�rmi m�s a bemenet akkor megszak�tja a t�rl�st
                SzervizMentes(szerviz); // Elmentem f�jlba a dolgokat
                return 0;
            }
        }
        system("cls");
        switch(int_choice){
            case 1: // Auto felv�tele
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
                    printf("Aut� rendsz�ma: %s\n", keresett_auto->rendszam);
                    printf("Aut� m�rk�ja: %s\n", keresett_auto->marka);
                    printf("Aut� t�pusa: %s\n", keresett_auto->tipus);
                    printf("M�szaki lej�rati ideje: %s\n", keresett_auto->muszaki);
                    printf("Aut� �vj�rata: %d\n", keresett_auto->evjarat);
                    printf("Tulaj neve: %s %s\n", keresett_auto->tulaj.vezeteknev, keresett_auto->tulaj.keresztnev);
                    printf("Tulaj telefonsz�ma: %s\n", keresett_auto->tulaj.telefonszam);
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
                printf("M�szaki vizsg�k\n");
                hamarosanLejaroMuszakik(szerviz);
                break;
            case 7:
                printf("Aut�k a nyilv�ntart�sban:\n");
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
