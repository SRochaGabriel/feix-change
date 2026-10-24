#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include "navegacao.h"
#include "usuario.h"
#include "ANSI-color-codes.h"

void initCotacao(FILE *f) {
    char *data_str;
    char info[100];
    const char *cotacoes_ini[] = {"BRL;0.0;", "BTC;409672.99;", "ETH;11938.4;", "XRP;6.44;"};
    time_t data = time(NULL);

    snprintf(data_str, 100, "%ld", data);

    for (int i = 0; i < 4; i++) {
        strcpy(info, cotacoes_ini[i]);
        strcat(info, data_str);
        fprintf(f, "%s\n", info);
    }
}

void initDirData() {
    struct _stat buffer;
    const char *dir = "data";
    int falha = 0;
    
    if (_stat(dir, &buffer) == -1) {
        if (errno == 2) {
            if (mkdir(dir) == -1) {
                falha = 1;
            }
        } else {
            falha = 1;
        }
    } 

    if (falha != 0) {
        fprintf(stderr, RED "ERRO: " CRESET "%s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void initDataFiles () {
    struct _stat buffer;
    int falha = 0;
    char *file_erro;
    const char *arquivos[] = {"data/transacao.txt", "data/usuario.txt", "data/carteira.txt", "data/cotacao.txt"};

    for (int i = 0; i < 4; i++) {
        if (_stat((char *)arquivos[i], &buffer) == -1) {
            FILE *f = fopen((char *)arquivos[i], "w");

            if (f == NULL) {
                falha = 1;
                strcpy(file_erro, (char *)arquivos[i]);
            } else if (i == 3) {
                initCotacao(f);
            }

            fclose(f);
        }
    }

    if (falha != 0) {
        fprintf(stderr, RED "ERRO: " CRESET "Não foi possível criar o arquivo %s. %s\n", file_erro, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

float getVariacao() {
    srand(time(NULL));
    return (rand() / (float)RAND_MAX) + (rand() % 10 - 5);
}

void insertNovoUsuario(char *nome, char *cpf, char *senha) {
    char registro[200];
    int erro = 0;
    FILE *f = fopen("data/usuario.txt", "a");

    if (f == NULL) {
        erro = 1;
        printErro("Não foi possível inserir o usuário.");
        fclose(f);
    }

    if (!erro) {
        strcat(nome, ";");
        strcat(cpf, ";");

        strcpy(registro, nome);
        strcat(registro, cpf);
        strcat(registro, senha);

        fprintf(f, "%s\n", registro);
    }

    fclose(f);
}

int checkCpfUnique(char *cpf) {
    char cpf_compare[12];
    char linha[150];
    int result = 1;
    int pos = 0;
    FILE *f = fopen("data/usuario.txt", "r");

    if (f == NULL) {
        result = 0;
        printErro("Erro ao acessar base de dados.");
    } else {
        while(fgets(linha, sizeof(linha), f)) {    
            pos = strcspn(linha, ";");
            pos++;
            strncpy(cpf_compare, linha + pos, 11);

            if (strcmp(cpf_compare, cpf) == 0) {
                printErro("Informacoes de cadastro invalidas. Voce sera redirecionado para a tela inicial. Tente se cadastrar novamente.");
                sleep(2);
                result = 0;
                break;
            }
        }

        fclose(f);
    }


    return result;
}

int authUser(char *nome, char *cpf, char *senha) {
    char nome_compare[120];
    char cpf_compare[12];
    char senha_compare[7];
    char dados[19];
    char linha[150];

    int result = 0;
    int pos = 0;

    FILE *f = fopen("data/usuario.txt", "r");

    if (f == NULL) {
        printErro("Erro ao acessar base de dados.");
    } else {
        while(fgets(linha, sizeof(linha), f)) {    
            pos = strcspn(linha, ";");
            pos++;
            strncpy(nome_compare, linha, pos - 1);
            strncpy(dados, linha + pos, 18);
            
            pos = strcspn(dados, ";");
            pos++;
            strncpy(cpf_compare, dados, 11);
            strncpy(senha_compare, dados + pos, 7);
            
            if (strcmp(cpf_compare, cpf) == 0 && strcmp(senha_compare, senha) == 0) {
                result = 1;
                break;
            } else {
                memset(nome_compare, 0, sizeof(nome_compare));
            }
        }
        
        if (!result) {
            printErro("Informacoes de login invalidas. Tente novamente.");
            sleep(2);
        } else {
            strcpy(nome, nome_compare);
        }
        fclose(f);
    }

    return result;
}