//
// Created by Matheus Freitas on 24/09/2023.
//
#include "biblioteca.h"
#include <stdio.h>
#include <string.h>

#define CLIENTES_FILE "clientes.bin"
#define TRANSACOES_FILE "transacoes.bin"
#define EXTRATO_FILE "extrato.txt"

static Cliente clientes[MAX_CLIENTES];
static int numClientes = 0;
static Transacao transacoes[MAX_TRANSACOES];
static int numTransacoes = 0;

void adicionarCliente(Cliente c) {
    if (numClientes < MAX_CLIENTES) {
        clientes[numClientes] = c;
        numClientes++;
    } else {
        printf("Limite de clientes atingido.\n");
    }
}

Cliente *buscarClientePorCPF(const char *cpf) {
    for (int i = 0; i < numClientes; i++) {
        if (strcmp(clientes[i].cpf, cpf) == 0) {
            return &clientes[i];
        }
    }
    return NULL;
}

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

void listarClientes() {
    for (int i = 0; i < numClientes; i++) {
        printf("Nome: %s, CPF: %s, Tipo de Conta: %s, Saldo: %.2f\n",
               clientes[i].nome,
               clientes[i].cpf,
               clientes[i].tipo == COMUM ? "Comum" : "Plus",
               clientes[i].saldo);
    }
}

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

void registrarTransacao(Transacao t) {
    if (numTransacoes < MAX_TRANSACOES) {
        transacoes[numTransacoes] = t;
        numTransacoes++;
    } else {
        printf("Limite de transações atingido.\n");
    }
}

void salvarClientesArquivo() {
    FILE *file = fopen(CLIENTES_FILE, "wb");
    if (file) {
        fwrite(&numClientes, sizeof(int), 1, file);
        fwrite(clientes, sizeof(Cliente), numClientes, file);
        fclose(file);
    } else {
        printf("Erro ao salvar clientes no arquivo.\n");
    }
}

void carregarClientesArquivo() {
    FILE *file = fopen(CLIENTES_FILE, "rb");
    if (file) {
        fread(&numClientes, sizeof(int), 1, file);
        fread(clientes, sizeof(Cliente), numClientes, file);
        fclose(file);
    } else {
        printf("Erro ao carregar clientes do arquivo.\n");
    }
}

void salvarTransacoesArquivo() {
    FILE *file = fopen(TRANSACOES_FILE, "wb");
    if (file) {
        fwrite(&numTransacoes, sizeof(int), 1, file);
        fwrite(transacoes, sizeof(Transacao), numTransacoes, file);
        fclose(file);
    } else {
        printf("Erro ao salvar transações no arquivo.\n");
    }
}

void carregarTransacoesArquivo() {
    FILE *file = fopen(TRANSACOES_FILE, "rb");
    if (file) {
        fread(&numTransacoes, sizeof(int), 1, file);
        fread(transacoes, sizeof(Transacao), numTransacoes, file);
        fclose(file);
    } else {
        printf("Erro ao carregar transações do arquivo.\n");
    }
}
