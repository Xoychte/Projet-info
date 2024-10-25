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
void affiche_grille (struct Case*, int, int);
int placerBateau (struct Case* tableau, struct Bateau barque, int Width );
void ecrire(struct Case);
struct Bateau demander_coordonnees(int);
char touche(struct Case*, int, int, int);
int verifier_placement_horizontal(struct Case*, int, int ,int);
struct Case regarder_case(struct Case*, int, int, int);




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

void affiche_grille(struct Case* tableau, int Height, int Width) {
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

int placerBateau (struct Case* tableau, struct Bateau barque, int Width ){  //depassement en dehors de la grille
    int taille = barque.taille;

    /*
    if (barque.orientation == 'h'){

        for (int i = 0; i < barque.taille; i++) {

            if (regarder_case(tableau,barque.x,barque.y,Width) != '~') {
                printf("Le bateau chevauche un autre bateau \n");

                struct Bateau nouvelEssai;
                nouvelEssai = demander_coordonnees(taille);
                placerBateau(tableau, nouvelEssai, Width);
                return 0;
            }
        }

        for (int i = -1; i < (barque.taille + 1); i++) {
            for (int j = -1 ; j < 2; j++){
                int position = (barque.y - 1 + j) * Width + barque.x - 1 + i;
                if (tableau[position].value != '~') {

                    printf("Le bateau est trop proche d'un autre \n");

                    struct Bateau nouvelEssai;
                    nouvelEssai = demander_coordonnees(taille);
                    placerBateau(tableau, nouvelEssai, Width);
                    return 0;

                }


            }

        }


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
        */

    if (barque.orientation == 'h') {
        printf("%d %d \n", barque.x, barque.y);
        if (verifier_placement_horizontal(tableau,taille,barque.x,barque.y) == 1){
            for (int i = 0; i < barque.taille; i++) {
                int position = (barque.y - 1) * Width + barque.x - 1 + i;
                tableau[position].couleur = 'r';
                char valeur[20];
                sprintf((char *) valeur, "%d",barque.id);
                tableau[position].value = valeur[0];
            }

    } else {
        struct Bateau nouvelEssai = demander_coordonnees(taille);
        placerBateau(tableau,nouvelEssai,Width);
    }

    } else if(barque.orientation == 'v') {

        for (int i = 0; i < barque.taille; i++) {
            int position = (barque.y - 1 - i) * Width + barque.x - 1;

            if (tableau[position].value != '~') {
                printf("Le bateau chevauche un autre bateau \n");

                struct Bateau nouvelEssai;
                nouvelEssai = demander_coordonnees(taille);
                placerBateau(tableau, nouvelEssai, Width);
                return 0;
            }
        }


        for (int i = -1; i < (barque.taille + 1); i++) {
            for (int j = -1 ; j < 2; j++){
                int position = (barque.y - 1 - i) * Width + barque.x - 1 + j;
                if (tableau[position].value != '~') {

                    printf("Le bateau est trop proche d'un autre \n");

                    struct Bateau nouvelEssai;
                    nouvelEssai = demander_coordonnees(taille);
                    placerBateau(tableau, nouvelEssai, Width);
                    return 0;

                }


            }

        }


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
    // Background color
    const char* bg_code;
    switch (c.background) {
        case 'r': bg_code = "\033[0;41m"; break;
        case 'g': bg_code = "\033[0;42m"; break;
        case 'y': bg_code = "\033[0;43m"; break;
        case 'b': bg_code = "\033[0;44m"; break;
        case 'm': bg_code = "\033[0;45m"; break;
        case 'c': bg_code = "\033[0;46m"; break;
        case 'w': bg_code = "\033[0;47m"; break;
        default:  bg_code = "\033[0;40m"; break;
    }

    // Text color
    const char* text_code;
    switch (c.couleur) {
        case 'r': text_code = "\033[0;31m"; break;
        case 'g': text_code = "\033[0;32m"; break;
        case 'y': text_code = "\033[0;33m"; break;
        case 'b': text_code = "\033[0;34m"; break;
        case 'm': text_code = "\033[0;35m"; break;
        case 'c': text_code = "\033[0;36m"; break;
        case 'w': text_code = "\033[0;37m"; break;
        default:  text_code = "\033[0;30m"; break;
    }

    // Print character with colors and reset
    printf("%s%s%c\033[0m", bg_code, text_code, c.value);
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

int verifier_placement_horizontal(struct Case* tableau,int taille, int x, int y) {
    int resultat = 1;
    if (x <= 0 || x > (10 - taille) || y <= 0 || y > 10) {
        printf("Le bateau ne rentre pas dans la grille !");
        resultat = 0;
    } else {

        for (int i = 0; i < taille; i++) {
            if (regarder_case(tableau,x + i,y,10).couleur == 'r') {
                printf("%s %c %d \n","Le bateau en chevauche un autre en: ",(char)(y+65),x+1);
                resultat = 0;
            }
        }
        int Xmin = -1;
        int Xmax = x + taille - 1;
        if (x == 1) { Xmin = 0;}
        else if (x == 10 - (taille - 1)) { Xmax = x + taille - 2;}

        int Ymin = -1;
        int Ymax = 1;
        if (y == 1){Ymin =0;}
        else if (y == 9) { Ymax = 0;}

        for (int i = Xmin; i <= Xmax; i++) {
            for (int j = Ymin; j <= Ymax; j++) {
                if (regarder_case(tableau,x + i,y + j,10).value != '~') {
                    printf("Le bateau est trop proche d'un autre \n");
                    resultat = 0;
                }

            }
        }



    }

    return resultat;
}



struct Case regarder_case(struct Case* tableau, int x, int y, int tailleGrille){
    int position = (y - 1) * tailleGrille + (x - 1);
    struct Case resultat;
    if (position > tailleGrille * tailleGrille) {
        resultat = (struct Case){'a','a','a'};
    } else {
        resultat = tableau[position];
    }

    return resultat;
}