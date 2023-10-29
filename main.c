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
    // Declaração de variáveis utilizadas para armazenar dados temporários e a opção do menu escolhida pelo usuário.
    int opcao;
    char cpf[MAX_CPF_SIZE];
    char senha[MAX_SENHA_SIZE];
    float valor;
    Cliente c;

    // Carrega informações de clientes e transações dos respectivos arquivos ao iniciar o programa.
    carregarClientesArquivo();
    carregarTransacoesArquivo();

    // Loop infinito para o programa continuar em execução até que o usuário opte por sair.
    while (1) {
        menu();  // Exibe o menu de opções.
        scanf("%d", &opcao);

        // Estrutura de seleção para tratar a opção escolhida pelo usuário.
        switch (opcao) {
            case 1:  // Opção para adicionar um novo cliente.
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

            case 2:  // Opção para remover um cliente existente.
                printf("CPF do cliente a ser removido: ");
                scanf("%s", cpf);
                removerCliente(cpf);
                break;

            case 3:  // Opção para listar todos os clientes.
                listarClientes();
                break;

            case 4:  // Opção para realizar um débito em uma conta.
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Senha: ");
                scanf("%s", senha);
                printf("Valor: ");
                scanf("%f", &valor);
                debitar(cpf, senha, valor);
                break;

            case 5:  // Opção para realizar um depósito em uma conta.
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Valor: ");
                scanf("%f", &valor);
                depositar(cpf, valor);
                break;

            case 6:  // Opção para exibir o extrato de uma conta.
                printf("CPF: ");
                scanf("%s", cpf);
                printf("Senha: ");
                scanf("%s", senha);
                extrato(cpf, senha);
                break;

            case 7:  // Opção para realizar uma transferência entre contas.
                char cpfDestino[MAX_CPF_SIZE];
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

            case 0:  // Opção para sair do programa.
                salvarClientesArquivo();         // Salva as informações de clientes no arquivo antes de sair.
                salvarTransacoesArquivo();       // Salva as informações de transações no arquivo antes de sair.
                exit(0);                         // Termina a execução do programa.
                break;

            default:  // Caso o usuário insira uma opção que não esteja listada no menu.
                printf("Opção inválida. Tente novamente.\n");
        }
    }

    return 0;  // Retorna 0 ao finalizar a execução do programa (embora, devido ao loop, este ponto não seja alcançado).
}
