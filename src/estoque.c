#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "estoque.h"

int atualizarEstoque(char nomeProduto[], int quantidadeAlterar, int modo) {
    // modo = 1 -> venda (subtrai)
    // modo = 2 -> reposição (soma)

    FILE *arquivo = fopen("estoque.txt", "r");
    if (arquivo == NULL) {
        printf("estoque.txt não encontrado.\n");
        return 0;
    }

    //Lê tudo em arrays dinâmicos para regravar depois
    int *codigo = NULL;
    char *tipo = NULL;
    char (*nome)[31] = NULL;
    float *preco = NULL;
    int *quantidade = NULL;
    int total = 0;
    int encontrado = 0;
    size_t capacity = 0;

    while (1) {
        int cod;
        char t;
        char n[31];
        float p;
        int q;
        int read = fscanf(arquivo, "%d %c %30s %f %d", &cod, &t, n, &p, &q);
        if (read == EOF || read == 0) break;

        if (total + 1 > (int)capacity) {
            capacity = (capacity == 0) ? 10 : capacity * 2;
            codigo = realloc(codigo, capacity * sizeof(int));
            tipo = realloc(tipo, capacity * sizeof(char));
            nome = realloc(nome, capacity * sizeof(*nome));
            preco = realloc(preco, capacity * sizeof(float));
            quantidade = realloc(quantidade, capacity * sizeof(int));
            if (!codigo || !tipo || !nome || !preco || !quantidade) {
                printf("Erro de memoria.\n");
                fclose(arquivo);
                return 0;
            }
        }

        codigo[total] = cod;
        tipo[total] = t;
        strncpy(nome[total], n, 31);
        preco[total] = p;
        quantidade[total] = q;

        // comparar nomes
        if (strcmp(nome[total], nomeProduto) == 0) {
            encontrado = 1;
            if (modo == 1) { // venda
                if (quantidade[total] >= quantidadeAlterar) {
                    quantidade[total] -= quantidadeAlterar;
                    printf("Produto '%s' atualizado: nova quantidade %d\n", nomeProduto, quantidade[total]);
                } else {
                    printf("Estoque insuficiente de '%s'. Quantidade disponível: %d\n", nomeProduto, quantidade[total]);
                    fclose(arquivo);
                    free(codigo); free(tipo); free(nome); free(preco); free(quantidade);
                    return -1; // quantidade insuficiente
                }
            } else if (modo == 2) { // reposição
                quantidade[total] += quantidadeAlterar;
                printf("Produto '%s' reabastecido: nova quantidade %d\n", nomeProduto, quantidade[total]);
            }
        }

        total++;
    }
    fclose(arquivo);

    if (!encontrado) {
        printf("Produto '%s' nao encontrado no estoque!\n", nomeProduto);
        free(codigo); free(tipo); free(nome); free(preco); free(quantidade);
        return 0; 
    }

    // Regrava todo o arquivo atualizado
    arquivo = fopen("estoque.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir estoque.txt para escrita.\n");
        free(codigo); free(tipo); free(nome); free(preco); free(quantidade);
        return 0;
    }

    for (int i = 0; i < total; i++) {
        fprintf(arquivo, "%d %c %s %.2f %d\n",
                codigo[i], tipo[i], nome[i], preco[i], quantidade[i]);
    }
    fclose(arquivo);

    free(codigo); free(tipo); free(nome); free(preco); free(quantidade);

    return 1;
}

