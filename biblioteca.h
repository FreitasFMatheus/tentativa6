#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#define MAX_NOME 100
#define MAX_CPF 12
#define MAX_SENHA 10
#define MAX_CLIENTES 100
#define MAX_TRANSACOES 1000



#define MAX_CPF_SIZE (MAX_CPF + 1)
#define MAX_NOME_SIZE (MAX_NOME + 1)
#define MAX_SENHA_SIZE (MAX_SENHA + 1)

typedef enum {
    COMUM,
    PLUS
} TipoConta;

typedef struct {
    char nome[MAX_NOME_SIZE];
    char cpf[MAX_CPF_SIZE];
    char senha[MAX_SENHA_SIZE];
    TipoConta tipo;
    float saldo;
} Cliente;

typedef enum {
    DEPOSITO,
    DEBITO
} TipoTransacao;

typedef struct {
    char cpf[MAX_CPF_SIZE];
    TipoTransacao tipo;
    float valor;
} Transacao;

// Funções relacionadas a clientes
void adicionarCliente(Cliente c);
Cliente *buscarClientePorCPF(const char *cpf);
void removerCliente(const char *cpf);
void listarClientes();
void debitar(const char *cpf, const char *senha, float valor);
void depositar(const char *cpf, float valor);
void extrato(const char *cpf, const char *senha);
void transferir(char cpf[], char senha[], char cpfDestino[], float valor);

// Funções relacionadas a transações
void registrarTransacao(Transacao t);

// Funções para salvar e carregar dados de arquivos
void salvarClientesArquivo();
void carregarClientesArquivo();
void salvarTransacoesArquivo();
void carregarTransacoesArquivo();

#endif // BIBLIOTECA_H
