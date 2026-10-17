#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "ANSI-color-codes.h"

int initDirData() {
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
        printf(RED "ERRO: " CRESET "%s\n", strerror(errno));
    }

    return falha;
}

int initDataFiles () {
    struct _stat buffer;
    int falha = 0;
    char *file_erro;
    const char *arquivos[4] = {"data/transacao.txt", "data/usuario.txt", "data/carteira.txt", "data/cotacao.txt"};

    for (int i = 0; i < 4; i++) {
        if (_stat((char *)arquivos[i], &buffer) == -1) {
            FILE *f = fopen((char *)arquivos[i], "w");

            if (f == NULL) {
                falha = 1;
                strcpy(file_erro, (char *)arquivos[i]);
            }

            fclose(f);
        }
    }

    if (falha != 0) {
        printf(RED "ERRO: " CRESET "Não foi possível criar o arquivo %s. %s\n", file_erro, strerror(errno));
    }

    return falha;
}