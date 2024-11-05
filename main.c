#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif


struct Case {  //Représente une case de la grille de jeu
    char value;
    char couleur;
    char background;
};

struct Bateau { //Représente un bateau mais sert UNIQUEMENT lors du placement, pas lors du jeu (pour l'instant)
    int taille;
    int id;
    int x;
    int y;
    char orientation;
};


struct Case* init (int Height, int Width);
void mise_en_place_bateaux (struct Case*, int);
void affiche_grille (struct Case*, int, int);
void placerBateau (struct Case* tableau, struct Bateau bateau, int Width );
void ecrire(struct Case);
struct Bateau demander_coordonnees(int);
char tirer(struct Case*, int, int, int);
int verifier_placement_horizontal(struct Case*, int, int ,int);
int verifier_placement_vertical(struct Case*, int, int ,int);
struct Case regarder_case(struct Case*, int, int, int);




int main(void) {


#ifdef _WIN32 //On s'assure qu'il sera possible d'imprimer les caractères qu'on veut utiliser
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif


    const int Height = 10;
    const int Width = 10;
    struct Case* grille_joueur_1 = init(Height, Width);
    affiche_grille(grille_joueur_1,Height,Width);
    printf("Au premier joueur de placer ses bateaux\n");
    mise_en_place_bateaux(grille_joueur_1,Width);

    struct Case* grille_joueur_2 = init(Height, Width);
    printf("Au second joueur de placer ses bateaux\n");
    mise_en_place_bateaux(grille_joueur_2,Width);

    tirer(grille_joueur_1, 2, 3, Width);
    affiche_grille(grille_joueur_1, Height, Width);
    tirer(grille_joueur_1, 3, 3, Width);
    affiche_grille(grille_joueur_1, Height, Width);
    tirer(grille_joueur_1,1,3,Width);
    affiche_grille(grille_joueur_1,Height,Width);

    free(grille_joueur_1);
    free(grille_joueur_2);
    return 0;
}

struct Case* init (int Height, int Width){ // Sert à initialiser une nouvelle grille vide pour le jeu



    struct Case* tableau =(struct Case*)malloc(sizeof(struct Case)*(Height*Width)); // On alloue un tableau linéarisé pour un rectangle
    if (tableau == NULL) {
        printf("Pas assez de mémoire pour l'allocation, fin du programme\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < Height * Width; i++){ // On remplis le tableau de ~ bleu sur fond normal représentant l'eau
        tableau [i].value = '~';
        tableau [i].background = ' ';
        tableau [i].couleur = 'b';
    }

    return tableau;

}

void mise_en_place_bateaux(struct Case* tableau, int Width){ //Sert à faire placer ses bateaux à un joueur

    for (int i = 5; i>0 ; i--){ // on fait placer 5 bateaux de taille décroissante

        placerBateau(tableau,demander_coordonnees(i),Width);

        affiche_grille(tableau,10,10);

    }


}

void affiche_grille(struct Case* tableau, int Height, int Width) {
#ifdef _WIN32
    printf("\033[2J\033[H "); //On efface tout le terminal et on replace le curseur au début
    for (int k = 1; k< Width + 1; k++) {printf(" %d",k);} // On affiche les nombres indicants les colonnes
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
            ecrire(tableau[i * Width + j]); // Le contenu de la case
            printf("%c", 186);  // ║
        }
        printf("\n");
    }
    printf(" %c", 200);  // ╚
    for (int j = 1; j < Width; j++) {
        printf("%c%c", 205, 202);  // ═╩
    }
    printf("%c%c\n", 205, 188);  // ═╝

#else





    printf("\033[2J\033[H");


    for (int k = 1; k < Width + 1; k++) {
        printf(" %d", k);
    }


    printf("\n ┌");
    for (int j = 1; j < Width; j++) {
        printf("─┬");
    }
    printf("─┐\n");

    // Grid content
    for (int i = 0; i < Height; i++) {
        if (i != 0) {
            printf(" ├");
            for(int j = 1; j < Width; j++) {
                printf("─┼");
            }
            printf("─┤\n");
        }

        printf("%c│", (65 + i));
        for (int j = 0; j < Width; j++) {
            ecrire(tableau[i * Width + j]);
            printf("│");
        }
        printf("\n");
    }


    printf(" └");
    for (int j = 1; j < Width; j++) {
        printf("─┴");
    }
    printf("─┘\n");
#endif
}


