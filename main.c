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



struct Case* init (int Height, int Width);
struct Case* bateau (struct Case*,int, int);
struct Case* affiche (struct Case*, int, int);
void ecrire(struct Case);




int main(void) {


    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif


    const int Height = 9;
    const int Width = 9;
    struct Case* tableau = init(Height, Width);
    bateau(tableau,Height, Width);

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
        tableau [i].value = 'A';
        tableau [i].background = 'r';
        tableau [i].couleur = 'B';
    }


    affiche(tableau, Height, Width);

    return tableau;

}

struct Case* bateau(struct Case* tableau, int Height, int Width){
    int xBateau;
    int yBateau;

    scanf("%d", &xBateau);
    scanf("%d", &yBateau );
    tableau[(yBateau - 1) * Width + xBateau - 1].background = 'b';

    affiche(tableau, Height, Width);
    return tableau;

}

struct Case* affiche (struct Case* tableau, int Height, int Width){
    for (int i = 0; i < Height; i++){
        for (int j = 0; j < Width; j++){
            ecrire(tableau[i * Width + j]);

        }
        printf("\n");

    }}





void ecrire(struct Case c) {
    switch (c.couleur) {
        case 'r':printf("\033[0;31m");break;
        case 'g':printf("\033[0;32m");break;
        case 'y':printf("\033[0;33m");break;
        case 'b':printf("\033[0;34m");break;
        case 'm':printf("\033[0;35m");break;
        case 'c':printf("\033[0;36m");break;
        case 'w':printf("\033[0;37m");break;
        case 'B':printf("\033[0;30m");break;
        default:printf("\033[0;30m");break;
    }
    switch (c.background) {
        case 'r':printf("\033[0;41m");break;
        case 'g':printf("\033[0;42m");break;
        case 'y':printf("\033[0;43m");break;
        case 'b':printf("\033[0;44m");break;
        case 'm':printf("\033[0;45m");break;
        case 'c':printf("\033[0;46m");break;
        case 'w':printf("\033[0;47m");break;
        case 'B':printf("\033[0;40m");break;
        default:printf("\033[0;40m");break;
    }
    printf("%c", c.value);
    printf("\033[0m");
}