# Definição das variáveis de diretório
PASSAGEIRO = ./Passageiro
PONTO = ./Ponto
ONIBUS= ./Onibus
FILA = ./Fila
CONJUNTO_DE_STRUCTS = ./Conjunto_de_Structs

# Definição dos arquivos fonte
SRC_FILA = $(FILA)/Fila.c 
SRC_PASSAGEIRO= $(PASSAGEIRO)/Passageiro.c
SRC_PONTO = $(PONTO)/Ponto.c
SRC_ONIBUS = $(ONIBUS)/Onibus.c


all: 
	gcc -c $(SRC_FILA) -I $(CONJUNTO_DE_STRUCTS)/ -o Fila.o
	gcc -c $(SRC_PASSAGEIRO) -I $(CONJUNTO_DE_STRUCTS)/ -o Passageiro.o -pthread
	gcc -c $(SRC_PONTO) -I $(CONJUNTO_DE_STRUCTS)/ -o Ponto.o -pthread
	gcc -c $(SRC_ONIBUS) -I $(CONJUNTO_DE_STRUCTS)/ -o Onibus.o -pthread
	gcc Main.c Fila.o  Passageiro.o Ponto.o Onibus.o -I $(CONJUNTO_DE_STRUCTS)/  -o Main


run:
	./Main

clear:
	 rm ./Main ./Passageiro.o ./Fila.o ./Ponto.o ./Onibus.o 
