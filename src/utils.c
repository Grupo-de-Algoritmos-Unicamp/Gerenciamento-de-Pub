#include <stdio.h>
#include "utils.h"
#include "estoque.h"
#include "pedido.h"

void menuInicial(){
    int resposta;
    void (*gerenciar[])()={sair, menuCadastroProduto, menuConsultarProdutos, /*cadastro de clientes*/, registrarPedido(); /*gerar relatório*/); 
    while (resposta!=0){
    printf("--------------MENU INICIAL--------------");
    printf("\n O que deseja fazer?");
    printf("\n(1) Cadastrar produto\n(2) Consultar produto\n(3) Cadastrar cliente\n(4) Registrar pedido\n(5) Gerar relatório\n(0) Sair");
    printf("\nSelecionar: ");
    scanf("%d",&resposta);
    printf("\n----------------------------------------");
    switch(resposta){
        if(resposta<=3 && resposta>=0){
            gerenciar[resposta]();
        }else{
            printf("Resposta inválida\n");
            }
        }
    }
}

void sair(){
    printf("Saindo...");
}
