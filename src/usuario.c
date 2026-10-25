#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "arquivos.h"
#include "navegacao.h"
#include "ANSI-color-codes.h"

typedef struct _usuario {
    char nome[101];
    char cpf[12];
    char senha[7];
} Usuario;

Usuario *inicializaUser() {
    static Usuario user = {0};

    return &user;
}

const char *getUsrNome(const Usuario *user) {
    return user->nome;
}

const char *getUsrCpf(const Usuario *user) {
    return user->cpf;
}

const char *getUsrSenha(const Usuario *user) {
    return user->senha;
}

void setUsr(Usuario *user, const char *nome, const char *cpf, const char *senha) {
    strcpy(user->nome, nome);
    strcpy(user->cpf, cpf);
    strcpy(user->senha, senha);
}

void cadastrar() {
    char nome[101];
    char cpf[20];
    char senha[10];
    char *cpr;
    int len;
    int erro = 0;
    int continua = 1;

    printf("Digite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");

    do {
        erro = 0;
        printf("Insira seu nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = 0;

        if (strcmp(nome, "SAIR") == 0) {
            continua = 0;
            break;
        }

        len = strlen(nome);
        cpr = strchr(nome, ';');
        
        if (len > 100) {
            printErro("Seu nome ultrapassa os limites de caracteres. Tente novamente.");
            erro = 1;
        } else if (cpr != NULL) {
            printErro("Informe seu nome sem utilizar caracteres especiais como ';'. Tente novamente.");
            erro = 1;
        }
        
    } while (erro == 1);
    
    if (continua) {
        do {
            erro = 0;
            printf("Informe seu CPF (apenas numeros): ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0;
            
            if (strcmp(cpf, "SAIR") == 0) {
                continua = 0;
                break;
            }
            
            len = strlen(cpf);

            if (len != 11) {
                printErro("O CPF deve ter 11 digitos. Tente novamente.\n");
                erro = 1;
            } else {
                for (int i = 0; i < len; i++) {
                    if (!isdigit(cpf[i])) {
                        printErro("Digite apenas os numeros do CPF. Tente novamente.\n");
                        erro = 1;
                        break;
                    }
                }
            }
        } while (erro == 1);
    }
    
    if (continua) {
        do {
            erro = 0;
            printf("Crie sua senha (6 numeros): ");
            fgets(senha, sizeof(senha), stdin);
            senha[strcspn(senha, "\n")] = 0;
            
            if (strcmp(senha, "SAIR") == 0) {
                continua = 0;
                break;
            }
            
            len = strlen(senha);
            
            if (len != 6) {
                printErro("Sua senha deve ter 6 digitos. Tente novamente.\n");
                erro = 1;
            } else {
                for (int i = 0; i < len; i++) {
                    if (!isdigit(senha[i])) {
                        printErro("Sua senha deve ter apenas numeros. Tente novamente.\n");
                        erro = 1;
                        break;
                    }
                }
            }
    
        } while (erro == 1);
    }

    if (continua) {
        continua = checkCpfUnique(cpf);
    }

    if (continua) {
        insertNovoUsuario(nome, cpf, senha);
        printf("Cadastrando usuario...");
        sleep(2);
        printf(GRN " Usuario cadastrado com sucesso!");
        sleep(1);
    }
}

int login(int *auth, Usuario *user) {
    char nome[150];
    char cpf[20];
    char senha[10];

    char *cpr;
    int len;
    int erro;
    int continua = 1;

    printf("Digite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");
    
    if (continua) {
        do {
            erro = 0;
            printf("Informe seu CPF (apenas numeros): ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0;
            
            if (strcmp(cpf, "SAIR") == 0) {
                continua = 0;
                break;
            }
            
            len = strlen(cpf);

            if (len != 11) {
                printErro("O CPF deve ter 11 digitos. Tente novamente.\n");
                erro = 1;
            } else {
                for (int i = 0; i < len; i++) {
                    if (!isdigit(cpf[i])) {
                        printErro("Digite apenas os numeros do CPF. Tente novamente.\n");
                        erro = 1;
                        break;
                    }
                }
            }
        } while (erro);
    }
    
    if (continua) {
        do {
            erro = 0;
            printf("Informe sua senha (6 numeros): ");
            fgets(senha, sizeof(senha), stdin);
            senha[strcspn(senha, "\n")] = 0;
            
            if (strcmp(senha, "SAIR") == 0) {
                continua = 0;
                break;
            }
            
            len = strlen(senha);
            
            if (len != 6) {
                printErro("Sua senha deve ter 6 digitos. Tente novamente.\n");
                erro = 1;
            } else {
                for (int i = 0; i < len; i++) {
                    if (!isdigit(senha[i])) {
                        printErro("Sua senha deve ter apenas numeros. Tente novamente.\n");
                        erro = 1;
                        break;
                    }
                }
            }
    
        } while (erro);
    }

    if (continua) {
        printf("Autenticando...");
        sleep(1);

        if (authUser(nome, cpf, senha) == 1) {
            setUsr(user, nome, cpf, senha);
            *auth = 1;
            printf(GRN " Bem-vindo!");
            sleep(1);
        }
    }

    return continua;
}

void logoff(Usuario *user) {
    memset(user, 0, sizeof(Usuario));
}