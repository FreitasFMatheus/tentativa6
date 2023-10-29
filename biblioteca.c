//
// Created by Matheus Freitas on 24/09/2023.
//

#include "biblioteca.h"
#include <stdio.h>
#include <string.h>

//"olá Leo, tudo bem contigo não coloquei nome de filme nesse projeto igual no semestre passado, mas ainda assim espero que tenha ficado menos cansativo de ler durante a correção"

// Definindo constantes para os nomes dos arquivos
#define CLIENTES_FILE "clientes.bin"
#define TRANSACOES_FILE "transacoes.bin"
#define EXTRATO_FILE "extrato.txt"

// Arrays estáticos para armazenar clientes e transações
static Cliente clientes[MAX_CLIENTES];
static int numClientes = 0;  // Contador de clientes
static Transacao transacoes[MAX_TRANSACOES];
static int numTransacoes = 0;  // Contador de transações

// Função para adicionar um cliente à lista de clientes
void adicionarCliente(Cliente c) {
    if (numClientes < MAX_CLIENTES) {
        clientes[numClientes] = c;
        numClientes++;
    } else {
        printf("Limite de clientes atingido.\n");
    }
}

// Busca um cliente pelo CPF e retorna um ponteiro para o mesmo
Cliente *buscarClientePorCPF(const char *cpf) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            return &clientes[i];
        }
    }
    return NULL;  // Retorna NULL se não encontrar o cliente
}

// Remove um cliente pelo CPF
void removerCliente(const char *cpf) {
    int index = -1;
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        for (int i = index; i < numClientes - 1; i++) {
            clientes[i] = clientes[i + 1];
        }
        numClientes--;
    } else {
        printf("Cliente não encontrado.\n");
    }
}

// Função que lista todos os clientes registrados
void listarClientes() {
    for (int i = 0; i < numClientes; i++) {
        printf("Nome: %s, CPF: %s, Tipo de Conta: %s, Saldo: %.2f\n",
               clientes[i].nome,
               clientes[i].cpf,
               clientes[i].tipo == COMUM ? "Comum" : "Plus",
               clientes[i].saldo);
    }
}

// Função para debitar um valor da conta de um cliente
void debitar(const char *cpf, const char *senha, float valor) {
    Cliente *cliente = buscarClientePorCPF(cpf);
    if (cliente && strcmp(cliente->senha, senha) == 0) {
        float taxa = (cliente->tipo == COMUM) ? 0.05 : 0.03;
        float debito = valor + valor * taxa;
        cliente->saldo -= debito;

        // Registrar transação
        Transacao t;
        strcpy(t.cpf, cpf);
        t.tipo = DEBITO;
        t.valor = debito;
        registrarTransacao(t);
    } else {
        printf("CPF ou senha incorretos.\n");
    }
}

// Função para depositar um valor na conta de um cliente
void depositar(const char *cpf, float valor) {
    Cliente *cliente = buscarClientePorCPF(cpf);
    if (cliente) {
        cliente->saldo += valor;

        // Registrar transação
        Transacao t;
        strcpy(t.cpf, cpf);
        t.tipo = DEPOSITO;
        t.valor = valor;
        registrarTransacao(t);
    } else {
        printf("Cliente não encontrado.\n");
    }
}

// Função para gerar o extrato das transações de um cliente
void extrato(const char *cpf, const char *senha) {
    Cliente *cliente = buscarClientePorCPF(cpf);
    if (cliente && strcmp(cliente->senha, senha) == 0) {
        FILE *file = fopen(EXTRATO_FILE, "w");
        for (int i = 0; i < numTransacoes; i++) {
            if (strcmp(transacoes[i].cpf, cpf) == 0) {
                fprintf(file, "Tipo: %s, Valor: %.2f\n",
                        transacoes[i].tipo == DEPOSITO ? "Depósito" : "Débito",
                        transacoes[i].valor);
            }
        }
        fclose(file);
    } else {
        printf("CPF ou senha incorretos.\n");
    }
}

