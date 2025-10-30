# Define o compilador
CC = gcc

# Define as flags de compilação
# -Wextra -> Mostra ainda mais warnings do que o -Wall
CFLAGS = -Wall -Wextra -g

# O nome do executável final
TARGET = gerenciaPub

# Define o diretório dos arquivos fonte
SRC_DIR = src

# Lista de TODOS os arquivos fonte que fazem parte do projeto
SRCS = 	$(SRC_DIR)/main.c \
	$(SRC_DIR)/pedido.c \
	$(SRC_DIR)/estoque.c \
	$(SRC_DIR)/utils.c

# Gera a lista de arquivos objeto.
# Eles serão criados no diretório raiz
OBJS = $(SRCS:$(SRC_DIR)/%.c=%.o)


# --- REGRAS ---

# AVISO: As linhas de receita DEVEM ser indentadas com um caractere TAB, não com espaços.

# Regra padrão
.PHONY: all
all: $(TARGET)

# Regra de "linkagem":
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regra de "compilação":
%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Regra 'clean':
.PHONY: clean
clean:
	del $(TARGET) $(OBJS)