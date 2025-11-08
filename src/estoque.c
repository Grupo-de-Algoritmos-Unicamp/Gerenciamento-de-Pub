#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "estoque.h"

FILE* abrirArquivoEstoque(int modo) {
    // 1:estoque-a  2:estoque-r 3:tempAlterar-w  4:tempExcluir-w
    FILE* arquivo = NULL;
    switch (modo) {
        case 1: arquivo = fopen("estoque.txt", "a"); break;
        case 2: arquivo = fopen("estoque.txt", "r"); break;
        case 3: arquivo = fopen("estoque.txt", "w"); break;
        case 4: arquivo = fopen("tempAlterar.txt", "w"); break;
        case 5: arquivo = fopen("tempExcluir.txt", "w"); break;
        default: printf("Modo inválido.\n");
    }

    if (arquivo == NULL)
        printf("Erro ao abrir o arquivo.\n");

    return arquivo;
}

int verificarProduto(int codigo) {
    Produtos produtoLido;
    int encontrado = 0;

    FILE *arquivo = abrirArquivoEstoque(2);
    if (arquivo == NULL){
        return 0;
    }

    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d\n", &produtoLido.codigo, &produtoLido.tipo, produtoLido.nome, &produtoLido.preco, &produtoLido.quantidade) == 5) {
        if (produtoLido.codigo == codigo) {
            encontrado = 1;
            return 1;
        }
    }

    fclose(arquivo);
    return encontrado;
}

//CADASTRO DE PRODUTOS----------------------------------------------------------------------------------------------
void cadastrarProduto(){
    Produtos produto;

    printf("\n-------Cadastro de Produtos-------\n");
    printf("Digite o código do produto (até 6 números): ");
    scanf("%d", &produto.codigo);

    //Verifica se existe um produto com esse código no estoque
    int encontrado = verificarProduto(produto.codigo);
    if(encontrado == 1){
        printf("Já existe um produto com o código %d.\n", produto.codigo);
        return;
    }

    //Se não existe, continua normalmente
    printf("Digite o tipo de produto ('C' para comidas, 'B' para bebidas): ");
    scanf(" %c", &produto.tipo);
    printf("Digite o nome do produto: ");
    scanf(" %[^\n]", produto.nome);
    printf("Digite o preço do produto: ");
    scanf("%f", &produto.preco);
    printf("Digite a quantidade do produto: ");
    scanf("%d", &produto.quantidade);

    FILE *arquivo = abrirArquivoEstoque(1); //("estoque.txt", "a")
    if(arquivo==NULL){
        return;
    }
    fprintf(arquivo, "%06d;%c;%s;%.2f;%d\n", produto.codigo, produto.tipo, produto.nome, produto.preco, produto.quantidade);
    fclose(arquivo);

    printf("Produto cadastrado com sucesso!\n");

}

//ALTERAÇÃO DE PRODUTOS----------------------------------------------------------------------------------------------
void menuAlterarProduto(Produtos *produto) {
    int opcao=0;
    do{
        printf("\n\n--- Produto Encontrado ---\n");
        printf("Tipo: %c\nNome: %s\nPreço: R$%.2f\nQuantidade: %d\n", produto->tipo, produto->nome, produto->preco, produto->quantidade);
        printf("--------------------------\n");
        printf("O que deseja alterar?\n");
        printf("1. Nome\n2. Preço\n3. Tipo de produto\n4. Quantidade no estoque\n0. Salvar alterações e Voltar ao menu\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                printf("Digite o novo nome: ");
                scanf(" %[^\n]", produto->nome); 
                break;
            case 2:
                printf("Digite o novo preço: ");
                scanf(" %f", &produto->preco); 
                break;
            case 3:
                printf("Digite o novo tipo de produto: ");
                scanf(" %c", &produto->tipo);
                break;
            case 4:
                printf("Digite a nova quantidade do produto: ");
                scanf(" %d", &produto->quantidade);
                break;
            case 0:
                printf("Alterações salvas.\n");
                break;
            default:
                printf("Opção inválida\n");
                break;
        }
    }while(opcao!=0);
}

