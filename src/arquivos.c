#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
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

void insertNovoUsuario() {}