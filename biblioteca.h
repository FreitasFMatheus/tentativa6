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

typedef struct {
    Cliente clientes[MAX_CLIENTES];
    int numClientes;
    Transacao transacoes[MAX_TRANSACOES];
    int numTransacoes;
} BancoDados;

// Funções relacionadas a clientes
void adicionarCliente(BancoDados *dados, Cliente c);
Cliente *buscarClientePorCPF(BancoDados *dados, const char *cpf);
void removerCliente(BancoDados *dados, const char *cpf);
void listarClientes(BancoDados *dados);
void debitar(BancoDados *dados, const char *cpf, const char *senha, float valor);
void depositar(BancoDados *dados, const char *cpf, float valor);
void extrato(BancoDados *dados, const char *cpf, const char *senha);
void transferir(BancoDados *dados, char cpf[], char senha[], char cpfDestino[], float valor);

// Funções relacionadas a transações
void registrarTransacao(BancoDados *dados, Transacao t);

// Funções para salvar e carregar dados de arquivos
void salvarClientesArquivo(BancoDados *dados);
void carregarClientesArquivo(BancoDados *dados);
void salvarTransacoesArquivo(BancoDados *dados);
void carregarTransacoesArquivo(BancoDados *dados);

#endif // BIBLIOTECA_H
