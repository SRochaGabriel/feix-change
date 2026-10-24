#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "ANSI-color-codes.h"
#include "usuario.h"

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

void navegacao(int *tela, char opt, int *running, int *auth, int *erro) {
    if (opt == 'S') {
        if (*auth) {
            *auth = 0;
        } else {
            *running = 0;
        }
    }

    if (running) {
        switch (*tela) {
            case 1:
                if (opt == 'L') {
                    login();
                } else if (opt == 'C') {
                    cadastrar();
                } else {
                    *erro = 1;
                }
        }
    }
}

void printErro(char *err_detail) {
    char err_msg[100] = RED "ERRO: " CRESET;
    
    strcat(err_msg, err_detail);
    fprintf(stderr, err_msg);
    printf("\n");
}