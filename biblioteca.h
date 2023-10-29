#ifndef BIBLIOTECA_H
#define BIBLIOTECA_H

#define MAX_NOME 100
#define MAX_CPF 12
#define MAX_SENHA 10
#define MAX_CLIENTES 100
#define MAX_TRANSACOES 1000

typedef enum {
    COMUM,
    PLUS
} TipoConta;

typedef struct {
    char nome[MAX_NOME];
    char cpf[MAX_CPF];
    char senha[MAX_SENHA];
    TipoConta tipo;
    float saldo;
} Cliente;

typedef enum {
    DEPOSITO,
    DEBITO
} TipoTransacao;

typedef struct {
    char cpf[MAX_CPF];
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
