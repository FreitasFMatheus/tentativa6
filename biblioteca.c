//
// Created by Matheus Freitas on 24/09/2023.
//

#include "biblioteca.h"
#include <stdio.h>
#include <string.h>

// Definindo constantes para os nomes dos arquivos
#define CLIENTES_FILE "clientes.bin"
#define TRANSACOES_FILE "transacoes.bin"
#define EXTRATO_FILE "extrato.txt"

// Função para adicionar um cliente à lista de clientes
void adicionarCliente(BancoDados *dados, Cliente c) {
    if (dados->numClientes < MAX_CLIENTES) {
        dados->clientes[dados->numClientes] = c;
        dados->numClientes++;
    } else {
        printf("Limite de clientes atingido.\n");
    }
}

// Busca um cliente pelo CPF e retorna um ponteiro para o mesmo
Cliente *buscarClientePorCPF(BancoDados *dados, const char *cpf) {
    for (int i = 0; i < dados->numClientes; i++) {
        if (strcmp(dados->clientes[i].cpf, cpf) == 0) {
            return &dados->clientes[i];
        }
    }
    return NULL;
}

// Remove um cliente pelo CPF
void removerCliente(BancoDados *dados, const char *cpf) {
    int index = -1;
    for (int i = 0; i < dados->numClientes; i++) {
        if (strcmp(dados->clientes[i].cpf, cpf) == 0) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        for (int i = index; i < dados->numClientes - 1; i++) {
            dados->clientes[i] = dados->clientes[i + 1];
        }
        dados->numClientes--;
    } else {
        printf("Cliente não encontrado.\n");
    }
}

// Função que lista todos os clientes registrados
void listarClientes(BancoDados *dados) {
    for (int i = 0; i < dados->numClientes; i++) {
        printf("Nome: %s, CPF: %s, Tipo de Conta: %s, Saldo: %.2f\n",
               dados->clientes[i].nome,
               dados->clientes[i].cpf,
               dados->clientes[i].tipo == COMUM ? "Comum" : "Plus",
               dados->clientes[i].saldo);
    }
}

// Função para debitar um valor da conta de um cliente
void debitar(BancoDados *dados, const char *cpf, const char *senha, float valor) {
    Cliente *cliente = buscarClientePorCPF(dados, cpf);
    if (cliente && strcmp(cliente->senha, senha) == 0) {
        float taxa = (cliente->tipo == COMUM) ? 0.05 : 0.03;
        float debito = valor + valor * taxa;
        cliente->saldo -= debito;

        // Registrar transação
        Transacao t;
        strcpy(t.cpf, cpf);
        t.tipo = DEBITO;
        t.valor = debito;
        registrarTransacao(dados, t);
    } else {
        printf("CPF ou senha incorretos.\n");
    }
}

// Função para depositar um valor na conta de um cliente
void depositar(BancoDados *dados, const char *cpf, float valor) {
    Cliente *cliente = buscarClientePorCPF(dados, cpf);
    if (cliente) {
        cliente->saldo += valor;

        // Registrar transação
        Transacao t;
        strcpy(t.cpf, cpf);
        t.tipo = DEPOSITO;
        t.valor = valor;
        registrarTransacao(dados, t);
    } else {
        printf("Cliente não encontrado.\n");
    }
}

void extrato(BancoDados *dados, const char *cpf, const char *senha) {
    Cliente *cliente = buscarClientePorCPF(dados, cpf);
    if (cliente && strcmp(cliente->senha, senha) == 0) {
        FILE *file = fopen(EXTRATO_FILE, "w");
        for (int i = 0; i < dados->numTransacoes; i++) {
            if (strcmp(dados->transacoes[i].cpf, cpf) == 0) {
                fprintf(file, "Tipo: %s, Valor: %.2f\n",
                        dados->transacoes[i].tipo == DEPOSITO ? "Depósito" : "Débito",
                        dados->transacoes[i].valor);
            }
        }
        fclose(file);
    } else {
        printf("CPF ou senha incorretos.\n");
    }
}

// Função para transferir dinheiro entre dois clientes
void transferir(BancoDados *dados, char cpf[], char senha[], char cpfDestino[], float valor) {
    Cliente *clienteOrigem = buscarClientePorCPF(dados, cpf);
    Cliente *clienteDestino = buscarClientePorCPF(dados, cpfDestino);

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
            registrarTransacao(dados, tOrigem);

            // Registrar transação de depósito
            Transacao tDestino;
            strcpy(tDestino.cpf, cpfDestino);
            tDestino.tipo = DEPOSITO;
            tDestino.valor = valor;
            registrarTransacao(dados, tDestino);
        } else {
            printf("Saldo insuficiente.\n");
        }
    } else {
        printf("CPF ou senha incorretos ou cliente destino não encontrado.\n");
    }
}
// Função para registrar uma nova transação, seja de débito ou depósito.
void registrarTransacao(BancoDados *dados, Transacao t) {
    if (dados->numTransacoes < MAX_TRANSACOES) {
        dados->transacoes[dados->numTransacoes] = t;
        dados->numTransacoes++;
    } else {
        printf("Limite de transações atingido.\n");
    }
}

// Função para salvar os clientes em um arquivo binário
void salvarClientesArquivo(BancoDados *dados) {
    FILE *file = fopen(CLIENTES_FILE, "wb");
    if (file) {
        fwrite(&dados->numClientes, sizeof(int), 1, file);
        fwrite(dados->clientes, sizeof(Cliente), dados->numClientes, file);
        fclose(file);
    } else {
        printf("Erro ao salvar clientes no arquivo.\n");
    }
}

// Função para carregar os clientes de um arquivo binário
void carregarClientesArquivo(BancoDados *dados) {
    FILE *file = fopen(CLIENTES_FILE, "rb");
    if (file) {
        fread(&dados->numClientes, sizeof(int), 1, file);
        fread(dados->clientes, sizeof(Cliente), dados->numClientes, file);
        fclose(file);
    } else {
        printf("Erro ao carregar clientes do arquivo.\n");
    }
}

// Função para salvar as transações em um arquivo binário
void salvarTransacoesArquivo(BancoDados *dados) {
    FILE *file = fopen(TRANSACOES_FILE, "wb");
    if (file) {
        fwrite(&dados->numTransacoes, sizeof(int), 1, file);
        fwrite(dados->transacoes, sizeof(Transacao), dados->numTransacoes, file);
        fclose(file);
    } else {
        printf("Erro ao salvar transações no arquivo.\n");
    }
}

// Função para carregar as transações de um arquivo binário
void carregarTransacoesArquivo(BancoDados *dados) {
    FILE *file = fopen(TRANSACOES_FILE, "rb");
    if (file) {
        fread(&dados->numTransacoes, sizeof(int), 1, file);
        fread(dados->transacoes, sizeof(Transacao), dados->numTransacoes, file);
        fclose(file);
    } else {
        printf("Erro ao carregar transações do arquivo.\n");
    }
}
