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

#define USR_PATH "data/usuario.txt"
#define COT_PATH "data/cotacao.txt"
#define CART_PATH "data/carteira.txt"
#define TRNS_PATH "data/transacao.txt"

int contaLinhas(char *filename) {
    FILE *f = fopen(filename, "r");
    char linha[150] = "";
    int erro = 0;
    int i = 0;

    if (f == NULL) {
        printErro("Não foi possível recuperar o extrato.");
        erro = 1;
    }

    if (erro == 0) {
        while (fgets(linha, sizeof(linha), f)) {
            i++;
        }
    }

    return i;
}

void initCotacao(FILE *f) {
    char data_str[20];
    char info[100];
    const char *cotacoes_ini[] = {"BTC;409672.99;", "ETH;11938.4;", "XRP;6.44;"};
    time_t data = time(NULL);

    snprintf(data_str, sizeof(data_str), "%ld", data);

    for (int i = 0; i < 4; i++) {
        strcpy(info, cotacoes_ini[i]);
        strcat(info, data_str);
        fprintf(f, "%s\n", info);
    }
}

float getVariacao() {
    srand(time(NULL));
    return (rand() / (float)RAND_MAX) + (rand() % 10 - 5);
}

void getCotacao(char *moeda, char *cotacao) {
    FILE *f = fopen(COT_PATH, "r");
    int erro = 0;
    int pos = 0;
    char moeda_nome[4] = "";
    char linha[200] = "";
    
    if (f == NULL) {
        printErro("Não foi possível recuperar dados de cotação de criptomoeda.");
        erro = 1;
    }

    if (erro == 0) {
        while (fgets(linha, sizeof(linha), f)) {
            strncpy(moeda_nome, linha, 3);

            if (strcmp(moeda_nome, moeda) == 0) {
                pos = strcspn(linha + 4, ";");
                strncpy(cotacao, linha + 4, pos);
            }
        }
        fclose(f);
    }
}

void updateCotacoes() {
    float variacao = getVariacao();
    double cotacao_val;
    int pos = 0;
    int erro = 0;
    int atualiza = 0;
    char nome[5] = "";
    char cotacao[20] = "";
    char ultimo_update[20] = "";
    char data[50];
    char linha[50];
    FILE *f = fopen(COT_PATH, "r");
    FILE *f2 = fopen("data/cotacaoUPDT.txt", "w");
    time_t ultimo_tempo;
    time_t agora = time(NULL);

    if (f == NULL || f2 == NULL) {
        printErro("Erro ao acessar base de dados.");
    } else {
        while(fgets(linha, sizeof(linha), f)) {    
            pos = strcspn(linha, ";");
            pos++;
            strncpy(nome, linha, pos);
            
            strncpy(data, linha + pos, 50);
            
            pos = strcspn(data, ";");
            pos++;
            strncpy(cotacao, data, pos);
            
            strncpy(ultimo_update, data + pos, 11);
            
            ultimo_tempo = (time_t)atoi(ultimo_update);
            
            if (difftime(agora, ultimo_tempo) > 86400) {
                atualiza = 1;
            } else {
                break;
            }
            
            if (atualiza) {
                cotacao_val = atof(cotacao);
                cotacao_val += (cotacao_val * variacao / 100);
                snprintf(cotacao, 20, "%.2lf", cotacao_val);
                strcat(cotacao, ";");

                snprintf(ultimo_update, 20, "%ld", agora);
                fprintf(f2, "%s%s%s\n", nome, cotacao, ultimo_update);
            }

            memset(cotacao, 0, sizeof(cotacao));
        }

        fclose(f);
        fclose(f2);
    }

    if (atualiza) {
        if (remove(COT_PATH) != 0) {
            erro = 1;
        }
    
        if (rename("data/cotacaoUPDT.txt", COT_PATH) != 0) {
            erro = 1;
        }
    
        if (erro == 1) {
            printErro("Não foi possível atualizar corretamente a cotação.");
            exit(EXIT_FAILURE);
        }
    } else {
        remove("data/cotacaoUPDT.txt");
        printf(YEL "LEMBRETE: " CRESET "Nossas cotacoes sao atualizadas diariamente, se estao na data de hoje, nao e necessario tentar atualizar as cotacoes.\n");
        sleep(1.5);        
    }
}

