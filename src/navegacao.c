#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "ANSI-color-codes.h"
#include "usuario.h"
#include "arquivos.h"

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

void printTela(int tela) {
    switch (tela) {
        case 1:
            clearTela();
            printLogo();
            printf("\nL - Login   C - Cadastro    S - Sair\n");
            break;
        case 2:
            clearTela();
            printLogo();
            exibeCotacoes();
            printf("\nC - Saldo    E - Extrato    D - Depositar    R - Saque    I - Comprar cripto    V - Vender cripto    A - Atualizar cotacao    S - Sair\n");
            break;
    }
}

void navegacao(int *tela, char opt, int *running, int *auth, int *erro, Usuario *user) {
    if (opt == 'S') {
        if (*auth) {
            *auth = 0;
            *tela = 1;
            logoff(user);
        } else {
            *running = 0;
        }
    } else if (running) {
        switch (*tela) {
            case 1:
                if (opt == 'L') {
                    if (login(auth, user) == 1) {
                        *tela = 2;
                    }
                } else if (opt == 'C') {
                    cadastrar();
                } else {
                    *erro = 1;
                }
                break;
            case 2:
                if (opt == 'A') {
                    updateCotacoes();
                }
                break;
        }
    }
}

void printErro(char *err_detail) {
    char err_msg[200] = RED "ERRO: " CRESET;
    
    strcat(err_msg, err_detail);
    fprintf(stderr, err_msg);
    printf("\n");
}