void alterarProduto(){
    int codigoAlterar;
    int encontrado = 0;
    Produtos produto;

    FILE *arquivo=abrirArquivoEstoque(2); //("estoque.txt", "r");
    FILE *arqTemp=abrirArquivoEstoque(4); //("tempAlterar.txt", "w");

    if((arquivo==NULL)||(arqTemp==NULL)){
        if(arquivo==NULL){
            fclose(arqTemp);
        }if(arqTemp==NULL){
            fclose(arquivo);
        }
        return;
    }

    
    printf("\n\n-------Alterar Produto-------\n");
    printf("Digite o código do produto que deseja alterar: ");
    scanf("%d", &codigoAlterar);
    
    //Percorre o arquivo de texto buscando pelo produto que possui o código
    while(fscanf(arquivo, "%d;%c;%[^;];%f;%d\n", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade)==5){
        if(produto.codigo==codigoAlterar){
            encontrado=1;
            //Chama a função menuAlterarProduto para alterar o arquivo
            menuAlterarProduto(&produto);
        }
        // Escreve a linha no arquivo temporário.
        fprintf(arqTemp, "%d;%c;%s;%.2f;%d\n", produto.codigo, produto.tipo, produto.nome, produto.preco, produto.quantidade);
    }

    fclose(arquivo);
    fclose(arqTemp);
    
    if(encontrado){
        //Se o produto foi encontrado, apaga 'estoque.txt', e renomeia 'temp.txt' para ser o novo 'estoque.txt'
        remove("estoque.txt");
        rename("tempAlterar.txt", "estoque.txt");
        printf("\nProduto alterado com sucesso!\n");
    }else{
        //Se não foi encontrado o produto, apenas apaga o arquivo temporário que foi criado.
        remove("tempAlterar.txt");
        printf("\nProduto com código %d não encontrado.\n", codigoAlterar);
    }
}

//EXCLUSÃO DE PRODUTOS ----------------------------------------------------------------------------
void excluirProduto(){
    FILE *arquivo = abrirArquivoEstoque(2);
    FILE *arqTemp = abrirArquivoEstoque(5);

    if(arquivo==NULL || arqTemp == NULL){
        return;
    }

    Produtos produto;

    int codigoExcluir, encontrado = 0;

    printf("\n-------Excluir Produto-------\n");
    printf("Digite o código do produto que deseja excluir: ");
    scanf("%d", &codigoExcluir);
    
    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d\n", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade) == 5) {
        if (produto.codigo == codigoExcluir) {
            encontrado = 1;
            printf("Produto '%s' removido.\n", produto.nome);
        } else {
            fprintf(arqTemp, "%d;%c;%s;%.2f;%d\n", produto.codigo, produto.tipo, produto.nome, produto.preco, produto.quantidade);
        }
    }

    fclose(arquivo);
    fclose(arqTemp);

    if (encontrado) {
        remove("estoque.txt");
        rename("tempExcluir.txt", "estoque.txt");
    } else {
        remove("tempExcluir.txt");
        printf("Produto com código %d não encontrado.\n", codigoExcluir);
    }
}

//LISTA DE PRODUTOS---------------------------------------------------------------------------------------------------
void listarTodos()
{
    FILE* arquivo = abrirArquivoEstoque(2);
    if(arquivo == NULL){
        return;
    }

    Produtos produto;
    
    printf("\n------ Lista de Produtos ------\n");
    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d\n", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade) == 5) {
        printf("Código:%d | Tipo:%c | Nome:%s | R$%.2f | Qtd:%d\n", produto.codigo, produto.tipo, produto.nome, produto.preco, produto.quantidade);
    }
    fclose(arquivo);
}

//listar apenas as bebidas do estoque
void listarBebidas(){
    FILE* arquivo = abrirArquivoEstoque(2);
    if(arquivo == NULL){
        return;
    }

    Produtos produto;

    printf("\n------ Lista de Bebidas ------\n");
    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d\n", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade) == 5) {
        if (produto.tipo == 'B' || produto.tipo == 'b')
            printf("%d | %s | R$%.2f | %d un\n", produto.codigo, produto.nome, produto.preco, produto.quantidade);
    }

    fclose(arquivo);
}

//listar todas as comidas do estoque
void listarComidas(){
    FILE *arquivo = abrirArquivoEstoque(2);
    if (arquivo == NULL){
        return;
    } 

    Produtos produto;
    printf("\n------ Lista de Comidas ------\n");
    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d\n", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade) == 5) {
        if (produto.tipo == 'C' || produto.tipo == 'c')
            printf("%d | %s | R$%.2f | %d un\n", produto.codigo, produto.nome, produto.preco, produto.quantidade);
    }
    fclose(arquivo);
}

void consultarProdutoPorCodigo(){
    FILE *arquivo = abrirArquivoEstoque(2);
    if (arquivo == NULL){
        return;
    }

    Produtos produto;
    int codigoProcurado, encontrado = 0;
    printf("\nDigite o código do produto: ");
    scanf("%d", &codigoProcurado);

    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d\n", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade) == 5) {
        if (produto.codigo == codigoProcurado) {
            printf("Código:%d | Tipo:%c | Nome:%s | R$%.2f | Qtd:%d\n", produto.codigo, produto.tipo, produto.nome, produto.preco, produto.quantidade);
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("Produto não encontrado.\n");

    fclose(arquivo);
}

//FUNÇÕES AUXILIARES PARA PEDIDOS---------------------------------------------------------------------------------------------------
int obterPrecoQuantidadePorNome(const char nomeProduto[], float *precoUnitario, int *quantidadeDisponivel) {
    FILE *arquivo = abrirArquivoEstoque(2);
    if (!arquivo) return 0;

    Produtos produto;

    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade) != EOF) {
        if (strcmp(produto.nome, nomeProduto) == 0) {
            *precoUnitario = produto.preco;
            *quantidadeDisponivel = produto.quantidade;
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return 0;
}

int obterPrecoQuantidadePorCodigo(int codigoBusca, float *precoUnitario, int *quantidadeDisponivel, char *nomeProduto) {
    FILE *arquivo = abrirArquivoEstoque(2);
    if (!arquivo) return 0;

    Produtos produto;

    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade) != EOF) {
        if (produto.codigo == codigoBusca) {
            *precoUnitario = produto.preco;
            *quantidadeDisponivel = produto.quantidade;
            strcpy(nomeProduto, produto.nome);
            fclose(arquivo);
            return 1;
        }
    }

    fclose(arquivo);
    return 0;
}