void exibeCotacoes() {
    int pos = 0;
    char nome[4] = "";
    char cotacao[20] = "";
    char ultimo_update[20] = "";
    char data[50];
    char linha[50];
    FILE *f = fopen(COT_PATH, "r");
    time_t tempo;
    struct tm *tmp;

    if (f == NULL) {
        printErro("Erro ao acessar base de dados.");
    } else {
        while(fgets(linha, sizeof(linha), f)) {    
            pos = strcspn(linha, ";");
            strncpy(nome, linha, pos);

            strncpy(data, linha + pos, 50);

            pos = strcspn(data + 1, ";");
            strncpy(cotacao, data + 1, pos);
            
            if (strlen(ultimo_update) == 0) {
                strncpy(ultimo_update, data + pos + 2, 11);
            }
            printf(YEL "|");
            printf(" %s - %s ", nome, cotacao);

            memset(cotacao, 0, sizeof(cotacao));
        }

        tempo = (time_t)atoi(ultimo_update);
        tmp = localtime(&tempo);
        strftime(ultimo_update, sizeof(ultimo_update), "%d/%m/%Y", tmp);

        printf("| Ultima atualizacao em: %s\n" CRESET, ultimo_update);

        fclose(f);
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
    char *file_erro = "";
    const char *arquivos[] = {TRNS_PATH, USR_PATH, CART_PATH, COT_PATH};

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
        } else if (i == 3) {
            updateCotacoes();
        }
    }

    if (falha != 0) {
        fprintf(stderr, RED "ERRO: " CRESET "Não foi possível criar o arquivo %s. %s\n", file_erro, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

int insertNovaCarteira(char *cpf) {
    FILE *f = fopen(CART_PATH, "a");
    int erro = 0;
    char *saldoInicial = "0.0";

    if (f == NULL) {
        printErro("Não foi possível criar a carteira do usuário.");
        erro = 1;
    }

    // cpf + saldos para real, btc, eth e xrp
    if (erro == 0) {
        fprintf(f, "%s;%s;%s;%s;%s\n", cpf, saldoInicial, saldoInicial, saldoInicial, saldoInicial);

        fclose(f);
    }

    return erro;
}

void insertNovoUsuario(char *nome, char *cpf, char *senha) {
    int erro = 0;
    FILE *f = fopen(USR_PATH, "a");

    if (f == NULL) {
        erro = 1;
        printErro("Não foi possível inserir o usuário.");
    }

    if (erro == 0) {
        erro = insertNovaCarteira(cpf);
    }

    if (erro == 0) {
        fprintf(f, "%s;%s;%s\n", nome, cpf, senha);
    }

    fclose(f);
}

int checkCpfUnique(char *cpf) {
    char cpf_compare[12] = "";
    char linha[150] = "";
    int result = 1;
    int pos = 0;
    FILE *f = fopen(USR_PATH, "r");

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
    char nome_compare[120] = "";
    char cpf_compare[12] = "";
    char senha_compare[7] = "";
    char dados[19] = "";
    char linha[150] = "";

    int result = 0;
    int pos = 0;

    FILE *f = fopen(USR_PATH, "r");

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
        
        if (result == 0) {
            printf("\n");
            printErro("Informacoes de login invalidas. Tente novamente.");
            sleep(2);
        } else {
            strcpy(nome, nome_compare);
        }
        fclose(f);
    }

    return result;
}

void getSaldo(const char *cpf, char *real, char *btc, char *eth, char *xrp) {
    FILE *f = fopen(CART_PATH, "r");
    int erro = 0;
    int pos = 0;
    int pos2 = 0;
    char cpf_compare[12];
    char linha[100];
    char data[100];

    if (f == NULL) {
        printErro("Não foi possível recuperar o saldo");
        erro = 1;
    }

    if (erro == 0) {
        while (fgets(linha, sizeof(linha), f)) {
            pos = strcspn(linha, ";");
            strncpy(cpf_compare, linha, pos);
            
            if (strcmp(cpf_compare, cpf) == 0) {
                pos++;
                strncpy(data, linha + pos, 100);
                pos = strcspn(data, ";");
                strncpy(real, data, pos);
                
                pos++;
                strncpy(data, data + pos, 100);
                pos = strcspn(data, ";");
                strncpy(btc, data, pos);
                
                pos++;
                strncpy(data, data + pos, 100);
                pos = strcspn(data, ";");
                strncpy(eth, data, pos);
                
                pos++;
                strncpy(data, data + pos, 100);
                pos = strcspn(data, ";");
                strncpy(xrp, data, pos);
                break;
            }
        }

        fclose(f);
    }
}

void updateSaldo(const char *cpf, char *real, char *btc, char *eth, char *xrp) {
    FILE *f = fopen(CART_PATH, "r");
    FILE *f2 = fopen("data/carteiraUPDT.txt", "w");
    int erro = 0;
    int pos = 0;
    int pos2 = 0;
    char cpf_compare[12];
    char linha[100];
    char data[100];
    char BRL[20] = "";
    char BTC[20] = "";
    char ETH[20] = "";
    char XRP[20] = "";
    double brl_d = 0;
    double btc_d = 0;
    double eth_d = 0;
    double xrp_d = 0;
    double new_valor = 0;

    if (f == NULL) {
        printErro("Não foi possível atualizar o saldo");
        erro = 1;
    }

    if (erro == 0) {
        while (fgets(linha, sizeof(linha), f)) {
            pos = strcspn(linha, ";");
            strncpy(cpf_compare, linha, pos);

            if (strcmp(cpf_compare, cpf) == 0) {
                pos++;
                strncpy(data, linha + pos, 100);
                pos = strcspn(data, ";");
                strncpy(BRL, data, pos);
                pos++;
                strncpy(data, data + pos, 100);
                pos = strcspn(data, ";");
                strncpy(BTC, data, pos);
                pos++;
                strncpy(data, data + pos, 100);
                pos = strcspn(data, ";");
                strncpy(ETH, data, pos);
                pos++;
                strncpy(data, data + pos, 100);
                pos = strcspn(data, ";");
                strncpy(XRP, data, pos);

                if (strlen(real) > 0) {
                    brl_d = atof(BRL);
                    new_valor = atof(real);
                    brl_d += new_valor;
                    snprintf(BRL, sizeof(BRL), "%.2lf", brl_d);
                }
                
                if (strlen(btc) > 0) {
                    btc_d = atof(BTC);
                    new_valor = atof(real);
                    btc_d += new_valor;
                    snprintf(BTC, sizeof(BTC), "%.2lf", btc_d);
                }
                
                if (strlen(eth) > 0) {
                    eth_d = atof(ETH);
                    new_valor = atof(real);
                    eth_d += new_valor;
                    snprintf(ETH, sizeof(ETH), "%.2lf", eth_d);
                }
                
                if (strlen(xrp) > 0) {
                    xrp_d = atof(XRP);
                    new_valor = atof(real);
                    xrp_d += new_valor;
                    snprintf(XRP, sizeof(XRP), "%.2lf", xrp_d);
                }

                fprintf(f2, "%s;%s;%s;%s;%s", cpf_compare, BRL, BTC, ETH, XRP);
            } else {
                fprintf(f2, "%s", linha);
            }
        }

        fclose(f);
        fclose(f2);
        remove(CART_PATH);
        rename("data/carteiraUPDT.txt", CART_PATH);
    }
}

void insertTransacao(const char *cpf, char tipo, char *valor, char *moeda) {
    FILE *f = fopen(TRNS_PATH, "a");
    time_t data = time(NULL);
    int erro = 0;
    char data_str[20];

    snprintf(data_str, sizeof(data_str), "%ld", data);
    printf("%s;%s;%c;%s;%s\n", cpf, data_str, tipo, valor, moeda);

    if (f == NULL) {
        printErro("Não foi possível acessar a base de dados");
        erro = 1;
    }

    if (erro == 0) {
        fprintf(f, "%s;%s;%c;%s;%s\n", cpf, data_str, tipo, valor, moeda);
        fclose(f);
    }
}

void getTransacoes(const char *cpf, int l, char linhas[l][200]) {
    FILE *f = fopen(TRNS_PATH, "r");
    int i = 0;
    int erro = 0;
    int pos = 0;
    char cpf_compare[12] = "";
    char linha[200] = "";

    if (f == NULL) {
        printErro("Não foi possível recuperar o extrato.");
        erro = 1;
    }

    if (erro == 0) {
        while (fgets(linha, sizeof(linha), f)) {
            pos = strcspn(linha, ";");
            strncpy(cpf_compare, linha, pos);
            

            if (strcmp(cpf_compare, cpf) == 0) {
                strcpy(linhas[i], linha);
                i++;
            }
        }
    }
}