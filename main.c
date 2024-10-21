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
void ecrire(struct Case);


int main(void) {

    #ifdef _WIN32
    // Enable ANSI escape codes
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif


    printf("Hello, World!\n");
    struct Case Babz = {'X','r','y'};
    ecrire(Babz);
    return 0;

}

void ecrire(struct Case c) {
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
    printf("%c", c.value);
}