void placerBateau (struct Case* tableau, struct Bateau bateau, const int Width ){  // Cette fonction teste si un bateau peut être placé ou non selon les règles, le place si possible et redemande au joueur sinon.

    if (bateau.orientation == 'h') {
        if (verifier_placement_horizontal(tableau,bateau.taille,bateau.x,bateau.y) == 1){
            for (int i = 0; i < bateau.taille; i++) {
                int position = (bateau.y - 1) * Width + bateau.x - 1 + i;       // Décalage de 1 par choix (grille de 1 à 10 et non de 0 à 9)
                tableau[position].couleur = 'r';
                char valeur[20];
                sprintf((char *) valeur, "%d",bateau.id);
                tableau[position].value = valeur[0];
            }

        } else {
            struct Bateau nouvelEssai = demander_coordonnees(bateau.taille);        // Gestion des erreurs d'entrées de l'utilisateur
            placerBateau(tableau,nouvelEssai,Width);
        }

    } else if(bateau.orientation == 'v') {      //Pareil qu'au dessus mais quand l'orientation est verticale

        if (verifier_placement_vertical(tableau,bateau.taille,bateau.x,bateau.y) == 1) {
            for (int i = 0; i < bateau.taille; i++) {
                int position = (bateau.y - 1 - i) * Width + bateau.x - 1;
                tableau[position].couleur = 'r';
                char value[20];
                sprintf((char *) value, "%d", bateau.id);
                tableau[position].value = value[0];
            }
        }
        else {
            struct Bateau nouvelEssai = demander_coordonnees(bateau.taille);
            placerBateau(tableau,nouvelEssai,Width);
        }


    } else {
        printf("L'orientation n'est pas correcte (ecrire h ou v en minuscule)\n");      // Gestion des erreurs d'entrées de l'utilisateur
        struct Bateau nouvelEssai = demander_coordonnees(bateau.taille);
        placerBateau(tableau, nouvelEssai, Width);

    }


}





void ecrire(const struct Case c) { //Cette fonction permet d'écrire les caractères de la grille avec une couleur de texte et une couleur de fond à l'aide des séquences d'échappement ANSI
    //Couleur de fond
    const char* bg_code;
    switch (c.background) {
        case 'r': bg_code = "\033[0;41m"; break; //rouge
        case 'g': bg_code = "\033[0;42m"; break; //vert
        case 'y': bg_code = "\033[0;43m"; break; //jaune
        case 'b': bg_code = "\033[0;44m"; break; //bleu
        case 'm': bg_code = "\033[0;45m"; break; //magenta
        case 'c': bg_code = "\033[0;46m"; break; //cyan
        case 'w': bg_code = "\033[0;47m"; break; //blanc
        default:  bg_code = "\033[0;40m"; break; //noir
    }

    //Couleur du texte
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

    //Print les caractères et remise à zéro
    printf("%s%s%c\033[0m", bg_code, text_code, c.value);
}

struct Bateau demander_coordonnees(int taille) { // Cette fonction demande au joueur de donner des coordonnées pour un bateau d'une taille donnée et son orientation
    printf("%s %d %s","Placez votre bateau de taille ", taille," avec son y, son x et son orientation (h ou v)" );
    struct Bateau barque = {taille,taille, };
    int X;
    char Y;

    scanf(" %c", &Y);
    scanf(" %d", &X);
    barque.x = X;
    barque.y = (int)Y - 64;

    scanf(" %c", &(barque.orientation));
    return barque;
}


char tirer(struct Case* tableau,int y, int x,int Width){                // Fonction determinant si le tir touche, plouf ou coule le bateau (et modifie la grille en conséquence)
    int position = (y-1) * Width + (x-1);
    char resultat;
    if (position < Width * Width && !(x <= 0 || x > 10 || y <= 0 || y > 10)){   // Verifie que le tir est dans la grille

        if (tableau[position].value == '~'){
            tableau[position].value = 'x';
            tableau[position].couleur = 'w';
            printf("Plouf");
            resultat ='p';
        }
        else{

            int compteur = 0;
        for (int i = 0; i < Width * Width; i++) {               //permet de déterminer si il reste des cases sur lesquelles le bateau n'est pas encore touché
                if (tableau[i].value == tableau[position].value) {
                    compteur += 1;
                }
            }
            if (compteur == 1) {
                printf("Coule");
                resultat = 'c';
            } else {
                printf("Touche");
                resultat = 't';
            }

            tableau[position].value = 'x';
            tableau[position].couleur = 'r';




        }
    }
    else {
        printf("Hors de la grille");
        resultat ='g';
    }
    return resultat;
}


