#include <stdio.h>
#include <stdbool.h>
#include "tela.h"
#include "arquivos.h"

int main() {
    int tela = 1;
    char opt = '~';
    bool auth = false;
    bool running = true;
    
    // inicializacao de ambiente
    initDirData();
    initDataFiles();

    while (running) {
        if (!auth) {
            printTela(tela, &opt);
            if (opt == 'S') {
                running = false;
            }
        }
    }

    return 0;
}