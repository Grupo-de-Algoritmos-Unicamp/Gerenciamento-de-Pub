#ifndef UTILS_H
#define UTILS_H

FILE* abrirArquivoClientes(int modo);

typedef struct Pedido Pedido;
typedef struct Produto Produto;
typedef struct ResumoProduto ResumoProduto;

void menuInicial();
void sair();
void cadastrarCliente();
void consultarCliente();
void lerDadosRelatorio(Pedido **pedidos, size_t *numPedidos, Produto **itens, int **idsItens, size_t *numItens, int filtrar, const char *dataFiltro);
void gerarArquivoRelatorio(const char *dataFiltro, int filtrar, ResumoProduto *resumo, size_t numResumo, float totalVendas, size_t numPedidos);
void gerarRelatorio();

float calcularValorGasto(const char* buscaCpf);

#endif

