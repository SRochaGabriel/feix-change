int initDirData();
int initDataFiles();
void insertNovoUsuario(char *nome, char *cpf, char *senha);
int checkCpfUnique(char *cpf);
int authUser(char *cpf, char *senha);