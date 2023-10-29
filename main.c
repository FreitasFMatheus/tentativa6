#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

// Feito por:
 //Matheus Ferreira de Freitas - 24.123.080-4
 // Rafael Russo Mingroni - 24.123.054-9

void menu() {
    printf("====================================\n");
    printf("Banco QuemPoupaTem\n");
    printf("====================================\n");
    printf("1. Novo cliente\n");
    printf("2. Apaga cliente\n");
    printf("3. Listar clientes\n");
    printf("4. Débito\n");
    printf("5. Depósito\n");
    printf("6. Extrato\n");
    printf("7. Transferência Entre Contas\n");
    printf("0. Sair\n");
    printf("====================================\n");
    printf("Escolha uma opção: ");
}

int main() {
    int opcao;
    char cpf[MAX_CPF];
    char senha[MAX_SENHA];
    float valor;
    Cliente c;

    carregarClientesArquivo();
    carregarTransacoesArquivo();

    while (1) {
        menu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Nome: ");
                scanf("%s", c.nome);
                printf("CPF: ");
                scanf("%s", c.cpf);
                printf("Senha: ");
                scanf("%s", c.senha);
                printf("Tipo de conta (0 para COMUM, 1 para PLUS): ");
                scanf("%d", (int*)&c.tipo);
                printf("Valor inicial: ");
                scanf("%f", &c.saldo);
                adicionarCliente(c);
                break;

            case 2:
                printf("CPF do cliente a ser removido: ");
                scanf("%s", cpf);
                removerCliente(cpf);
                break;

            case 3:
                listarClientes();
                break;

            case 4:
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Senha: ");
                scanf("%s", senha);
                printf("Valor: ");
                scanf("%f", &valor);
                debitar(cpf, senha, valor);
                break;

            case 5:
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Valor: ");
                scanf("%f", &valor);
                depositar(cpf, valor);
                break;

            case 6:
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Senha: ");
                scanf("%s", senha);
                extrato(cpf, senha);
                break;

            case 7:
                char cpfDestino[MAX_CPF];
                printf("CPF (Origem): ");
                scanf("%s", cpf);
                printf("Senha (Origem): ");
                scanf("%s", senha);
                printf("CPF (Destino): ");
                scanf("%s", cpfDestino);
                printf("Valor: ");
                scanf("%f", &valor);
                transferir(cpf, senha, cpfDestino, valor);
                break;

            case 0:
                salvarClientesArquivo();
                salvarTransacoesArquivo();
                exit(0);
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
