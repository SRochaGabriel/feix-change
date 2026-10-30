#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "ANSI-color-codes.h"
#include "usuario.h"
#include "arquivos.h"

void printErro(char *err_detail) {
    char err_msg[200] = RED "ERRO: " CRESET;
    
    strcat(err_msg, err_detail);
    fprintf(stderr, err_msg);
    printf("\n");
}

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

void printTela(int tela, Usuario *user) {
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
        case 3:
            clearTela();
            printf(BLU "##############################" YEL " MINHA CARTEIRA " BLU "##############################\n" CRESET);
            exibeSaldo(user);
            printf("\nV - Voltar\n");
            break;
        case 4:
            clearTela();
            printf(BLU "##############################" YEL " EXTRATO " BLU "##############################\n" CRESET);
            exibeExtrato(user);
            printf("\nV - Voltar\n");
            break;
    }
}

void navegacao(int *tela, char opt, int *running, int *auth, int *erro, Usuario *user) {
    int result;

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
                } else if (opt == 'C') {
                    result = matchSenha(getUsrSenha(user));
                    while(getchar() != '\n');
                    
                    if (result == 1) {
                        *tela = 3; // tela de saldo
                    } else if (result == 0) {
                        printErro("Senha invalida. Tente novamente.\n");
                        sleep(1.5);
                    } else if (result == 2) {
                        *tela = 2;
                    }
                } else if (opt == 'D') {
                    if (deposito(user) == 1) {
                        *tela = 3;
                    }
                } else if (opt == 'R') {
                    if (saque(user) == 1) {
                        *tela = 3;
                    }
                } else if (opt == 'E') {
                    result = matchSenha(getUsrSenha(user));
                    while(getchar() != '\n');
                    
                    if (result == 1) {
                        *tela = 4; // tela de extrato
                    } else if (result == 0) {
                        printErro("Senha invalida. Tente novamente.\n");
                        sleep(1.5);
                    } else if (result == 2) {
                        *tela = 2;
                    }
                } else if (opt == 'I') {
                    result = matchSenha(getUsrSenha(user));
                    while(getchar() != '\n');
                    
                    if (result == 1) {
                        compraCripto(user);
                    }
                } else if (opt == 'V') {
                    vendeCripto(user);
                } else {
                    *erro = 1;
                }
                break;
            case 3:
            case 4:
                if (opt == 'V') {
                    *tela = 2;
                } else {
                    *erro = 1;
                }
                break;
        }
    }
}