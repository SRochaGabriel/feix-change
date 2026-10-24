typedef struct _usuario Usuario;
Usuario *inicializaUser();
const char *getUsrNome(const Usuario *user);
const char *getUsrCpf(const Usuario *user);
const char *getUsrSenha(const Usuario *user);
void cadastrar();
void login();