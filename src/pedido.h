#ifndef PEDIDO_H
#define PEDIDO_H

int gerarProximoIDPedido();
void gerarArquivoPedidos(int id, const char *cpf, int qtdProdutos, float total);
void gerarArquivoItensVendidos(int id, char nomes[][31], int quant[], float unit[], float total[], int n);
void processarPagamento(float valorTotal, float *pagoCliente, float *troco);
int verificarProdutosPedido(int qtdProdutos, char nomes[][31], int quantidades[], float precoUnit[], float precoTotal[], float *valorTotal);
int atualizarEstoquePedido(int qtdProdutos, char nomes[][31], int quantidades[]);
void registrarPedido();

#endif


