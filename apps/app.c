#include <stdio.h>
#include <stdbool.h>
#include "navegacao.h"
#include "arquivos.h"

int main() {
    int tela = 1;
    char opt = '~';
    bool erro = false;
    bool auth = false;
    bool running = true;
    
    // inicializacao de ambiente
    initDirData();
    initDataFiles();

    while (running) {
        if (!auth) {
            printTela(tela);
            if (erro) {
                printErro("Opcao selecionada invalida. Tente novamente.");
            }
            opt = getOpt();
            navegacao(&tela, opt, &running, &auth, &erro);
        }
    }

    return 0;
}