//ATUALIZAR ESTOQUE---------------------------------------------------------------------------------------------------
int atualizarEstoque(char nomeProduto[], int quantidadeAlterar, int modo) {
    // modo = 1 -> venda (subtrai)
    // modo = 2 -> reposição (soma)

    FILE *arquivo = abrirArquivoEstoque(2); // modo leitura
    if (arquivo == NULL) {
        printf("estoque.txt não encontrado.\n");
        return 0;
    }

    Produtos *lista = NULL;
    size_t total = 0, capacidade = 0;
    int encontrado = 0;
    int indiceEncontrado = -1;

    // Lê todos os produtos do arquivo e armazena em memória
    Produtos produto;
    while (fscanf(arquivo, "%d;%c;%[^;];%f;%d\n", &produto.codigo, &produto.tipo, produto.nome, &produto.preco, &produto.quantidade) == 5) {
        
        if (total == capacidade) {
            capacidade = (capacidade == 0) ? 10 : capacidade * 2;
            lista = realloc(lista, capacidade * sizeof(Produtos));
            if (!lista) {
                printf("Erro de memória.\n");
                fclose(arquivo);
                return 0;
            }
        }

        lista[total] = produto;

        // verifica se é o produto que deve ser atualizado
        if (strcmp(produto.nome, nomeProduto) == 0) {
            encontrado = 1;
            indiceEncontrado = (int)total;
        }

        total++;
    }
    fclose(arquivo);

    if (!encontrado) {
        printf("Produto '%s' não encontrado no estoque.\n", nomeProduto);
        free(lista);
        return 0;
    }

    // Atualiza o produto encontrado
    Produtos *prod = &lista[indiceEncontrado];

    if (modo == 1) { // venda (subtrai)
        if (prod->quantidade < quantidadeAlterar) {
            printf("Estoque insuficiente de '%s'. Quantidade disponível: %d\n",
                   prod->nome, prod->quantidade);
            free(lista);
            return -1;
        }
        prod->quantidade -= quantidadeAlterar;
        printf("Produto '%s' atualizado: nova quantidade %d\n",
               prod->nome, prod->quantidade);
    } 
    else if (modo == 2) { // reposição (soma)
        prod->quantidade += quantidadeAlterar;
        printf("Produto '%s' reabastecido: nova quantidade %d\n",
               prod->nome, prod->quantidade);
    } 
    else {
        printf("Modo inválido.\n");
        free(lista);
        return 0;
    }

 
    arquivo = abrirArquivoEstoque(3);
    if (arquivo == NULL) {
        printf("Erro ao reabrir estoque.txt para escrita.\n");
        free(lista);
        return 0;
    }

    for (size_t i = 0; i < total; i++) {
        fprintf(arquivo, "%d;%c;%s;%.2f;%d\n",
                lista[i].codigo,
                lista[i].tipo,
                lista[i].nome,
                lista[i].preco,
                lista[i].quantidade);
    }

    fclose(arquivo);
    free(lista);
    return 1;
}

//MENUS---------------------------------------------------------------------------------------------------
void menuConsultarProdutos() {
    int opcao;
    do {
        printf("\n--- MENU CONSULTA PRODUTOS ---\n");
        printf("1. Listar todos\n2. Listar bebidas\n3. Listar comidas\n4. Consultar por código\n0. Voltar\nEscolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: listarTodos(); break;
            case 2: listarBebidas(); break;
            case 3: listarComidas(); break;
            case 4: consultarProdutoPorCodigo(); break;
            case 0: break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}

void menuCadastroProduto() {
    int opcao;
    do {
        printf("\n--- MENU CADASTRO PRODUTO ---\n");
        printf("1. Cadastrar\n2. Alterar\n3. Excluir\n0. Voltar\nEscolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: cadastrarProduto(); break;
            case 2: alterarProduto(); break;
            case 3: excluirProduto(); break;
            case 0: break;
            default: printf("Opção inválida!\n");
        }
    } while (opcao != 0);
}