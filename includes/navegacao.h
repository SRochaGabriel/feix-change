#include "usuario.h"

void printTela(int tela, Usuario *user);
void navegacao(int *tela, char opt, int *running, int *auth, int *erro, Usuario *user);
void printErro(char *err_detail);
char getOpt();