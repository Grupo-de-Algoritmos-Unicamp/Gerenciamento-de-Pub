#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "estoque.h"
#include "pedido.h"

struct Cliente {
    char nome[50];
    char cpf[20];
    char telefone[15];
    char email[100];
};

void cadastrarCliente() {
    
    struct Cliente c;
    
    printf("\n--------Cadastro de Cliente--------\n");
    
    printf("Digite o nome completo:");
    scanf(" %[^\n]", c.nome);
    
    printf("Digite o CPF:");
    scanf("%s", c.cpf);
    
    printf("Digite o telefone:");
    scanf(" %s", c.telefone);
    
    printf("Digite o email:");
    scanf("%s", c.email);
    
    FILE *arqCliente = fopen("clientes.txt", "a+");
    if (arqCliente == NULL){
        printf("Erro ao abrir o arquivo!");
        return;
    }
    
    fprintf(arqCliente, "%s;%s;%s;%s\n", c.nome, c.cpf, c.telefone, c.email);
    
    fclose(arqCliente);
    
    printf("\nCliente cadastrado com sucesso!\n");
}

float calcularValorGasto(char* buscaCpf) {
    
    FILE *arqPedido;
    char cpfPedido[20];
    float valorGasto; 
    float valorTotalGasto = 0.0; 

    arqPedido = fopen("pedidos.txt", "r");
    if (arqPedido == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 0.0;
    }

    while(fscanf(arqPedido, "%*s %s %*s %*d %f", cpfPedido, &valorGasto) != EOF) {
        if(strcmp(buscaCpf, cpfPedido) == 0) {
            valorTotalGasto += valorGasto;
        }
    }

    fclose(arqPedido);
    
    return valorTotalGasto;
}

void consultarCliente() {
    
    struct Cliente c; 
    char senha[7];
    const char senhaCorreta[] = "543210";
    char buscaCpf[20];
    int encontrado = 0;
    
    printf("\n---- Acessar Dados ----\n");
    printf("Digite a senha:");
    scanf("%s", senha);
    
    if (strcmp(senha,senhaCorreta) != 0) {
        printf("Senha incorreta! Acesso negado.\n");
        return;
    }
        
    printf("Digite o CPF do cliente:");
    scanf("%s", buscaCpf);

    FILE *arqCliente = fopen("clientes.txt", "r");
    if (arqCliente == NULL){
        printf("Erro ao abrir o arquivo!\n");
        return;
    }
    
    while(fscanf(arqCliente, "%[^;];%[^;];%[^;];%[^\n]\n", c.nome, c.cpf, c.telefone, c.email) != EOF) {
        if (strcmp(buscaCpf, c.cpf) == 0) {
            encontrado = 1;
            printf("\n---- Dados do Cliente ----\n");
            printf(" Nome:%s\n CPF:%s\n Telefone:%s\n Email:%s", c.nome, c.cpf, c.telefone, c.email);
            
            float totalGasto = calcularValorGasto(buscaCpf);
            
            printf("\n Valor total gasto: R$ %.2f\n", totalGasto);
            break;  
        }
    } 
    
    fclose(arqCliente);
    
    if(!encontrado) {
        printf("\nCliente com CPF %s não encontrado.\n", buscaCpf);
        return;
    }
}

void sair(){
    printf("Saindo...");
}

void menuInicial(){
    int resposta;
    void (*gerenciar[])() = {sair, menuCadastroProduto, menuConsultarProdutos, registrarPedido, cadastrarCliente, consultarCliente};
    
    while (resposta!=0){
        printf("--------------MENU INICIAL--------------");
        printf("\n O que deseja fazer?");
        printf("\n(1) Cadastrar produto\n(2) Consultar produto\n(3) Registrar pedidos\n(4) Cadastrar cliente\n(5) Consultar cliente\n(0) Sair");
        printf("\nSelecionar: ");
        scanf("%d",&resposta);
        printf("\n----------------------------------------");

        if (resposta >= 0 && resposta <= 5) {
            gerenciar[resposta](); 
        } else {
            printf("Opção inválida!\n");
        }  
    }
}







