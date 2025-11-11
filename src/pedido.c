#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "estoque.h"
#include "pedido.h"

#define ARQUIVO_PEDIDOS "pedidos.txt"
#define ARQUIVO_ITENS "itens_vendidos.txt"

FILE* abrirArquivoPedidos(const char* modo) {
    FILE* arquivo = fopen(ARQUIVO_PEDIDOS, modo);
    if (arquivo == NULL) {
        printf("Erro ao abrir %s no modo %s!\n", ARQUIVO_PEDIDOS, modo);
    }
    return arquivo;
}

FILE* abrirArquivoItens(const char* modo) {
    FILE* arquivo = fopen(ARQUIVO_ITENS, modo);
    if (arquivo == NULL) {
        printf("Erro ao abrir %s no modo %s!\n", ARQUIVO_ITENS, modo);
    }
    return arquivo;
}


//Garante que ao abrir o programa novamente o número do pedido não recomece em 1
int gerarProximoIDPedido() {
    FILE *f = abrirArquivoPedidos("r");

    if (f==NULL){
        return 1; //Retorna 1 se o arquivo pedidos.txt não existir
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
    FILE *arquivo = abrirArquivoPedidos("a");

    if (!arquivo) {
        printf("Erro ao abrir pedidos.txt!\n");
        return;
    }

    time_t agora = time(NULL);
    struct tm *t = localtime(&agora);
    char data[20];
    strftime(data, 20, "%d/%m/%Y", t);

    fprintf(arquivo, "%03d;%s;%s;%d;%.2f\n", id, cpf, data, qtdProdutos, total);
    fclose(arquivo);
}

void gerarArquivoItensVendidos(Pedido *pedido) {
    FILE *arquivo = abrirArquivoItens("a");
    if (!arquivo) {
        printf("Erro ao abrir itens_vendidos.txt!\n");
        return;
    }

    for (int i = 0; i < pedido->quantidadeProdutos; i++)
        fprintf(arquivo, "%03d;%s;%d;%.2f;%.2f\n", pedido->id, pedido->produto[i].nome, pedido->produto[i].quantidade, pedido->produto[i].precoUnitario, pedido->produto[i].precoTotal);
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

int verificarProdutosPedido(Pedido *pedido) {
    pedido->valorTotal = 0;

    for (int i = 0; i < pedido->quantidadeProdutos; i++) {
        printf("\nNome do produto %d: ", i + 1);
        scanf(" %20[^\n]", pedido->produto[i].nome);

        printf("Quantidade: ");
        if (scanf("%d", &pedido->produto[i].quantidade) != 1 || pedido->produto[i].quantidade <= 0) {
            printf("Quantidade inválida.\n");
            return 0;
        }
        
        float preco;
        int disponivel;

        if (!obterPrecoQuantidadePorNome(pedido->produto[i].nome, &preco, &disponivel)) {
            char opcao;
            printf("Produto '%s' não encontrado no estoque.\n", pedido->produto[i].nome);
            printf("Deseja procurar pelo código? (s/n): ");
            scanf(" %c", &opcao);

            if (opcao == 's' || opcao == 'S') {
                int codigoBusca;
                printf("Digite o código do produto: ");
                scanf("%d", &codigoBusca);

                if (!obterPrecoQuantidadePorCodigo(codigoBusca, &preco, &disponivel, pedido->produto[i].nome)) {
                    printf("Código %d também não encontrado. Pedido cancelado.\n", codigoBusca);
                    return 0;
                }
            } else {
                printf("Produto ignorado. Pedido cancelado.\n");
                return 0;
            }
        }
        
        if (disponivel < pedido->produto[i].quantidade) {
            printf("Estoque insuficiente de '%s'! Há apenas %d unidades disponíveis.\n", pedido->produto[i].nome, disponivel);
            
            char opcao;
            printf("Deseja manter o produto com %d unidades disponíveis? (s/n): ", disponivel);
            scanf(" %c", &opcao);

            if (opcao == 's' || opcao == 'S') {
                pedido->produto[i].quantidade = disponivel;
                printf("Quantidade ajustada para %d unidades.\n", disponivel);
            } else {
                printf("Produto '%s' removido do pedido.\n", pedido->produto[i].nome);
                return 0;
            }
        }

        pedido->produto[i].precoUnitario = preco;
        pedido->produto[i].precoTotal = preco * pedido->produto[i].quantidade;
        pedido->valorTotal += pedido->produto[i].precoTotal;
    }
    return 1;
}

int atualizarEstoquePedido(Pedido *pedido) {
    for (int i = 0; i < pedido->quantidadeProdutos; i++) {
        int r = atualizarEstoque(pedido->produto[i].nome, pedido->produto[i].quantidade, 1);
        if (r != 1) {
            printf("Falha ao atualizar estoque de '%s'.\n", pedido->produto[i].nome);
            return 0;
        }
    }
    return 1;
}

void registrarPedido() {
    Pedido pedido;
    char continuar = 'n';
    float pagoCliente = 0, troco = 0;

    do {
        printf("\nDigite o CPF do cliente: ");
        scanf("%14s", pedido.cpf);

        printf("Digite a quantidade de produtos distintos: ");
        if (scanf("%d", &pedido.quantidadeProdutos) != 1 || pedido.quantidadeProdutos <= 0) {
            printf("Quantidade inválida.\n");
            return;
        }

        pedido.produto = malloc(pedido.quantidadeProdutos * sizeof(ItemPedido));
        if (!pedido.produto) {
            printf("Erro de memória.\n");
            return;
        }

        // Verificação antes de atualizar o estoque
        if (!verificarProdutosPedido(&pedido)){
            free(pedido.produto);
            return;
        } 
        
        // Atualiza o estoque
        if (!atualizarEstoquePedido(&pedido)){
            free(pedido.produto);
            return;
        }

        // Processamento e gravação
        printf("\nTotal: R$ %.2f\n", pedido.valorTotal);
        processarPagamento(pedido.valorTotal, &pagoCliente, &troco);

        pedido.id = gerarProximoIDPedido();
        gerarArquivoPedidos(pedido.id, pedido.cpf, pedido.quantidadeProdutos, pedido.valorTotal);
        gerarArquivoItensVendidos(&pedido);

        printf("\nPedido %03d registrado com sucesso!\n", pedido.id);

        free(pedido.produto);

        printf("Deseja registrar outro pedido? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    printf("\nSaindo...\n");
}


