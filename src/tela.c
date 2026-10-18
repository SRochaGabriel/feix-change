#include <stdio.h>
#include <stdlib.h>
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

void printMenu(int tela) {
    switch (tela) {
        case 1:
            print("1 - Login    2 - Cadastro    3 - Sair");
    }
}