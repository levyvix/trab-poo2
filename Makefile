# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -Wextra -std=c++20 -Iinclude

# Nome do executável
TARGET = prog

# Diretório de origem dos arquivos fonte
SRC_DIR = src

# Diretório de saída dos arquivos objeto
OBJ_DIR = obj

# Lista de arquivos fonte (.cpp)
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Lista de arquivos objeto (.o)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Regra padrão (executada ao digitar apenas 'make')
all: $(TARGET)

# Regra para gerar o executável
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regra para gerar os arquivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR) # Cria o diretório obj se não existir
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regra para gerar apenas os arquivos objeto
objs: $(OBJS)

# Regra para forçar a recompilação dos arquivos objeto
force: clean objs

# Regra para limpar arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Informa ao make que 'all', 'objs', 'force' e 'clean' não são arquivos
.PHONY: all objs force clean