#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include "Funcoes.h"

#define TAMANHO_QUADRADO 40
#define NUM_COLUNAS 24
#define NUM_LINHAS 20

#define MAX_TERRENO (NUM_COLUNAS * NUM_LINHAS)
#define MAX_COMBUSTIVEL 50

typedef struct Terreno {
	float x;
	float y;
	bool ativo;
	bool destrutivel;
} Terreno;

typedef struct Combustivel {
	float x;
	float y;
	bool ativo;
} Combustivel;


void CarregarMapa(const char* arquivo, Jogador* jogador, Inimigo* inimigos, Terreno* terrenos, Combustivel* combustiveis);
void DesenhaMapa(Terreno* terrenos, Combustivel* combustiveis);
void AtualizaMapa(Terreno* terrenos, Combustivel* combustiveis, float velocidade);

#endif