int verifier_placement_horizontal(struct Case* tableau,int taille, int x, int y) { // Cette fonction vérifie si le placement du bateau demandé par l'utilisateur est autorisé
    int resultat = 1; // Un resultat de 1 signifie que le bateau est plaçable, 0 sinon
    if (x <= 0 || x > (11 - taille) || y <= 0 || y > 10) { // On vérifie que le bateau peut rentrer dans la grille
        printf("Le bateau ne rentre pas dans la grille ! \n");
        resultat = 0;
    } else {

        for (int i = 0; i < taille; i++) { // On parcoure la zone supposée remplie par le bateau
            if (regarder_case(tableau,x + i,y,10).couleur == 'r') {
                printf("%s %c %d \n","Le bateau en chevauche un autre en: ",(char)(y+64),x+i);
                resultat = 0;
            }
        }
        // On s'intéresse maintenant à si le bateau est adjacent ou non à un autre
        int Xmin = -1;
        int Xmax = taille;
        if (x == 1) { Xmin = 0;} // dans le cas ou le bateau est collé au bord gauche de la grille, on ne regarde pas les cases à sa gauche puisqu'elles n'existent pas
        else if (x == 10 - (taille - 1)) { Xmax = taille - 1;} //pareil à droite

        int Ymin = -1;
        int Ymax = 1;
        if (y == 1){Ymin =0;} // Si le bateau est à la première ligne on ne regarde pas les cases du dessus
        else if (y == 10) { Ymax = 0;} // pareil tout en bas

        for (int i = Xmin; i <= Xmax; i++) {  // On parcoure les cases adjacentes au bateau pour voir si un autre bateau s'y trouve déjà, un bateau étant le seul élément ayant une couleur rouge dans le jeu
            for (int j = Ymin; j <= Ymax; j++) {
                if (regarder_case(tableau,x + i,y + j,10).couleur == 'r') { // Si la case contient un bateau alors on ne peut pas le placer
                    printf("Le bateau est trop proche d'un autre \n");
                    resultat = 0;
                }

            }
        }



    }

    return resultat;
}

int verifier_placement_vertical(struct Case* tableau,int taille, int x, int y) { // Cette fonction à la même utilité que verifier_placement_horizontal mais pour un bateau vertical et son fonctionnement est similaire
    int resultat = 1;
    if (x <= 0 || x > 10 || y < taille || y > 10) {
        printf("Le bateau ne rentre pas dans la grille");
        resultat = 0;
    }

    else {
        for (int i = 0; i < taille; i++) {
            if (regarder_case(tableau,x,y - i,10).couleur == 'r') {
                printf("%s %c %d \n","Le bateau en chevauche un autre en : ",(char)(y + 64 - i), x);
                resultat = 0;

            }
        }

        int Xmin = -1;
        int Xmax;
        switch (x) {
            case 1:Xmin = 0; break;
            case 10:Xmax = 0; break;
            default:Xmax = 1; break;
        }

        int Ymin = -1;
        if (y == 1){Ymin =0;}
        int Ymax = taille;
        if (y == 10){Ymax = taille - 1;}
        for (int i = Xmin; i <= Xmax; i++) {
            for (int j = Ymin; j <= Ymax; j++) {
                if (regarder_case(tableau,x + i,y - j,10).couleur == 'r') {
                    printf("Le bateau est trop proche d'un autre");
                    resultat = 0;
                }
            }
        }

    }
    return resultat;
}

struct Case regarder_case(struct Case* tableau, int x, int y, int tailleGrille){
    int position = (y - 1) * tailleGrille + (x - 1); // On applique un décalage de 1 car la grille est considérée dans le jeu comme allant de 1 à 10
    struct Case resultat;
    if (position > tailleGrille * tailleGrille || x - 1 > tailleGrille || y - 1 > tailleGrille || x -1 < 0 || y - 1 < 0) { // On vérifie si la position demandée est bien dans la grille
        resultat = (struct Case){'a','a','a'}; // dans ce cas on renvoie une case qui ne peut exister au sein de la grille de jeu pour signifier l'accès hors grille
    } else {
        resultat = tableau[position]; // On renvoie sinon la valeur demandée
    }

    return resultat;
}