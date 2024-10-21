#include <stdio.h>
#include <stdlib.h>
void init (void);




int main(void) {

    init();

    return 0;
}

void init (void){
    int Height;
    int Width;

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