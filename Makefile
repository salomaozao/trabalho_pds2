# =============================================================================
#  PDS II - Projeto Final - RPG por Turnos
#  Makefile de automacao da compilacao, execucao, testes e cobertura.
#
#  Alvos principais:
#    make            compila o jogo em build/rpg
#    make run        compila (se preciso) e executa o jogo
#    make test       compila e executa a suite de testes de unidade (doctest)
#    make coverage   recompila com instrumentacao e gera o relatorio do gcovr
#    make docs       gera a documentacao Doxygen em docs/html
#    make clean      remove binarios, objetos e arquivos de cobertura
#    make deps       baixa o doctest.h e instala o gcovr
# =============================================================================

CXX      := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -pedantic -Iinclude
LDFLAGS  :=

SRC_DIR   := src
INC_DIR   := include
TEST_DIR  := tests
BUILD_DIR := build

# No Windows o executavel recebe a extensao .exe e as bibliotecas do GCC sao
# linkadas estaticamente. Sem isso, uma libstdc++-6.dll antiga presente no PATH
# (a que vem junto com o Git Bash, por exemplo) pode ser carregada no lugar da
# do compilador e derrubar o programa com STATUS_ENTRYPOINT_NOT_FOUND.
ifeq ($(OS),Windows_NT)
    EXE := .exe
    LDFLAGS += -static-libgcc -static-libstdc++
    # O gcovr instalado via "pip install --user" nao entra no PATH do Windows,
    # entao ele e chamado como modulo do Python.
    GCOVR ?= python -m gcovr
else
    EXE :=
    GCOVR ?= gcovr
endif

ALVO      := $(BUILD_DIR)/rpg$(EXE)
ALVO_TEST := $(BUILD_DIR)/testes$(EXE)

# Todos os .cpp de src/ compoem o jogo.
FONTES  := $(wildcard $(SRC_DIR)/*.cpp)
OBJETOS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(FONTES))

# Os testes reaproveitam todos os fontes menos o main.cpp, que tem sua propria
# funcao main e conflitaria com a do doctest.
FONTES_LIB  := $(filter-out $(SRC_DIR)/main.cpp,$(FONTES))
OBJETOS_LIB := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(FONTES_LIB))

FONTES_TEST  := $(wildcard $(TEST_DIR)/*.cpp)
OBJETOS_TEST := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(FONTES_TEST))

.PHONY: all run test coverage docs clean clean-obj deps ajuda

all: $(ALVO)

# ----------------------------------------------------------------------------
# Compilacao do jogo
# ----------------------------------------------------------------------------
$(ALVO): $(OBJETOS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJETOS) -o $@ $(LDFLAGS)
	@echo "==> Jogo compilado em $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# ----------------------------------------------------------------------------
# Compilacao dos testes (doctest)
# ----------------------------------------------------------------------------
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(TEST_DIR) -c $< -o $@

$(ALVO_TEST): $(OBJETOS_LIB) $(OBJETOS_TEST)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJETOS_LIB) $(OBJETOS_TEST) -o $@ $(LDFLAGS)
	@echo "==> Testes compilados em $@"

# ----------------------------------------------------------------------------
# Execucao
# ----------------------------------------------------------------------------
run: $(ALVO)
	@echo "==> Executando o jogo"
	@./$(ALVO)

test: $(ALVO_TEST)
	@echo "==> Executando os testes de unidade"
	@./$(ALVO_TEST)

# ----------------------------------------------------------------------------
# Cobertura de codigo
# A instrumentacao muda as flags, entao os objetos antigos sao descartados
# antes e depois para nao misturar builds instrumentados com builds normais.
# ----------------------------------------------------------------------------
coverage: CXXFLAGS += --coverage -O0 -g
coverage: LDFLAGS  += --coverage
coverage: clean-obj $(ALVO_TEST)
	@echo "==> Executando os testes instrumentados"
	@./$(ALVO_TEST)
	@echo "==> Gerando o relatorio de cobertura"
	$(GCOVR) -r . --filter $(SRC_DIR)/ --filter $(INC_DIR)/ \
	      --exclude $(TEST_DIR)/ --print-summary \
	      --html-details $(BUILD_DIR)/cobertura.html
	@echo "==> Relatorio detalhado em $(BUILD_DIR)/cobertura.html"

# ----------------------------------------------------------------------------
# Documentacao
# ----------------------------------------------------------------------------
docs:
	doxygen Doxyfile
	@echo "==> Documentacao gerada em docs/html/index.html"

# ----------------------------------------------------------------------------
# Limpeza
# ----------------------------------------------------------------------------
clean-obj:
	@rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.gcno $(BUILD_DIR)/*.gcda

clean: clean-obj
	@rm -f $(ALVO) $(ALVO_TEST) $(BUILD_DIR)/*.html $(BUILD_DIR)/*.css
	@rm -f *.gcov
	@echo "==> Diretorio build/ limpo"

# ----------------------------------------------------------------------------
# Dependencias externas
# ----------------------------------------------------------------------------
deps:
	@echo "==> Baixando o doctest.h"
	curl -sL -o $(TEST_DIR)/doctest.h \
	     https://raw.githubusercontent.com/doctest/doctest/v2.4.11/doctest/doctest.h
	@echo "==> Instalando o gcovr"
	pip install --user gcovr

ajuda:
	@echo "Alvos: all, run, test, coverage, docs, clean, deps"
