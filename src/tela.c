#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
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

char getOpt() {
    char opt = toupper(getchar());
    int i = 0;

    while(getchar() != '\n') {
        i++;
    }

    if (i > 0) {
        opt = '~';
    }

    return opt;
}

void printTela(int tela, char *opt) {
    switch (tela) {
        case 1:
            clearTela();
            printLogo();
            printf("L - Login    C - Cadastro    S - Sair\n");
    }
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

void printErro(int erro) {
    char *err_msg;

    switch (erro) {
        case 1:
            err_msg = RED "ERRO: " CRESET "Opcao selecionada invalida. Tente novamente.\n";
    }

    fprintf(stderr, err_msg);
}