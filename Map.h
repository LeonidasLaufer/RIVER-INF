#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include "Funcoes.h"

#define TAMANHO_QUADRADO 40
#define NUM_COLUNAS 24
#define NUM_LINHAS 20

#define MAX_FASES_SUPORTADAS 10

#define MAX_TERRENO (NUM_COLUNAS * NUM_LINHAS * MAX_FASES_SUPORTADAS)
#define MAX_COMBUSTIVEL (20 * MAX_FASES_SUPORTADAS)

#define ALTURA_MAPA (NUM_LINHAS * TAMANHO_QUADRADO)

typedef struct Terreno {
	float x;
	float y;
	bool ativo;
	bool destrutivel;
	int pontos;
} Terreno;

typedef struct Combustivel {
	float x;
	float y;
	bool ativo;
} Combustivel;

void InicializarEntidades(Inimigo* inimigos, Terreno* terrenos, Combustivel* combustiveis);
void CarregarTrechoMapa(const char* arquivo, Jogador* jogador, Inimigo* inimigos, int* qtd_inimigos, Terreno* terrenos, int* qtd_terrenos, Combustivel* combustiveis, int* qtd_combustiveis, float desvioY);
void DesenhaMapa(Terreno* terrenos, Combustivel* combustiveis, Texture2D sprite_posto);
void AtualizaMapa(Terreno* terrenos, Combustivel* combustiveis, float velocidade);

#endif