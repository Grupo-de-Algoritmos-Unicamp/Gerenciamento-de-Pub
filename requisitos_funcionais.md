Claro, segue a conversão para Markdown, com os emojis removidos:

## Requisitos Funcionais

### RF001 - Cadastros de Produtos
O sistema deve permitir que o administrador cadastre novos produtos, com nome, preço, tipo (comida ou bebida), quantidade no estoque e código de identificação para cada produto.
* **Entrada:** Código, Nome da bebida ou comida, preço e quantidade no estoque.
* **Saída:** Confirmação do cadastro da bebida ou comida e geração do arquivo estoque.txt.

### RF002 - Registro de Pedidos
O sistema deve permitir o registro dos pedidos que forem realizados, com o CPF do cliente, número do pedido, lista dos produtos, com seus respectivos preços, tipo de pagamento, valor total pago, troco, se houver.
* **Entrada:** CPF do cliente, produtos comprados e forma de pagamento.
* **Saída:** Confirmação do pedido realizado e geração dos arquivos pedidos.txt e itens_vendidos.txt.

### RF003 - Atualizar Estoque
O sistema deve atualizar o estoque conforme produtos são vendidos ou quando o estoque for reposto.
* **Entrada:** Nome ou código do produto, quantidade de itens no pedido ou quantidade de produtos entrantes.
* **Saída:** Nova quantidade de itens no estoque e no arquivo estoque.txt.

### RF004 - Gerar relatórios de vendas
O sistema deve gerar relatórios de vendas resumidos, filtrando por data se desejado, exibindo produtos vendidos, quantidades, valor total e ticket médio por pedido.
* **Entrada:** Data do período que será utilizado no relatório (opcional), pedidos.txt e itens_vendidos.txt.
* **Saída:** Impressão na tela da lista dos produtos vendidos, quantidade total de vendas, valor total faturado e ticket médio por pedido. Além disso, geração do arquivo relatorio_resumido.txt caso desejado.

### RF005 - Cadastro de Clientes
O sistema deve permitir o cadastro de clientes com nome, CPF, email e telefone.
* **Entrada:** Nome, CPF, email e telefone do cliente.
* **Saída:** Confirmação do cadastro do cliente e geração do arquivo clientes.txt.

### RF006 - Consultar Cadastro de clientes
O sistema deve permitir o cadastro de clientes com nome, CPF, email e telefone.
* **Entrada:** CPF do cliente e senha de acesso.
* **Saída:** Dados do cliente e total de valor gasto.

### RF007 - Consultar Lista de Produtos
O sistema deve permitir que o usuário consulte o código e o nome de todos os produtos oferecidos.
* **Entrada:** Selecionar a opção consultar lista de produtos.
* **Saída:** Códigos e nomes dos produtos.

### RF008 - Consultar Lista de Bebidas
O sistema deve permitir que o usuário consulte o código e o nome de todas as bebidas oferecidas.
* **Entrada:** Selecionar a opção consultar lista de bebidas.
* **Saída:** Códigos e nomes das bebidas.

### RF009 - Consultar Lista de Comidas
O sistema deve permitir que o usuário consulte o código e o nome de todas as comidas oferecidas.
* **Entrada:** Selecionar a opção consultar lista de comidas.
* **Saída:** Códigos e nomes das comidas.

### RF010 - Consultar Produto por Código
O sistema deve permitir que o usuário pesquise um produto pelo seu código consulte todas as informações do produto.
* **Entrada:** Código do produto.
* **Saída:** Código, nome, preço, quantidade no estoque.

### RF011 - Alterar o produto
O sistema deve permitir alterar o nome, tipo, preço ou quantidade de um produto já cadastrado.
* **Entrada:** Código do produto e novos valores.
* **Saída:** alteração e confirmação de atualização dos produtos.

### RF012 - Excluir e ativar produto
O sistema deve permitir marcar um produto como inativo (0 = inativo e 1 = ativo) e, posteriormente, reativá-lo, caso necessário.
* **Entrada:** Código do produto.
* **Saída:** exclusão ou ativação do produto, e mensagem de confirmação da ação.
