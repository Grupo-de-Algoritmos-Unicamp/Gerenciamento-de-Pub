#include <stdio.h>
#include "estoque.h"
#include "pedido.h"

int processarPagamento(float valorTotal, float *pagoCliente, float *troco) {
    int tipoPagamento;
    char trocoSN;

    do {
        printf("Qual sera a forma de pagamento: \n");
        printf("(1) PIX \n");
        printf("(2) Cartao de Credito/Debito \n");
        printf("(3) Dinheiro \n");
        printf("Opcao: ");
        scanf("%d", &tipoPagamento);

        switch (tipoPagamento) {
            case 1:
                printf("\nGere o codigo na maquininha\n");
                break;
            case 2:
                printf("\nSelecione credito ou debito na maquininha, e peca para o cliente inserir ou aproximar o cartao na maquininha\n");
                break;
            case 3:
                printf("\nAbra a caixa registradora e receba o dinheiro do cliente!\n");
                printf("Eh preciso devolver troco ao cliente? <s/n> ");
                scanf(" %c", &trocoSN);

                if (trocoSN == 's' || trocoSN == 'S') {
                    printf("\nDigite a quantia que o cliente forneceu em nota: ");
                    scanf("%f", pagoCliente);

                    *troco = *pagoCliente - valorTotal;

                    if (*troco < 0) {
                        printf("\nO valor fornecido eh insuficiente! Falta R$%.2f\n", -*troco);
                        *troco = 0; 
                    } else {
                        printf("\nDevolva o troco de R$%.2f ao cliente\n", *troco);
                    }
                }
                break;
            default:
                printf("\nOpcao de pagamento invalida! TENTE NOVAMENTE!\n\n");
                continue;
        }
        break;
    } while (1);

    return tipoPagamento;
}

void gerarArquivoPedido(char cpf[], int numeroPedido, char nome[][31], float preco[], int quantidade[], int i, float valorTotal, int tipoPagamento, float pagoCliente, float troco) {
    char nomeArquivo[50];
    sprintf(nomeArquivo, "pedido_%d.txt", numeroPedido);

    FILE *pedido = fopen(nomeArquivo, "w");
    if (pedido == NULL) {
        printf("Erro ao criar o arquivo do pedido!\n");
        return;
    }

    fprintf(pedido, "----------PEDIDO----------\n\n");
    fprintf(pedido, "CPF do Cliente: %s\n", cpf);
    fprintf(pedido, "Numero do pedido: %d\n\n", numeroPedido);
    fprintf(pedido, "Lista de Produtos:\n\n");

    for (int j = 0; j < i; j++) {
        fprintf(pedido, "Produto %d\n", j + 1);
        fprintf(pedido, "Nome: %s\n", nome[j]);
        fprintf(pedido, "Quantidade: %d\n", quantidade[j]);
        fprintf(pedido, "Preco total: R$ %.2f\n\n", preco[j]);
    }

    fprintf(pedido, "Total: R$ %.2f\n\n", valorTotal);

    fprintf(pedido, "Forma de pagamento: ");

    if (tipoPagamento == 1) {
        fprintf(pedido, "PIX\n");
    } else if (tipoPagamento == 2) {
        fprintf(pedido, "Cartao\n");
    } else {
        fprintf(pedido, "Dinheiro\n");
        if (troco > 0) {
            fprintf(pedido, "Cliente forneceu R$ %.2f\n", pagoCliente);
            fprintf(pedido, "Troco: R$ %.2f\n", troco);
        }
    }

    fclose(pedido);
    printf("\nPedido registrado com sucesso! Arquivo: %s\n\n", nomeArquivo);
}

void gerarArquivoFidelidade(char cpf[], float valorTotal, int quantidadeTotal) {
    FILE *arquivo = fopen("fidelidade.txt", "a"); 

    if (arquivo == NULL) {
        printf("Erro ao gerar arquivo fidelidade!\n");
        return;
    }

    fprintf(arquivo, "%s %.2f %d\n", cpf, valorTotal, quantidadeTotal);

    fclose(arquivo);
    printf("Registro de fidelidade salvo para o cliente %s\n", cpf);
}

void registrarPedido() {
    char cpf[15], pedidoSN;
    int tipoPagamento, numeroPedido = 1, i;
    float valorTotal, pagoCliente, troco;

    do {
        valorTotal = 0; 
        pagoCliente = 0;
        troco = 0;

        printf("Digite o CPF do cliente <utilizando pontos e hifen>: ");
        scanf("%s", cpf);

        printf("Digite quantos produtos o cliente comprou: ");
        scanf("%d", &i);

        char nome[i][31];
        int quantidade[i], quantidadeTotal = 0;
        float preco[i];

        for (int j = 0; j < i; j++) {
            printf("\nDigite o nome do produto %d: ", j + 1);
            scanf(" %[^\n]", nome[j]);

            printf("Digite o preco unitario do produto %d: ", j + 1);
            scanf("%f", &preco[j]);

            printf("Digite a quantidade do produto %d: ", j + 1);
            scanf("%d", &quantidade[j]);

            quantidadeTotal += quantidade[j];
            preco[j] *= quantidade[j]; 
            valorTotal += preco[j];

            atualizarEstoque(nome[j], quantidade[j], 1);

        }

        printf("\nTotal: R$ %.2f\n\n", valorTotal);

        tipoPagamento = processarPagamento(valorTotal, &pagoCliente, &troco);

        gerarArquivoPedido(cpf, numeroPedido, nome, preco, quantidade, i, valorTotal, tipoPagamento, pagoCliente, troco);

        gerarArquivoFidelidade(cpf, valorTotal, quantidadeTotal);

        numeroPedido++;

        printf("Deseja gerar um novo pedido <s/n>? ");
        scanf(" %c", &pedidoSN);

        printf("\n");

    } while (pedidoSN == 's' || pedidoSN == 'S');

    printf("Saindo...\n");
}

int main() {
    registrarPedido();
    return 0;
}
