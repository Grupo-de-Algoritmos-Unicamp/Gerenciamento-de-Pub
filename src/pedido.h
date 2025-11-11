#ifndef PEDIDO_H
#define PEDIDO_H
#include "estoque.h"

typedef struct{
    char nome[21];
    int quantidade;
    float precoUnitario;
    float precoTotal;
} ItemPedido;

typedef struct{
    int id;
    char cpf[32];
    char data[20];
    int quantidadeProdutos;
    float valorTotal;
    ItemPedido *produto;
} Pedido;

FILE* abrirArquivosPedidos(int modo);

void registrarPedido();
void gerarArquivoPedidos(int id, const char *cpf, int qtdProdutos, float total);
void gerarArquivoItensVendidos(Pedido *pedido);
void processarPagamento(float valorTotal, float *pagoCliente, float *troco);

int gerarProximoIDPedido();
int verificarProdutosPedido(Pedido *pedido);
int atualizarEstoquePedido(Pedido *pedido);

#endif



