#include <stdio.h>
#include <stdbool.h>
#include "tela.h"
#include "arquivos.h"

int main() {
    int tela = 1;
    int erro = 0;
    char opt = '~';
    bool auth = false;
    bool running = true;
    
    // inicializacao de ambiente
    initDirData();
    initDataFiles();

    while (running) {
        if (!auth) {
            printTela(tela);
            if (erro != 0) {
                printErro(erro);
            }
            opt = getOpt();
            navegacao(&tela, opt, &running, &auth, &erro);
        }
    }

    return 0;
}