int initDirData();
int initDataFiles();
void insertNovoUsuario(char *nome, char *cpf, char *senha);
int checkCpfUnique(char *cpf);
int authUser(char *nome, char *cpf, char *senha);
void exibeCotacoes();
void updateCotacoes();
void getSaldo(const char *cpf, char *real, char *btc, char *eth, char*xrp);