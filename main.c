#include <stdio.h>
#include <stdlib.h>



char* init (int Height, int Width);
char* bateau (char*,int, int);
char* affiche (char*, int, int);




int main(void) {
    const int Height = 9;
    const int Width = 9;
    char* tableau = init(Height, Width);
    bateau(tableau,Height, Width);

    free(tableau);
    return 0;
}

char* init (int Height, int Width){



    char* tableau =(char*)malloc(sizeof(char)*(Height*Width));
    if (tableau == NULL) {
        printf("PROBLEME\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < Height * Width; i++){
        tableau [i] = 'A';
    }


    affiche(tableau, Height, Width);

    return tableau;

}

char* bateau(char* tableau, int Height, int Width){
    int xBateau;
    int yBateau;

    scanf("%d", &xBateau);
    scanf("%d", &yBateau );
    tableau[(yBateau - 1) * Width + xBateau - 1] = 'B';

    affiche(tableau, Height, Width);
    return tableau;

}

char* affiche (char* tableau, int Height, int Width){
    for (int i = 0; i < Height; i++){
        for (int j = 0; j < Width; j++){
            printf("%c", tableau[i * Width + j]);

        }
        printf("\n");

    }}