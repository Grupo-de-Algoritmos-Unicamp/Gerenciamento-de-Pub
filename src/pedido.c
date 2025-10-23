#include <stdio.h>
#include "pedido.h"

void registrarPedido(){
    char cpf[15];
    int numeroPedido, i;

    printf("Digite o CPF do cliente <utilizando pontos e hifen>: ");
    scanf("%s", cpf);

    printf("Digite o numero do pedido: ");
    scanf("%i", &numeroPedido);

    printf("Digite quantos produtos o cliente comprou: ");
    scanf("%d", &i);

    char nome[i][31];
    float preco[i], valorTotal = 0;

    for(int j = 0; j<i; j++){
        printf("\n");
        printf("Digite o nome do %d° produto: ", j+1);
        scanf(" %[^\n]", nome[j]);

        printf("Digite o preco do %d° produto: ", j+1);
        scanf("%f", &preco[j]);

        valorTotal += preco[j];

        printf("\n");
    }

    printf("Total: %.2f\n\n", valorTotal);

    int tipoPagamento;
    float pagoCliente = 0, troco = 0;
    char trocoSN;

    do{
        printf("Qual sera a forma de pagamento: \n");
        printf("(1) PIX \n");
        printf("(2) Cartao de Credito/Debito \n");
        printf("(3) Dinheiro \n");
        scanf("%d", &tipoPagamento);

        switch(tipoPagamento){
            case 1:
                printf("Gere o codigo na maquininha");
                break;
            case 2:
                printf("Selecione credito ou debito na maquininha, e peca para o cliente inserir ou aproximar o cartão na maquininha");
                break;
            case 3:
                printf("\nAbra a caixa registradora e receba o dinheiro do cliente! \n");
                printf("Eh preciso devolver troco ao cliente? <s/n>");
                scanf(" %c", &trocoSN);

                if(trocoSN == 's'){
                    printf("\nDigite a quantia que o cliente forneceu em nota: ");
                    scanf("%f", &pagoCliente);

                    troco = pagoCliente - valorTotal;

                    printf("\nDevolva o troco de R$%.2f ao cliente", troco);
                }

                break;
            default:
                printf("Opcao de pagamento invalida! TENTE NOVAMENTE!");
                continue;
        }

    break;

    }while(1);

    //Criando o arquivo txt do pedido
    FILE *pedido = fopen("pedido.txt", "w");

    fprintf(pedido, "----------PEDIDO----------\n\n");
    fprintf(pedido, "CPF do Cliente: %s\n", cpf);
    fprintf(pedido, "Numero do pedido: %d\n\n", numeroPedido);
    fprintf(pedido, "Lista de Produtos: \n");

    for(int j = 0; j<i; j++){
        fprintf(pedido, "Produto: %d°\n", j+1);
        fprintf(pedido, "Nome: %s\n", nome[j]);
        fprintf(pedido, "Preco: R$ %.2f\n\n", preco[j]);
    }

    fprintf(pedido, "\nTotal: R$ %.2f\n\n", valorTotal);

    fprintf(pedido, "Forma de pagamento do cliente: ");

    if(tipoPagamento == 1){
        fprintf(pedido, "PIX");
    } else if(tipoPagamento == 2){
        fprintf(pedido, "Cartao");
    } else{
        fprintf(pedido, "Dinheiro\n");

        if(troco>0){
            fprintf(pedido, "Cliente forneceu R$ %.2f\n", pagoCliente);
            fprintf(pedido, "Troco: R$ %.2f", troco);
        }
    }

    fclose(pedido);

    printf("\n\nPedido registrado com sucesso!");
}

int main(){
    registrarPedido();

    return 0;
}