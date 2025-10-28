#include <stdio.h>
#include "utils.h"
#include "estoque.h"
#include "pedido.h"

void sair(){
    printf("Saindo...");
}

void menuInicial(){
    int resposta;
    void (*gerenciar[])() = {sair, menuCadastroProduto, menuConsultarProdutos, registrarPedido};
    
    while (resposta!=0){
        printf("--------------MENU INICIAL--------------");
        printf("\n O que deseja fazer?");
        printf("\n(1) Cadastrar produto\n(2) Consultar produto\n(3) Cadastrar cliente\n(4) Registrar pedido\n(5) Gerar relatório\n(0) Sair");
        printf("\nSelecionar: ");
        scanf("%d",&resposta);
        printf("\n----------------------------------------");

        if (resposta >= 0 && resposta <= 3) {
            gerenciar[resposta](); 
        } else {
            printf("Opção inválida!\n");
        }  
    }
}