//CADASTRO DE PRODUTOS----------------------------------------------------------------------------------------------
void cadastrarProduto(){
    char nome[31];
    float preco;
    int quantidade, codigo;
    char tipo;

    printf("\n-------Cadastro de Produtos-------\n");
    printf("Digite o código do produto (até 6 números): ");
    scanf("%d", &codigo);
    getchar();
    printf("Digite o tipo de produto ('C' para comidas, 'B' para bebidas): ");
    scanf("%c", &tipo);
    getchar();
    printf("Digite o nome do produto: ");
    scanf(" %30[^\n]", nome);
    printf("Digite o preço do produto: ");
    scanf("%f", &preco);
    printf("Digite a quantidade do produto: ");
    scanf("%d", &quantidade);

    // Verifica se o produto já existe no arquivo
    FILE *arquivo = fopen("estoque.txt", "r");
    if (arquivo != NULL) {
        int codigoLido, qtdLida;
        char tipoLido, nomeLido[31];
        float precoLido;
        while (fscanf(arquivo, "%d %c %30s %f %d", &codigoLido, &tipoLido, nomeLido, &precoLido, &qtdLida) != EOF) {
            if (strcmp(nomeLido, nome) == 0) {
                fclose(arquivo);
                printf("\nProduto '%s' ja existe. Atualizando quantidade...\n", nome);
                atualizarEstoque(nome, quantidade, 2); // modo 2 = reposição
                return;
            }
        }
        fclose(arquivo);
    }

    // Se não existe, adiciona novo produto
    arquivo = fopen("estoque.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo estoque.txt\n");
        return;
    }

    fprintf(arquivo, "%d %c %s %.2f %d\n", codigo, tipo, nome, preco, quantidade);
    fclose(arquivo);
    printf("Produto '%s' cadastrado com sucesso!\n", nome);
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
                scanf(" %30[^\n]", nome); 
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
    while(fscanf(arquivo, "%d %c %49s %f %d", &codigo, &tipo, nome, &preco, &quantidade)!=EOF){
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
    int codigoExcluir;
    int encontrado=0;
    
    int codigo;
    char tipo;
    char nome[50];
    float preco;
    int quantidade;
    
    FILE *arquivo=fopen("estoque.txt", "r");
    FILE *temp=fopen("temp.txt", "w");

    if ((arquivo==NULL)||(temp==NULL)) {
        printf("Erro: nao foi possivel abrir os arquivos.\n");
        return;
    }
    
    printf("\n-----------------Excluir Produto-----------------\n");
    printf("Digite o código do produto que deseja excluir: ");
    scanf("%d", &codigoExcluir);
    
    while(fscanf(arquivo, "%d %c %49s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
        if(codigo==codigoExcluir){
            encontrado = 1;
            printf("Produto '%s', código: %d, foi excluído.\n", nome, codigo);
        } else{
            fprintf(temp, "%d %c %s %.2f %d\n", codigo, tipo, nome, preco, quantidade);
        }
    }

    fclose(arquivo);
    fclose(temp);

    if(encontrado){
        remove("estoque.txt");
        rename("temp.txt", "estoque.txt");
    } else{
        remove("temp.txt");
        printf("\nProduto com código %d não encontrado.\n", codigoExcluir);
    }
}

void menuCadastroProduto(){
    int resposta=-1;
    void (*gerenciar[])()={menuInicial, cadastrarProduto, alterarProduto, excluirProduto};
    //Repete a pergunta e cadastra produtos no estoque e 
    //volta para o menu inicial quando o usuário digita 0
    while (resposta!=0) {
        printf("\n\n-------Menu de Cadastro-------\n");
        printf("Digite 1 para cadastrar um produto\n");
        printf("Digite 2 para alterar o produto\n");
        printf("Digite 3 para excluir um produto no estoque\n");
        printf("Digite 0 para voltar ao menu inicial\n");
        printf("Resposta: ");
        scanf("%d", &resposta);
        if(resposta>=0 && resposta<=3){
            gerenciar[resposta]();
        } else{
            printf("Resposta inválida\n");
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

    fclose(arquivo);
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
        while (fscanf(arquivo, "%d %c %30s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
            if(tipo == 'B') {
                printf("%d %s\n", codigo, nome);
            }
        }
    }

    fclose(arquivo);
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
        while (fscanf(arquivo, "%d %c %30s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
            if(tipo == 'C') {
            printf("%d %s\n", codigo, nome);
            }
        }
    }

    fclose(arquivo);
}

void consultarProdutoPorCodigo(){
    FILE* arquivo;
    int codigo, quantidade, procuraCodigo, encontrado = 0;
    char tipo;
    float preco;
    char nome[31];
    printf("\nDigite o código do produto: ");
    scanf("%d",&procuraCodigo);
    arquivo = fopen ("estoque.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo!"); //mensagem de erro caso o arquivo não exista
    } else {
        while (fscanf(arquivo, "%d %c %30s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
            if(procuraCodigo == codigo) {
                printf("%d %c %s R$%.2f %d(unidades)\n", codigo, tipo, nome, preco, quantidade);
                encontrado = 1;
                break;
            }
            if (!encontrado){
                printf("Código não encontrado!\n");
            }
        }
    }

    fclose(arquivo);

}

//função para usuário escolher qual lista quer consultar
void menuConsultarProdutos() {
    int resposta;
    void (*gerenciar[])()={menuInicial, listarTodos, listarBebidas, listarComidas, consultarProdutoPorCodigo};
    printf("------------MENU DE CONSULTA------------\n");
    printf("Qual lista você deseja consultar?\n(1) Lista de todos os produtos\n(2) Lista de bebidas\n(3) Lista de comidas\n(4) Consultar por código\n(0) Voltar ao Menu Inicial");
    printf("Consultar: ");
    scanf("%d",&resposta);
    printf("\n----------------------------------------\n");
    if(resposta<=4 && resposta>=0){
        gerenciar[resposta]();
    } else{
        printf("Resposta inválida\n");
    }
}

int obterPrecoQuantidade(const char nomeProduto[], float *precoUnitario, int *quantidadeDisponivel) {
    FILE *arquivo = fopen("estoque.txt", "r");
    if (!arquivo) return 0;
    int codigo;
    char tipo;
    char nome[31];
    float preco;
    int quantidade;
    while (fscanf(arquivo, "%d %c %30s %f %d", &codigo, &tipo, nome, &preco, &quantidade) != EOF) {
        if (strcmp(nome, nomeProduto) == 0) {
            *precoUnitario = preco;
            *quantidadeDisponivel = quantidade;
            fclose(arquivo);
            return 1;
        }
    }
    fclose(arquivo);
    return 0;
}
