#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "ANSI-color-codes.h"

void clearTela() {
    system("cls");
}

void printLogo() {
    printf(RED ">>==============================================================================<<\n");
    printf(RED "||" BLU " ####### ####### ## " CRESET "##   ##  ###### ##   ##  #####  ###    ##  ######  ####### " RED "||\n");
    printf(RED "||" BLU " ##      ##      ## " CRESET " ## ##  ##      ##   ## ##   ## ####   ## ##       ##      " RED "||\n");
    printf(RED "||" BLU " #####   #####   ## " CRESET "  ###   ##      ####### ####### ## ##  ## ##   ### #####   " RED "||\n");
    printf(RED "||" BLU " ##      ##      ## " CRESET " ## ##  ##      ##   ## ##   ## ##  ## ## ##    ## ##      " RED "||\n");
    printf(RED "||" BLU " ##      ####### ## " CRESET "##   ##  ###### ##   ## ##   ## ##   ####  ######  ####### " RED "||\n");
    printf(RED ">>==============================================================================<<\n" CRESET);
}

void printTela(int tela, char *opt) {
    switch (tela) {
        case 1:
            clearTela();
            printLogo();
            printf("L - Login    C - Cadastro    S - Sair\n");
    }
    *opt = toupper(getchar());
}

void navegacao(int *tela, char opt, bool *running, bool *auth, int *erro) {
    switch (*tela) {
        case 1:
            if (opt == 'L') {
                *tela = 2;
            } else if (opt == 'C') {
                *tela = 3;
            } else if (opt == 'S') {
                *running = false;
            } else {
                *erro = 1;
            }
    }
}