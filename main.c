#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif



struct Case {
    char value;
    char couleur;
    char background;
};

struct Bateau {
    int taille;
    int id;
    int x;
    int y;
    char orientation;
};


struct Case* init (int Height, int Width);
struct Case* position (struct Case*, int);
struct Case* affiche_grille (struct Case*, int, int);
int placerBateau (struct Case* tableau, struct Bateau barque, int Width );
void ecrire(struct Case);
struct Bateau demander_coordonnees(int);
char touche(struct Case*, int, int, int);






int main(void) {


    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif


    const int Height = 10;
    const int Width = 10;
    struct Case* tableau = init(Height, Width);
    position(tableau,Width);
    affiche_grille(tableau, Height, Width);

    printf("%c\n",touche(tableau, 2, 2, Width));
    affiche_grille(tableau, Height, Width);
    touche(tableau, 3, 3, Width);
    affiche_grille(tableau, Height, Width);

    free(tableau);
    return 0;
}

struct Case* init (int Height, int Width){



    struct Case* tableau =(struct Case*)malloc(sizeof(struct Case)*(Height*Width));
    if (tableau == NULL) {
        printf("PROBLEME\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < Height * Width; i++){
        tableau [i].value = '~';
        tableau [i].background = ' ';
        tableau [i].couleur = 'b';
    }


    affiche_grille(tableau, Height, Width);

    return tableau;

}

struct Case* position(struct Case* tableau, int Width){

    for (int i = 5; i>0 ; i--){

        placerBateau(tableau,demander_coordonnees(i),Width);

        affiche_grille(tableau,10,10);

    }

    return tableau;

}

struct Case* affiche_grille(struct Case* tableau, int Height, int Width) {
    printf("\033[2J\033[H ");
    for (int k = 1; k< Width + 1; k++) {printf(" %d",k);}
    printf("\n %c", 201);  // ╔
    for (int j = 1; j < Width; j++) {
        printf("%c%c", 205, 203);  // ═╦
    }
    printf("%c%c\n", 205, 187);  // ═╗

    for (int i = 0; i < Height; i++) {
        if (i != 0) {
            printf(" %c", 204);  // ╠
            for(int j = 1; j < Width; j++) {
                printf("%c%c", 205, 206);  // ═╬
            }
            printf("%c%c\n", 205, 185);  // ═╣
        }
        printf("%c%c",(65 + i), 186); // ║
        for (int j = 0; j < Width; j++) {
            ecrire(tableau[i * Width + j]);
            printf("%c", 186);  // ║
        }
        printf("\n");
    }
    printf(" %c", 200);  // ╚
    for (int j = 1; j < Width; j++) {
        printf("%c%c", 205, 202);  // ═╩
    }
    printf("%c%c\n", 205, 188);  // ═╝
}

int placerBateau (struct Case* tableau, struct Bateau barque, int Width ){
    int taille = barque.taille;

    if (barque.orientation == 'h'){
        if(barque.x > 0 && barque.x <= (10 - barque.taille) && barque.y > 0 && barque.y <= 10) {
            for (int i = 0; i < barque.taille; i++) {
                int position = (barque.y - 1) * Width + barque.x -1 + i;
                tableau[position].couleur = 'r';
                char value[20];
                sprintf((char *) value, "%d", barque.id);
                tableau[position].value = value[0];
            }

        }
        else {

            printf("Le bateau ne rentre pas dans la grille\n");
            struct Bateau nouvelEssai;
            nouvelEssai = demander_coordonnees(taille);
            placerBateau(tableau, nouvelEssai, Width);

        }

    } else if(barque.orientation == 'v') {
        if (barque.x > 0 && barque.x <= 10 && (barque.y - barque.taille)> 0  && barque.y <= 10) {
            for (int i = 0; i < barque.taille; i++) {
                int position  = (barque.y - 1 - i) * Width + barque.x - 1;
                tableau[position].couleur = 'r';
                char value[20];
                sprintf((char *) value, "%d", barque.id);
                tableau[position].value = value[0];
            }

        } else {
            printf("Le bateau ne rentre pas dans la grille\n");
            struct Bateau nouvelEssai;
            nouvelEssai = demander_coordonnees(taille);
            placerBateau(tableau, nouvelEssai, Width);

        }




    } else {
        printf("L'orientation n'est pas correcte (ecrire h ou v en minuscule)\n");
        struct Bateau nouvelEssai;
        nouvelEssai = demander_coordonnees(taille);
        placerBateau(tableau, nouvelEssai, Width);

    }


}





void ecrire(struct Case c) {

    switch (c.background) {
        case 'r':printf("\033[0;41m");break;
        case 'g':printf("\033[0;42m");break;
        case 'y':printf("\033[0;43m");break;
        case 'b':printf("\033[0;44m");break;
        case 'm':printf("\033[0;45m");break;
        case 'c':printf("\033[0;46m");break;
        case 'w':printf("\033[0;47m");break;
        default:printf("\033[0;40m");break;
    }

    switch (c.couleur) {
        case 'r':printf("\033[0;31m");break;
        case 'g':printf("\033[0;32m");break;
        case 'y':printf("\033[0;33m");break;
        case 'b':printf("\033[0;34m");break;
        case 'm':printf("\033[0;35m");break;
        case 'c':printf("\033[0;36m");break;
        case 'w':printf("\033[0;37m");break;
        default:printf("\033[0;30m");break;
    }

    printf("%c", c.value);
    printf("\033[0m");
}

struct Bateau demander_coordonnees(int taille) {
    printf("%s %d %s","Placez votre bateau de taille ", taille," avec son x, y et son orientation (h ou v)" );
    struct Bateau barque = {taille,taille, };
    char Y;
    scanf(" %c", &Y);
    scanf("%d", &(barque.x));

    barque.y = (int)Y - 64;

    scanf(" %c", &(barque.orientation));
    return barque;
}


char touche(struct Case* tableau,int y, int x,int Width){
    int position = (y-1) * Width + (x-1);
    if (tableau[position].value == '~'){
        tableau[position].value = 'x';
        tableau[position].couleur = 'w';
        return ('f');
    }
    else{

        tableau[position].value = 'x';
        tableau[position].couleur = 'r';
        return ('t');
    }

}