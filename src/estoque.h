#ifndef ESTOQUE_H
#define ESTOQUE_H

void menuCadastroProduto();
void menuConsultarProdutos();

void cadastrarProduto();
void mostrarEstoque();
void atualizarEstoque(char nomeProduto[], int quantidadeAlterar, int modo);
void alterarProduto();
void excluirProduto();

#endif
