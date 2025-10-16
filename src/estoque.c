#include <stdio.h>
#include "estoque.h"

//CADASTRO DE PRODUTOS----------------------------------------------------------------------------------------------
void cadastrarProduto(){
    char nome[20];
    float preco;
    int quantidade, codigo;
    char tipo;
    FILE *arquivo = fopen("estoque.txt", "a+");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo estoque.txt\n");
        return;
    }

    printf("\n-------Cadastro de Produtos-------\n");
    printf("Digite o código do produto, no máximo 6 números inteiros: ");
    scanf("%d", &codigo);
    getchar(); // Limpa o buffer do teclado (o "enter" anterior)
    printf("Digite o tipo de produto, 'C' para comidas e 'B' para bebidas: ");
    scanf("%c", &tipo);
    getchar(); // Limpa o buffer do teclado
    printf("Digite o nome do produto: ");
    scanf("%[^\n]", nome);
    printf("Digite o preço do produto: ");
    scanf("%f", &preco);
    printf("Digite a quantidade do produto no estoque: ");
    scanf("%d", &quantidade);

    fprintf(arquivo, "%d %c %s %.2f %d\n", codigo, tipo, nome, preco, quantidade);
    fclose(arquivo);
}

void menuCadastroProduto(){
    int resposta = 1;
    
    // Repete a pergunta e cadastra produtos no estoque e 
    // volta para o menu inicial quando o usuário digita 0
    while (resposta != 0) {
        printf("\n\n-------Menu de Cadastro-------\n");
        printf("Digite 1 para cadastrar um produto\n");
        printf("Digite 0 para sair\n");
        printf("Resposta: ");
        scanf("%d", &resposta);
        
        if(resposta == 1){
            cadastrarProduto();
        } else if(resposta == 0){
            printf("Saindo...\n");
            break;
        } else{
            printf("Resposta inválida, digite novamente.\n");
        }
    }

}

//CONSULTA DE PRODUTOS (LISTA)---------------------------------------------------------------------------------------------------
//listar todos os itens do estoque
void listarTodos()
{
    FILE* arquivo;
    int codigo, quantidade;
    char tipo;
    float preco;
    char nome[31];
    arquivo = fopen ("estoque.txt", "r");
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo!"); //mensagem de erro caso o arquivo não exista
    }else {
        while (fscanf(arquivo, "%d %c %s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
            printf("%d %s\n", codigo, nome);
        }
    }
}

//listar apenas as bebidas do estoque
void listarBebidas(){
    FILE* arquivo;
    int codigo, quantidade;
    char tipo;
    float preco;
    char nome[31];
    arquivo = fopen ("estoque.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo!"); //mensagem de erro caso o arquivo não exista
    } else {
        while (fscanf(arquivo, "%d %c %s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
            if(tipo == 'B') {
            printf("%d %s\n", codigo, nome);
            }
        }
    }
}

//listar todas as comidas do estoque
void listarComidas(){
    FILE* arquivo;
    int codigo, quantidade;
    char tipo;
    float preco;
    char nome[31];
    arquivo = fopen ("estoque.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo!"); //mensagem de erro caso o arquivo não exista
    } else {
        while (fscanf(arquivo, "%d %c %s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
            if(tipo == 'C') {
            printf("%d %s\n", codigo, nome);
            }
        }
    }
}

//função para usuário escolher qual lista quer consultar
void menuListarProdutos() {
    int res;
    printf("------------MENU DE CONSULTA------------\n");
    printf("Qual lista você deseja consultar?\n(1) Lista de todos os produtos\n(2) Lista de bebidas\n(3) Lista de comidas\n");
    printf("Consultar: ");
    scanf("%d",&res);
    printf("\n----------------------------------------\n");
    switch(res){
        case 1:
            listarTodos();
            break;
        case 2:
            listarBebidas();
            break;
        case 3:
            listarComidas();
            break;
    }
}
