#include <stdio.h>
#include <stdlib.h>

int Height;
int Width;

void init (void){
    printf("%s", "donnez la hauteur \n");
    scanf("%d", &Height);
    printf("%s", "donnez la largeur \n");
    scanf("%d", &Width);
    char* tableau =(char*)malloc(sizeof(char)*(Height*Width));
    if (tableau == NULL) {
        printf("PROBLEME\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < Height * Width; i++){
        tableau [i] = 'A';
    }


    for (int i = 0; i < Height; i++){
        for (int j = 0; j < Width; j++){
            printf("%c", tableau[i * Width + j]);

        }
        printf("\n");
    }



    free(tableau);


}


int main(void) {



    init();
    printf("Hauteur: %d, Largeur: %d\n", Height, Width);


    return 0;
}
