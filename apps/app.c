#include <stdio.h>
#include "navegacao.h"
#include "arquivos.h"
#include "usuario.h"

int main() {
    int tela = 1;
    int erro = 0;
    int auth = 0;
    int running = 1;
    char opt = '~';
    Usuario *user = inicializaUser();
    
    initDirData();
    initDataFiles();

    while (running == 1) {
        printTela(tela);
        if (erro) {
            printErro("Opcao selecionada invalida. Tente novamente.");
        }
        // updateCotacoes();
        opt = getOpt();
        navegacao(&tela, opt, &running, &auth, &erro, user);
    }

    return 0;
}