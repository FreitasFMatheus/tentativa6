#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

// Autores do programa:
// Matheus Ferreira de Freitas - 24.123.080-4
// Rafael Russo Mingroni - 24.123.054-9

// Função que exibe o menu de opções para o usuário.
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
    char cpf[MAX_CPF_SIZE];
    char senha[MAX_SENHA_SIZE];
    float valor;
    Cliente c;

    // Cria a estrutura BancoDados
    BancoDados bancoDados;
    bancoDados.numClientes = 0;
    bancoDados.numTransacoes = 0;

    // Carrega informações de clientes e transações dos respectivos arquivos ao iniciar o programa.
    carregarClientesArquivo(&bancoDados);
    carregarTransacoesArquivo(&bancoDados);

    while (1) {
        menu();  // Exibe o menu de opções.
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
                adicionarCliente(&bancoDados, c);
                break;

            case 2:
                printf("CPF do cliente a ser removido: ");
                scanf("%s", cpf);
                removerCliente(&bancoDados, cpf);
                break;

            case 3:
                listarClientes(&bancoDados);
                break;

            case 4:
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Senha: ");
                scanf("%s", senha);
                printf("Valor: ");
                scanf("%f", &valor);
                debitar(&bancoDados, cpf, senha, valor);
                break;

            case 5:
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Valor: ");
                scanf("%f", &valor);
                depositar(&bancoDados, cpf, valor);
                break;

            case 6:
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Senha: ");
                scanf("%s", senha);
                extrato(&bancoDados, cpf, senha);
                break;

            case 7:
                char cpfDestino[MAX_CPF_SIZE];
                printf("CPF (Origem): ");
                scanf("%s", cpf);
                printf("Senha (Origem): ");
                scanf("%s", senha);
                printf("CPF (Destino): ");
                scanf("%s", cpfDestino);
                printf("Valor: ");
                scanf("%f", &valor);
                transferir(&bancoDados, cpf, senha, cpfDestino, valor);
                break;

            case 0:
                salvarClientesArquivo(&bancoDados);
                salvarTransacoesArquivo(&bancoDados);
                exit(0);
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;
}