// Função para transferir dinheiro entre dois clientes
void transferir(char cpf[], char senha[], char cpfDestino[], float valor) {
    Cliente *clienteOrigem = buscarClientePorCPF(cpf);
    Cliente *clienteDestino = buscarClientePorCPF(cpfDestino);

    if (clienteOrigem && clienteDestino && strcmp(clienteOrigem->senha, senha) == 0) {
        float taxaOrigem = (clienteOrigem->tipo == COMUM) ? 0.05 : 0.03;
        float debito = valor + valor * taxaOrigem;

        if (clienteOrigem->saldo - debito >= 0) {
            clienteOrigem->saldo -= debito;
            clienteDestino->saldo += valor;

            // Registrar transação de débito
            Transacao tOrigem;
            strcpy(tOrigem.cpf, cpf);
            tOrigem.tipo = DEBITO;
            tOrigem.valor = debito;
            registrarTransacao(tOrigem);

            // Registrar transação de depósito
            Transacao tDestino;
            strcpy(tDestino.cpf, cpfDestino);
            tDestino.tipo = DEPOSITO;
            tDestino.valor = valor;
            registrarTransacao(tDestino);
        } else {
            printf("Saldo insuficiente.\n");
        }
    } else {
        printf("CPF ou senha incorretos ou cliente destino não encontrado.\n");
    }
}

// Função para registrar uma nova transação, seja de débito ou depósito.
void registrarTransacao(Transacao t) {
    // Verifica se o limite de transações foi atingido
    if (numTransacoes < MAX_TRANSACOES) {
        // Adiciona a transação ao array de transações
        transacoes[numTransacoes] = t;
        numTransacoes++;
    } else {
        // Exibe uma mensagem de erro se o limite de transações for atingido
        printf("Limite de transações atingido.\n");
    }
}

// Função para salvar os clientes em um arquivo binário
void salvarClientesArquivo() {
    // Abre o arquivo para gravação em modo binário
    FILE *file = fopen(CLIENTES_FILE, "wb");
    if (file) {
        // Escreve a quantidade de clientes e depois os dados dos clientes
        fwrite(&numClientes, sizeof(int), 1, file);
        fwrite(clientes, sizeof(Cliente), numClientes, file);
        fclose(file);
    } else {
        // Exibe uma mensagem de erro se não for possível abrir o arquivo
        printf("Erro ao salvar clientes no arquivo.\n");
    }
}

// Função para carregar os clientes de um arquivo binário
void carregarClientesArquivo() {
    FILE *file = fopen(CLIENTES_FILE, "rb");
    if (file) {
        // Lê a quantidade de clientes e depois os dados dos clientes
        fread(&numClientes, sizeof(int), 1, file);
        fread(clientes, sizeof(Cliente), numClientes, file);
        fclose(file);
    } else {
        // Exibe uma mensagem de erro se não for possível abrir o arquivo
        printf("Erro ao carregar clientes do arquivo.\n");
    }
}

// Função para salvar as transações em um arquivo binário
void salvarTransacoesArquivo() {
    // Abre o arquivo para gravação em modo binário
    FILE *file = fopen(TRANSACOES_FILE, "wb");
    if (file) {
        // Escreve a quantidade de transações e depois os dados das transações
        fwrite(&numTransacoes, sizeof(int), 1, file);
        fwrite(transacoes, sizeof(Transacao), numTransacoes, file);
        fclose(file);
    } else {
        // Exibe uma mensagem de erro se não for possível abrir o arquivo
        printf("Erro ao salvar transações no arquivo.\n");
    }
}

// Função para carregar as transações de um arquivo binário
void carregarTransacoesArquivo() {
    FILE *file = fopen(TRANSACOES_FILE, "rb");
    if (file) {
        // Lê a quantidade de transações e depois os dados das transações
        fread(&numTransacoes, sizeof(int), 1, file);
        fread(transacoes, sizeof(Transacao), numTransacoes, file);
        fclose(file);
    } else {
        // Exibe uma mensagem de erro se não for possível abrir o arquivo
        // Tive alguns problemas fazendo essa parte, a leitura estava dando errado
        printf("Erro ao carregar transações do arquivo.\n");
    }
}
