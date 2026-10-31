#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "arquivos.h"
#include "navegacao.h"
#include "ANSI-color-codes.h"

typedef struct _usuario {
    char nome[101];
    char cpf[12];
    char senha[7];
} Usuario;

Usuario *inicializaUser() {
    static Usuario user = {0};

    return &user;
}

const char *getUsrNome(const Usuario *user) {
    return user->nome;
}

const char *getUsrCpf(const Usuario *user) {
    return user->cpf;
}

const char *getUsrSenha(const Usuario *user) {
    return user->senha;
}

void setUsr(Usuario *user, const char *nome, const char *cpf, const char *senha) {
    strcpy(user->nome, nome);
    strcpy(user->cpf, cpf);
    strcpy(user->senha, senha);
}

void cadastrar() {
    char nome[101];
    char cpf[20];
    char senha[10];
    char *cpr;
    int len;
    int erro = 0;
    int continua = 1;

    printf("Digite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");

    do {
        erro = 0;
        printf("Insira seu nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = 0;

        if (strcmp(nome, "SAIR") == 0) {
            continua = 0;
            break;
        }

        len = strlen(nome);
        cpr = strchr(nome, ';');
        
        if (len > 100) {
            printErro("Seu nome ultrapassa os limites de caracteres. Tente novamente.");
            erro = 1;
        } else if (cpr != NULL) {
            printErro("Informe seu nome sem utilizar caracteres especiais como ';'. Tente novamente.");
            erro = 1;
        }
        
    } while (erro == 1);
    
    if (continua) {
        do {
            erro = 0;
            printf("Informe seu CPF (apenas numeros): ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0;
            
            if (strcmp(cpf, "SAIR") == 0) {
                continua = 0;
                break;
            }
            
            len = strlen(cpf);

            if (len != 11) {
                printErro("O CPF deve ter 11 digitos. Tente novamente.\n");
                erro = 1;
            } else {
                for (int i = 0; i < len; i++) {
                    if (!isdigit(cpf[i])) {
                        printErro("Digite apenas os numeros do CPF. Tente novamente.\n");
                        erro = 1;
                        break;
                    }
                }
            }
        } while (erro == 1);
    }
    
    if (continua) {
        do {
            erro = 0;
            printf("Crie sua senha (6 numeros): ");
            fgets(senha, sizeof(senha), stdin);
            senha[strcspn(senha, "\n")] = 0;
            
            if (strcmp(senha, "SAIR") == 0) {
                continua = 0;
                break;
            }
            
            len = strlen(senha);
            
            if (len != 6) {
                printErro("Sua senha deve ter 6 digitos. Tente novamente.\n");
                erro = 1;
            } else {
                for (int i = 0; i < len; i++) {
                    if (!isdigit(senha[i])) {
                        printErro("Sua senha deve ter apenas numeros. Tente novamente.\n");
                        erro = 1;
                        break;
                    }
                }
            }
    
        } while (erro == 1);
    }

    if (continua) {
        continua = checkCpfUnique(cpf);
    }

    if (continua) {
        insertNovoUsuario(nome, cpf, senha);
        printf("Cadastrando usuario...");
        sleep(2);
        printf(GRN " Usuario cadastrado com sucesso!");
        sleep(1);
    }
}

int login(int *auth, Usuario *user) {
    char nome[150];
    char cpf[20];
    char senha[10];

    char *cpr;
    int len;
    int erro;
    int continua = 1;

    printf("Digite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");
    
    if (continua) {
        do {
            erro = 0;
            printf("Informe seu CPF (apenas numeros): ");
            fgets(cpf, sizeof(cpf), stdin);
            cpf[strcspn(cpf, "\n")] = 0;
            
            if (strcmp(cpf, "SAIR") == 0) {
                continua = 0;
                break;
            }
            
            len = strlen(cpf);

            if (len != 11) {
                printErro("O CPF deve ter 11 digitos. Tente novamente.\n");
                erro = 1;
            } else {
                for (int i = 0; i < len; i++) {
                    if (!isdigit(cpf[i])) {
                        printErro("Digite apenas os numeros do CPF. Tente novamente.\n");
                        erro = 1;
                        break;
                    }
                }
            }
        } while (erro);
    }
    
    if (continua) {
        do {
            erro = 0;
            printf("Informe sua senha (6 numeros): ");
            fgets(senha, sizeof(senha), stdin);
            senha[strcspn(senha, "\n")] = 0;
            
            if (strcmp(senha, "SAIR") == 0) {
                continua = 0;
                break;
            }
            
            len = strlen(senha);
            
            if (len != 6) {
                printErro("Sua senha deve ter 6 digitos. Tente novamente.\n");
                erro = 1;
            } else {
                for (int i = 0; i < len; i++) {
                    if (!isdigit(senha[i])) {
                        printErro("Sua senha deve ter apenas numeros. Tente novamente.\n");
                        erro = 1;
                        break;
                    }
                }
            }
    
        } while (erro);
    }

    if (continua) {
        printf("Autenticando...");
        sleep(1);

        if (authUser(nome, cpf, senha) == 1) {
            setUsr(user, nome, cpf, senha);
            *auth = 1;
            printf(GRN " Bem-vindo!");
            sleep(1);
        } else {
            continua = 0;
        }
    }

    return continua;
}

void logoff(Usuario *user) {
    memset(user, 0, sizeof(Usuario));
}

void exibeSaldo(const Usuario *user) {
    char real[30] = "";
    char btc[30] = "";
    char eth[30] = "";
    char xrp[30] = "";
    char cpf[12] = "";

    strcpy(cpf, getUsrCpf(user));

    getSaldo(cpf, real, btc, eth, xrp);

    printf(BLU "\n# Nome:" CRESET " %s " BLU "| CPF:" CRESET " %s\n", getUsrNome(user), cpf); 

    printf(BLU "# R$:     " CRESET "%s\n", real);
    printf(BLU "# BTC:    " CRESET "%s\n", btc);
    printf(BLU "# ETH:    " CRESET "%s\n", eth);
    printf(BLU "# XRP:    " CRESET "%s", xrp);
}

int matchSenha(const char *senha) {
    char senha_compare[7];
    int erro;
    int len;
    int result = 1;

    printf("Digite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");

    do {
        erro = 0;
        printf("Informe sua senha (6 numeros): ");
        fgets(senha_compare, sizeof(senha_compare), stdin);
        senha_compare[strcspn(senha_compare, "\n")] = 0;
        
        if (strcmp(senha_compare, "SAIR") == 0) {
            result = 2;
            break;
        }
        
        len = strlen(senha_compare);
        
        if (len != 6) {
            printErro("Sua senha deve ter 6 digitos. Tente novamente.\n");
            erro = 1;
        } else {
            for (int i = 0; i < len; i++) {
                if (!isdigit(senha_compare[i])) {
                    printErro("Sua senha deve ter apenas numeros. Tente novamente.\n");
                    erro = 1;
                    break;
                }
            }
        }

    } while (erro == 1);

    if (result == 1) {
        if (strcmp(senha_compare, senha) != 0) {
            result = 0;
        }
    }

    return result;
}

int deposito(Usuario *user) {
    char valor[30] = "";
    char cpf[12] = "";
    double valor_d;
    int erro;
    int len = 0;
    int result = 1;

    strcpy(cpf, getUsrCpf(user));

    printf("Digite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");

    do {
        erro = 0;
        printf("Quanto deseja depositar? Informe como (99.99): R$ ");
        fgets(valor, sizeof(valor), stdin);
        valor[strcspn(valor, "\n")] = 0;
        
        if (strcmp(valor, "SAIR") == 0) {
            result = 0;
            break;
        }
        
        len = strlen(valor);

        for (int i = 0; i < len; i++) {
            if (!isdigit(valor[i]) && (valor[i] != '.')) {
                printErro("Digite o valor em reais que deseja depositar.\n");
                erro = 1;
                break;
            }
        }

        if (erro == 0) {
            valor_d = atof(valor);
            
            if (valor_d <= 0) {
                erro = 1;
                printErro("Voce deve depositar uma quantia valida.\n");
            }
        }
    } while (erro == 1);

    if (result == 1) {
        snprintf(valor, sizeof(valor), "%.2lf", valor_d);
        insertTransacao(cpf, '+', valor, "BRL");
        updateSaldo(cpf, valor, "", "", "");
    }

    return result;
}

int saque(Usuario *user) {
    int erro;
    int len = 0;
    int result = 1;
    double real_d;
    double valor_d;
    char valor[30] = "";
    char real[30] = "";
    char btc[30] = "";
    char eth[30] = "";
    char xrp[30] = "";
    char cpf[12] = "";

    strcpy(cpf, getUsrCpf(user));
    
    getSaldo(cpf, real, btc, eth, xrp);

    real_d = atof(real);

    printf("Digite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");

    do {
        strcpy(valor, "");
        erro = 0;
        printf("Quanto deseja sacar? Informe como (99.99): R$ ");
        fgets(valor, sizeof(valor), stdin);
        valor[strcspn(valor, "\n")] = 0;
        
        if (strcmp(valor, "SAIR") == 0) {
            result = 0;
            break;
        }
        
        len = strlen(valor);

        for (int i = 0; i < len; i++) {
            if (!isdigit(valor[i]) && (valor[i] != '.')) {
                printErro("Digite o valor em reais que deseja depositar.\n");
                erro = 1;
                break;
            }
        }    

        if (erro == 0) {
            valor_d = atof(valor);
    
            if (real_d - valor_d < 0 || valor_d <= 0) {
                erro = 1;
                printErro("Nao e possivel sacar esse valor.\n");
            }
        }
    } while (erro == 1);

    if (result == 1) {
        snprintf(valor, sizeof(valor), "%.2lf", valor_d);
        insertTransacao(cpf, '-', valor, "BRL");
        
        valor_d *= -1;
        snprintf(valor, sizeof(valor), "%.2lf", valor_d);
        updateSaldo(cpf, valor, "", "", "");
    }

    return result;
}

void exibeExtrato(Usuario *user) {
    int l = contaLinhas("data/transacao.txt");
    int pos = 0;
    char linhas[l][200];
    char tipo = 'o';
    char data_str[100] = "";
    char valor[30] = "";
    char moeda[4] = "";
    char cotacao[30] =  "";
    char taxa[10] = "";
    time_t tempo;
    struct tm *tmp;

    getTransacoes(getUsrCpf(user), l, linhas);

    printf(BLU "\nNome: " CRESET "%s\n", getUsrNome(user));
    printf(BLU "CPF: " CRESET "%s\n\n", getUsrCpf(user));

    for (int i = 0; i < l; i++) {
        // pegando os campos de transacoes
        strncpy(linhas[i], linhas[i] + 12, sizeof(linhas[i]) - 1);
        
        pos = strcspn(linhas[i], ";");
        strncpy(data_str, linhas[i], pos);
        pos++;
        strncpy(linhas[i], linhas[i] + pos, sizeof(linhas[i]));
        
        tipo = linhas[i][0];
        pos = strcspn(linhas[i], ";");
        pos++;
        strncpy(linhas[i], linhas[i] + pos, sizeof(linhas[i]));
        
        pos = strcspn(linhas[i], ";");
        strncpy(valor, linhas[i], pos);
        pos++;
        strncpy(moeda, linhas[i] + pos, 3);
        // --

        if (strcmp(moeda, "BRL") == 0) {
            strcpy(cotacao, "0.0");
            strcpy(taxa, "Nao ha");
        } else {
            getCotacao(moeda, cotacao);
        }

        if (tipo = '+') {
            if (strcmp(moeda, "BTC") == 0) {
                strcpy(taxa, "2%");
            } else if (strcmp(moeda, "ETH") == 0) {
                strcpy(taxa, "1%");
            } else if (strcmp(moeda, "XRP") == 0) {
                strcpy(taxa, "1%");
            }
        } else {
            if (strcmp(moeda, "BTC") == 0) {
                strcpy(taxa, "3%");
            } else if (strcmp(moeda, "ETH") == 0) {
                strcpy(taxa, "2%");
            } else if (strcmp(moeda, "XRP") == 0) {
                strcpy(taxa, "1%");
            }
        }

        // pegando a data da transação
        tempo = (time_t)atoi(data_str);
        tmp = localtime(&tempo);
        strftime(data_str, sizeof(data_str), "%d/%m/%Y %H:%M", tmp);

        // linha do extrato
        printf(YEL "| %s | %c | %s | %s | CT: %s | TX: %s |\n" CRESET, data_str, tipo, valor, moeda, cotacao, taxa);
    }
}

void compraCripto(const Usuario *user) {
    char confirma = 'o';
    char opcao[10] = "";
    char valor[30] = "";
    char cotacao[30] = "";
    char brl_saldo[30] = "";
    char btc_saldo[30] = "";
    char eth_saldo[30] = "";
    char xrp_saldo[30] = "";
    char compra_saldo[30] = "";
    char cpf[12] = "";
    double brl_saldo_d = 0;
    double compra_saldo_d = 0;
    double valor_d = 0;
    double cotacao_d = 0;
    double custo = 0;
    float taxa = 0;
    int erro = 0;
    int stop = 0;
    int len = 0;

    strcpy(cpf, getUsrCpf(user));

    printf(BLU "##############################" YEL " SALDO ATUAL " BLU "##############################\n" CRESET);
    exibeSaldo(user);

    printf("\nDigite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");

    do {
        erro = 0;
        printf("Informe qual moeda deseja comprar: \n");
        printf("1 - BTC\n");
        printf("2 - ETH\n");
        printf("3 - XRP\n");
        fgets(opcao, sizeof(opcao), stdin);
        opcao[strcspn(opcao, "\n")] = 0;

        if (strcmp(opcao, "SAIR") == 0) {
            stop = 1;
            break;
        }
        
        if (stop == 0) {
            if (strcmp(opcao, "1") != 0 && strcmp(opcao, "2") != 0 && strcmp(opcao, "3") != 0) {
                printErro("Escolha uma opcao valida. Tente novamente.");
                erro = 1;
            }
            
            if (strcmp(opcao, "1") == 0) {
                strcpy(opcao, "BTC");
                taxa = 0.02;
            } else if (strcmp(opcao, "2") == 0) {
                strcpy(opcao, "ETH");
                taxa = 0.01;
            } else {
                strcpy(opcao, "XRP");
                taxa = 0.01;
            }
        }
        
        if (erro == 0 && stop == 0) {
            
        }
    } while (erro == 1);
    
    if (stop == 0) {
        printf("Essa compra incluira uma taxa de %.2f%%.\n", taxa * 100);
        do {
            erro = 0;
            printf("Informe quanto deseja comprar de %s em reais: R$ ", opcao);
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = 0;
            
            if (strcmp(valor, "SAIR") == 0) {
                stop = 1;
                break;
            }
            
            if (stop == 0) {
                len = strlen(valor);
                for (int i = 0; i < len; i++) {
                    if (!isdigit(valor[i]) && (valor[i] != '.')) {
                        printErro("Valor inserido invalido. Informe um numero.\n");
                        erro = 1;
                        break;
                    }
                }
                
                if (erro == 0) {
                    getSaldo(cpf, brl_saldo, btc_saldo, eth_saldo, xrp_saldo);
    
                    valor_d = atof(valor);
                    brl_saldo_d = atof(brl_saldo);
                    custo = valor_d + (valor_d * taxa);
    
                    if (valor_d <= 0) {
                        erro = 1;
                        printErro("Voce deve informar uma quantia valida.\n");
                    } else if (custo > brl_saldo_d) {
                        erro = 1;
                        printErro("Seu saldo atual nao e suficiente para realizar esta operacao. Tente novamente com um valor valido ou realize um deposito.");
                    }
                }
            }
        } while (erro == 1);
    }

    if (stop == 0) {
        getCotacao(opcao, cotacao);
        cotacao_d = atof(cotacao);
        
        compra_saldo_d += valor_d / cotacao_d;

        do {
            printf(YEL "O valor total dessa compra sera de R$ %.2lf por %lf %s. Deseja confirmar?\n" CRESET, custo, compra_saldo_d, opcao);
            printf("S - Sim     N - Nao\n");
            
            confirma = toupper(getchar());
            while(getchar() != '\n');
            
            if (confirma == 'N') {
                stop = 1;
                printf("Cancelando operacao...");
                sleep(1);
                break;
            } else if (confirma != 'S') {
                printErro("Selecione uma opcao valida.");
                erro = 1;
            }
        } while (erro == 1);
    }

    if (stop == 0) {
        printf("Confirmando transacao...");
        sleep(1);

        snprintf(compra_saldo, sizeof(compra_saldo), "%lf", compra_saldo_d);

        strcpy(btc_saldo, "");
        strcpy(eth_saldo, "");
        strcpy(xrp_saldo, "");

        if (strcmp(opcao, "BTC") == 0) {
            strcpy(btc_saldo, compra_saldo);
        } else if (strcmp(opcao, "ETH") == 0) {
            strcpy(eth_saldo, compra_saldo);
        } else {
            strcpy(xrp_saldo, compra_saldo);
        }
        snprintf(brl_saldo, sizeof(brl_saldo), "%.2lf", custo * -1);

        insertTransacao(cpf, '+', compra_saldo, opcao);
        updateSaldo(cpf, brl_saldo, btc_saldo, eth_saldo, xrp_saldo);

        printf(GRN "Transacao realizada com sucesso!\n" CRESET);
        sleep(2);
    }
}

void vendaCripto(const Usuario *user) {
    char confirma = 'o';
    char opcao[10] = "";
    char valor[30] = "";
    char cotacao[30] = "";
    char brl_saldo[30] = "";
    char btc_saldo[30] = "";
    char eth_saldo[30] = "";
    char xrp_saldo[30] = "";
    char venda_saldo[30] = "";
    char cpf[12] = "";
    double brl_saldo_d = 0;
    double venda_saldo_d = 0;
    double valor_d = 0;
    double cotacao_d = 0;
    double pagamento = 0;
    float taxa = 0;
    int erro = 0;
    int stop = 0;
    int len = 0;

    strcpy(cpf, getUsrCpf(user));

    printf(BLU "##############################" YEL " SALDO ATUAL " BLU "##############################\n" CRESET);
    exibeSaldo(user);

    printf("\nDigite " RED "SAIR" CRESET " a qualquer momento para cancelar a operacao e retornar ao menu.\n");

    do {
        erro = 0;
        printf("Informe qual moeda deseja vender: \n");
        printf("1 - BTC\n");
        printf("2 - ETH\n");
        printf("3 - XRP\n");
        fgets(opcao, sizeof(opcao), stdin);
        opcao[strcspn(opcao, "\n")] = 0;

        if (strcmp(opcao, "SAIR") == 0) {
            stop = 1;
            break;
        }
        
        if (stop == 0) {
            if (strcmp(opcao, "1") != 0 && strcmp(opcao, "2") != 0 && strcmp(opcao, "3") != 0) {
                printErro("Escolha uma opcao valida. Tente novamente.");
                erro = 1;
            }
            
            if (strcmp(opcao, "1") == 0) {
                strcpy(opcao, "BTC");
                taxa = 0.03;
            } else if (strcmp(opcao, "2") == 0) {
                strcpy(opcao, "ETH");
                taxa = 0.02;
            } else {
                strcpy(opcao, "XRP");
                taxa = 0.01;
            }
        }
        
        if (erro == 0 && stop == 0) {
            
        }
    } while (erro == 1);
    
    if (stop == 0) {
        printf("Essa tera uma taxa de %.2f%.\n", taxa * 100);
        do {
            erro = 0;
            printf("Informe quanto deseja vender de %s: ", opcao);
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = 0;
            
            if (strcmp(valor, "SAIR") == 0) {
                stop = 1;
                break;
            }
            
            if (stop == 0) {
                len = strlen(valor);
                for (int i = 0; i < len; i++) {
                    if (!isdigit(valor[i]) && (valor[i] != '.')) {
                        printErro("Valor inserido invalido. Informe um numero.\n");
                        erro = 1;
                        break;
                    }
                }
                
                if (erro == 0) {
                    getSaldo(cpf, brl_saldo, btc_saldo, eth_saldo, xrp_saldo);


                    if (strcmp(opcao, "BTC") == 0) {
                        strcpy(venda_saldo, btc_saldo);
                    } else if (strcmp(opcao, "ETH") == 0) {
                        strcpy(venda_saldo, eth_saldo);
                    } else {
                        strcpy(venda_saldo, xrp_saldo);
                    }
    
                    valor_d = atof(valor);
                    venda_saldo_d = atof(venda_saldo);                 
                    
                    if (valor_d <= 0) {
                        erro = 1;
                        printErro("Voce deve informar uma quantia valida.\n");
                    } else if (valor_d > venda_saldo_d) {
                        erro = 1;
                        printErro("Seu saldo atual nao e suficiente para realizar esta operacao. Tente novamente com um valor valido ou realize um deposito.");
                    }
                }
            }
        } while (erro == 1);
    }
    
    if (stop == 0) {
        getCotacao(opcao, cotacao);
        cotacao_d = atof(cotacao);
        
        pagamento = (valor_d * cotacao_d) - (valor_d * cotacao_d * taxa);

        do {
            printf(YEL "Voce recebera R$ %.2lf por %s %s. Deseja confirmar?\n" CRESET, pagamento, valor, opcao);
            printf("S - Sim     N - Nao\n");
            
            confirma = toupper(getchar());
            while(getchar() != '\n');
            
            if (confirma == 'N') {
                stop = 1;
                printf("Cancelando operacao...");
                sleep(1);
                break;
            } else if (confirma != 'S') {
                printErro("Selecione uma opcao valida.");
                erro = 1;
            }
        } while (erro == 1);
    }

    if (stop == 0) {
        printf("Confirmando transacao...");
        sleep(1);

        snprintf(valor, sizeof(valor), "%lf", valor_d * -1);
        snprintf(brl_saldo, sizeof(brl_saldo), "%.2lf", pagamento);

        strcpy(btc_saldo, "");
        strcpy(eth_saldo, "");
        strcpy(xrp_saldo, "");

        if (strcmp(opcao, "BTC") == 0) {
            strcpy(btc_saldo, valor);
        } else if (strcmp(opcao, "ETH") == 0) {
            strcpy(eth_saldo, valor);
        } else {
            strcpy(xrp_saldo, valor);
        }

        insertTransacao(cpf, '-', valor, opcao);
        updateSaldo(cpf, brl_saldo, btc_saldo, eth_saldo, xrp_saldo);

        printf(GRN "Transacao realizada com sucesso!\n" CRESET);
        sleep(2);
    }
}
