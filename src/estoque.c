#include <stdio.h>
#include "utils.h"
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

void menuAlterarProduto(char *nome, float *preco, char *tipo, int *quantidade) {
    int opcao=0;
    do{
        printf("\n\n--- Produto Encontrado ---\n");
        printf("Tipo: %c\n", *tipo);
        printf("Nome: %s\n", nome);
        printf("Preço: R$%.2f\n", *preco);
        printf("Quantidade: %d\n", *quantidade);
        printf("--------------------------\n");
        printf("O que deseja alterar?\n");
        printf("1. Nome\n2. Preço\n3. Tipo de produto\n4. Quantidade no estoque\n0. Salvar alterações e Voltar ao menu\n");
        printf("Opção: ");
        scanf("%d", &opcao);

        switch(opcao){
            case 1:
                printf("Digite o novo nome: ");
                scanf(" %[^\n]", nome); 
                break;
            case 2:
                printf("Digite o novo preço: ");
                scanf(" %f", preco); 
                break;
            case 3:
                printf("Digite o novo tipo de produto: ");
                scanf(" %c", tipo);
                break;
            case 4:
                printf("Digite a nova quantidade do produto: ");
                scanf(" %d", quantidade);
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
    
    int codigo;
    char tipo;
    char nome[50];
    float preco;
    int quantidade;

    FILE *arquivo=fopen("estoque.txt", "r");
    FILE *temp=fopen("temp.txt", "w");
    //Verifica se os arquivos podem ser abertos.
    if((arquivo==NULL)||(temp==NULL)){
        printf("Erro: não é possível abrir o arquivo\n");
        return;
    }
    
    printf("\n\n-------Alterar Produto-------\n");
    printf("Digite o código do produto que deseja alterar: ");
    scanf("%d", &codigoAlterar);
    
    //Percorre o arquivo de texto buscando pelo produto que possui o código
    while(fscanf(arquivo, "%d %c %s %f %d", &codigo, &tipo, nome, &preco, &quantidade)!=EOF){
        if(codigo==codigoAlterar){
            encontrado=1;
            //Chama a função menuAlterarProduto para alterar o arquivo
            menuAlterarProduto(nome, &preco, &tipo, &quantidade);
        }
        // Escreve a linha no arquivo temporário.
        fprintf(temp, "%d %c %s %.2f %d\n", codigo, tipo, nome, preco, quantidade);
    }

    fclose(arquivo);
    fclose(temp);
    
    if(encontrado){
        //Se o produto foi encontrado, apaga 'estoque.txt', e renomeia 'temp.txt' para ser o novo 'estoque.txt'
        remove("estoque.txt");
        rename("temp.txt", "estoque.txt");
        printf("\nProduto alterado com sucesso!\n");
    }else{
        //Se não foi encontrado o produto, apenas apaga o arquivo temporário que foi criado.
        remove("temp.txt");
        printf("\nProduto com código %d não encontrado.\n", codigoAlterar);
    }
}

void excluirProduto(){

}

void menuCadastroProduto(){
    int resposta=3;
    void (*gerenciar[])()={menuInicial, cadastrarProduto, alterarProduto, excluirProduto};
    //Repete a pergunta e cadastra produtos no estoque e 
    //volta para o menu inicial quando o usuário digita 0
    while (resposta!=0) {
        printf("\n\n-------Menu de Cadastro-------\n");
        printf("Digite 1 para cadastrar um produto\n");
        printf("Digite 2 para alterar o produto\n");//Alteração do código
        printf("Digite 3 para excluir um peoduto no estoque\n");//Alteração do código
        printf("Digite 0 para voltar ao menu inicial\n");//Ateração no código
        printf("Resposta: ");
        scanf("%d", &resposta);
        if(resposta<=3){//Alteração do código
            gerenciar[resposta]();//Alteração do código
        } else{//Alteração do código
            printf("Resposta inválida\n");//Alteração do código
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

void consultarProdutoPorCodigo(){
    FILE* arquivo;
    int codigo, quantidade, procuraCodigo;
    char tipo;
    float preco;
    char nome[31];
    printf("\nDigite o código do produto: ");
    scanf("%d",&procuraCodigo);
    arquivo = fopen ("estoque.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo!"); //mensagem de erro caso o arquivo não exista
    } else {
        while (fscanf(arquivo, "%d %c %s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
            if(procuraCodigo == codigo) {
            printf("%d %c %s R$%.2f %d(unidades)\n", codigo, tipo, nome, preco, quantidade);
            break;
            }else{
                perror("Código não encontrado!");
            }
        }
    }
}

//função para usuário escolher qual lista quer consultar
void menuConsultarProdutos() {
    int opcao;
    printf("------------MENU DE CONSULTA------------\n");
    printf("Qual lista você deseja consultar?\n(1) Lista de todos os produtos\n(2) Lista de bebidas\n(3) Lista de comidas\n(4) Consultar por código\n(0) Voltar ao Menu Inicial");
    printf("Consultar: ");
    scanf("%d",&opcao);
    printf("\n----------------------------------------\n");
    switch(opcao){
        case 1:
            listarTodos();
            break;
        case 2:
            listarBebidas();
            break;
        case 3:
            listarComidas();
            break;
        case 4:
            consultarProdutoPorCodigo();
            break;
        case 0:
            menuInicial();
            break;
    }
}


