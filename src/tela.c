#include <stdio.h>
#include <stdlib.h>
#include "ANSI-color-codes.h"

void clearTela() {
    system("cls");
}

void main() {
    printf(RED ">>==============================================================================<<\n");
    printf(RED "||" BLU " ███████ ███████ ██ " CRESET "██   ██  ██████ ██   ██  █████  ███    ██  ██████  ███████ " RED "||\n");
    printf(RED "||" BLU " ██      ██      ██ " CRESET " ██ ██  ██      ██   ██ ██   ██ ████   ██ ██       ██      " RED "||\n");
    printf(RED "||" BLU " █████   █████   ██ " CRESET "  ███   ██      ███████ ███████ ██ ██  ██ ██   ███ █████   " RED "||\n");
    printf(RED "||" BLU " ██      ██      ██ " CRESET " ██ ██  ██      ██   ██ ██   ██ ██  ██ ██ ██    ██ ██      " RED "||\n");
    printf(RED "||" BLU " ██      ███████ ██ " CRESET "██   ██  ██████ ██   ██ ██   ██ ██   ████  ██████  ███████ " RED "||\n");
    printf(RED ">>==============================================================================<<\n" CRESET);
}