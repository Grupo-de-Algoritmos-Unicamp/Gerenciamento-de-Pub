#ifndef ESTOQUE_H
#define ESTOQUE_H

void menuCadastroProduto();
void menuConsultarProdutos();

void cadastrarProduto();
void mostrarEstoque();
void alterarProduto();
void excluirProduto();
int atualizarEstoque(char nomeProduto[], int quantidadeAlterar, int modo);
int obterPrecoQuantidade(const char nomeProduto[], float *precoUnitario, int *quantidadeDisponivel);
int obterPrecoQuantidadePorCodigo(int codigoBusca, float *precoUnitario, int *quantidadeDisponivel, char *nomeProduto);

#endif



