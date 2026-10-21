#include <stdbool.h>

void printTela(int tela);
void navegacao(int *tela, char opt, bool *running, bool *auth, bool *erro);
void printErro(char *err_detail);
char getOpt();