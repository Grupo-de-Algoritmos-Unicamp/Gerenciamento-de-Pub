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

void processarPagamento(float valorTotal, float *pagoCliente, float *troco) {
    int tipoPagamento;
    char trocoSN;

    do {
        printf("Qual será a forma de pagamento:\n(1) PIX\n(2) Cartão de Crédito/Débito\n(3) Dinheiro \n");
        printf("Opção: ");
        scanf("%d", &tipoPagamento);

        switch (tipoPagamento) {
            case 1:
                printf("\nGere o código na maquininha\n");
                break;
            case 2:
                printf("\nSelecione crédito ou débito na maquininha, e peça para o cliente inserir ou aproximar o cartão na maquininha\n");
                break;
            case 3:
                printf("\nAbra a caixa registradora e receba o dinheiro do cliente!\n");
                printf("É preciso devolver troco ao cliente? <s/n> ");
                scanf(" %c", &trocoSN);

                if (trocoSN == 's' || trocoSN == 'S') {
                    printf("\nDigite a quantia que o cliente forneceu: ");
                    scanf("%f", pagoCliente);

                    *troco = *pagoCliente - valorTotal;

                    if (*troco < 0) {
                        printf("\nO valor fornecido é insuficiente! Falta R$%.2f\n", -*troco);
                        *troco = 0; 
                    } else {
                        printf("\nDevolva o troco de R$%.2f ao cliente\n", *troco);
                    }
                }
                break;
            default:
                printf("\nOpcao de pagamento inválida! TENTE NOVAMENTE!\n\n");
                continue;
        }
        break;
    } while (1);
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
    char cpf[32], continuar = 'n';
    float valorTotal, pagoCliente, troco;

    do {
        printf("\nDigite o CPF do cliente: ");
        scanf("%s", cpf);

        int qtdProdutos;
        printf("Digite a quantidade de produtos distintos: ");
        scanf("%d", &qtdProdutos);

        if (qtdProdutos <= 0) {
            printf("Quantidade invalida.\n");
            return;
        }

        char nomes[qtdProdutos][31];
        int quantidades[qtdProdutos];
        float precoUnit[qtdProdutos], precoTotal[qtdProdutos];

        // Verificação antes de atualizar o estoque
        if (!verificarProdutosPedido(qtdProdutos, nomes, quantidades, precoUnit, precoTotal, &valorTotal)){
            return;
        } 
        
        // Atualiza o estoque
        if (!atualizarEstoquePedido(qtdProdutos, nomes, quantidades)){
            return;
        }

        // Processamento e gravação
        printf("\nTotal: R$ %.2f\n", valorTotal);
        pagoCliente = 0;
        troco = 0;
        processarPagamento(valorTotal, &pagoCliente, &troco);

        int idPedido = gerarProximoIDPedido();
        gerarArquivoPedidos(idPedido, cpf, qtdProdutos, valorTotal);
        gerarArquivoItensVendidos(idPedido, nomes, quantidades, precoUnit, precoTotal, qtdProdutos);

        printf("\nPedido %03d registrado com sucesso!\n", idPedido);

        printf("Deseja registrar outro pedido? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    printf("\nSaindo...\n");
}
