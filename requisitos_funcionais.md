RF001 - Cadastros de Produtos:RF001 - Cadastros de Produtos: O sistema deve permitir que o administrador cadastre novos produtos, com nome, preço, tipo (comida ou bebida), quantidade no estoque e código de identificação para cada produto.
Entrada: Nome da bebida ou comida, descrição, preço, avisos de ingredientes alergênicos.
Saída: Confirmação do cadastro da bebida ou comida.


RF002 - Registro de Pedidos: O sistema deve permitir o registro dos pedidos que forem realizados, com o CPF do cliente, número do pedido, lista dos produtos, com seus respectivos preços, tipo de pagamento, valor total pago, troco, se houver.
Entrada: Número do pedido, descrição, tipo de pagamento, valor total pago, troco recebido.
Saída: Confirmação do pedido realizado.


RF003 - Atualizar Estoque: O sistema deve atualizar o estoque conforme produtos são vendidos ou quando o estoque for resposto.
Entrada: Quantidade de itens no pedido ou quantidade de produtos entrantes.
Saída: Nova quantidade de itens no estoque.

RF004 - Gerar relatórios de vendas: O sistema deve permitir que o usuário gere relatórios de vendas.
Entrada: Data em que foi gerado,volume de vendas, produtos mais vendidos, métricas de desempenho e financeiras.
Saída: Lista dos produtos vendidos, quantidade total de vendas, valor total faturado e produtos mais vendidos.

RF005 - Alerta de estoque baixo: O sistema deve emitir alertas quando algum produto estiver abaixo do limite mínimo de estoque configurado.
Entrada: Dados de estoque, limite mínimo configurado, atualização do estoque.
Saída: Notificação de estoque.

RF006 - Cadastro de clientes: O sistema deve permitir o cadastro de clientes com nome, CPF, email e telefone.
Entrada: Nome, CPF, email e telefone do cliente.
Saída: Confirmação do cadastro do cliente.


RF007 - Consultar Cadastro de clientes: O sistema deve permitir a consulta do cadastro do cliente utilizando o CPF.
Entrada: CPF do cliente.
Saída: Dados do cadastro do cliente: nome, CPF, email, telefone, número de pedidos já realizados.


RF008 - Pedir senha antes de acessar dados do cliente: O sistema deve pedir que o usuário digite uma senha antes de acessar os dados dos clientes.
Entrada: Senha digitada pelo usuário.
Saída: Senha correta/incorreta.


RF009 - Consultar Lista de Produtos: O sistema deve permitir que o usuário consulte o código e o nome de todos os produtos oferecidos.
Entrada: Selecionar a opção consultar lista de produtos.
Saída: Códigos e nomes dos produtos.


RF010 - Consultar Lista de Bebidas: O sistema deve permitir que o usuário consulte o código e o nome de todas as bebidas oferecidas.
Entrada: Selecionar a opção consultar lista de bebidas.
Saída: Códigos e nomes das bebidas.

RF011 - Consultar Lista de Bebidas: O sistema deve permitir que o usuário consulte o código e o nome de todas as bebidas oferecidas.
Entrada: Selecionar a opção consultar lista de bebidas.
Saída: Códigos e nomes das bebidas.



RF012 - Consultar Produto por Código: O sistema deve permitir que o usuário pesquise um produto pelo seu código consulte todas as informações do produto.
Entrada: Código do produto.
Saída: Código, nome, preço, quantidade no estoque.


RF013 - Alterar o produto: O sistema deve permitir alterar o nome ou o preço de um produto já registrado.
Entrada: Código do produto.
Saída: nome ou preço alterado do produto.


RF014 - Excluir o produto: O sistema deve permitir excluir um produto já registrado.
Entrada: Código do produto.
Saída: O produto não está mais no registro.

