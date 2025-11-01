#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "estoque.h"
#include "pedido.h"

//Garante que ao abrir o programa novamente o número do pedido não recomece em 1
int gerarProximoIDPedido() {
    FILE *f = fopen("pedidos.txt", "r");

    if (f==NULL){
        return 1; //Retorna 1 se o arquivo pedidos.txt estiver vazio
    } 

    int ultimoID = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), f)) {
        int id;
        if (sscanf(linha, "%d;", &id) == 1)
            if (id > ultimoID) ultimoID = id;
    }

    fclose(f);
    return ultimoID + 1;
}

void gerarArquivoPedidos(int id, const char *cpf, int qtdProdutos, float total) {
    FILE *arquivo = fopen("pedidos.txt", "a");

    if (!arquivo) {
        printf("Erro ao abrir pedidos.txt!\n");
        return;
    }

    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    char data[20];
    sprintf(data, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);

    fprintf(arquivo, "%03d %s %s %d %.2f\n", id, cpf, data, qtdProdutos, total);
    fclose(arquivo);
}

void gerarArquivoItensVendidos(int id, char nomes[][31], int quant[], float unit[], float total[], int n) {
    FILE *arquivo = fopen("itens_vendidos.txt", "a");
    if (!arquivo) {
        printf("Erro ao abrir itens_vendidos.txt!\n");
        return;
    }

    for (int i = 0; i < n; i++)
        fprintf(arquivo, "%03d %s %d %.2f %.2f\n", id, nomes[i], quant[i], unit[i], total[i]);

    fclose(arquivo);
}

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

int verificarProdutosPedido(int qtdProdutos, char nomes[][31], int quantidades[], float precoUnit[], float precoTotal[], float *valorTotal) {
    *valorTotal = 0;

    for (int i = 0; i < qtdProdutos; i++) {
        printf("\nNome do produto %d: ", i + 1);
        scanf(" %30[^\n]", nomes[i]);

        printf("Quantidade: ");
        scanf("%d", &quantidades[i]);

        if (quantidades[i] <= 0) {
            printf("Quantidade invalida.\n");
            return 0;
        }

        float preco;
        int disponivel;
        if (!obterPrecoQuantidade(nomes[i], &preco, &disponivel)) {
            printf("Produto '%s' não encontrado! Pedido cancelado.\n", nomes[i]);
            return 0;
        }

        if (disponivel < quantidades[i]) {
            printf("Estoque insuficiente de '%s'! Pedido cancelado.\n", nomes[i]);
            return 0;
        }

        precoUnit[i] = preco;
        precoTotal[i] = preco * quantidades[i];
        *valorTotal += precoTotal[i];
    }
    return 1;
}

int atualizarEstoquePedido(int qtdProdutos, char nomes[][31], int quantidades[]) {
    for (int i = 0; i < qtdProdutos; i++) {
        int r = atualizarEstoque(nomes[i], quantidades[i], 1);
        if (r != 1) {
            printf("Falha ao atualizar estoque de '%s'.\n", nomes[i]);
            return 0;
        }
    }
    return 1;
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


