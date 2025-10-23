#include <stdio.h>
#include "utils.h"

void menuInicial(){
    int opcao;
    printf("--------------MENU INICIAL--------------");
    printf("\n O que deseja fazer?");
    printf("\n(1) Cadastrar produto\n(2) Consultar produto\n(3) Cadastrar cliente\n(4) Registrar pedido\n(5) Gerar relatório\n(0) Sair");
    printf("\nSelecionar: ");
    scanf("%d",&opcao);
    printf("\n----------------------------------------");
    switch(opcao){
        case 1:
            menuCadastroProduto();
            break;
        case 2:
            menuConsultarProdutos();
            break;
        case 3:
            //inserir função para cadastro de cliente
            break;
        case 4:
            //inserir função para cadastrar pedido
            break;
        case 5:
            //inserir função para gerar relatório
            break;
        case 0:
            printf("Saindo...");
            break;
    }
}