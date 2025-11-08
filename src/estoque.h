#ifndef ESTOQUE_H
#define ESTOQUE_H

FILE* abrirArquivoEstoque(int modo);

void menuCadastroProduto();
void menuConsultarProdutos();

void cadastrarProduto();
void alterarProduto();
void excluirProduto();
void listarTodos();
void listarBebidas();
void listarComidas();
void consultarProdutoPorCodigo();

int verificarProduto(int codigo);
int atualizarEstoque(char nomeProduto[], int quantidadeAlterar, int modo);
int obterPrecoQuantidadePorNome(const char nomeProduto[], float *precoUnitario, int *quantidadeDisponivel);
int obterPrecoQuantidadePorCodigo(int codigoBusca, float *precoUnitario, int *quantidadeDisponivel, char *nomeProduto);

typedef struct {
    int codigo;
    char tipo;
    char nome[100];
    float preco;
    int quantidade;
} Produtos;

#endif



