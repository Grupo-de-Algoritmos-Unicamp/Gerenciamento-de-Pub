#ifndef PEDIDO_H
#define PEDIDO_H

int processarPagamento(float valorTotal, float *pagoCliente, float *troco);
void gerarArquivoPedido(char cpf[], int numeroPedido, char nome[][31], float preco[], int quantidade[], int i, float valorTotal, int tipoPagamento, float pagoCliente, float troco);
void registrarPedido();

#endif

