#ifndef FUNCOES_H
#define FUNCOES_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_RANKING 10
#define MAX_NOME 8

#define VEL_JOGADOR 4
#define LARG_JOGADOR 58
#define ALT_JOGADOR 54

#define LARG_NAVIO 130
#define ALT_NAVIO 34
#define LARG_HELI 64
#define ALT_HELI 40
#define MAX_INIMIGOS 5 * MAX_FASES_SUPORTADAS

#define LARG_TIRO 5
#define ALT_TIRO 10
#define VEL_TIRO 8
#define MAX_TIROS 3

#define LARG_POSTO 56
#define ALT_POSTO 96

#define LARGURA 960
#define ALTURA 800


typedef enum {
	EST_MENU,
	EST_JOGO,
	EST_PAUSE,
	EST_MORTE,
	EST_RANKING
}EstadoJogo;

typedef enum {
	NAVIO,
	HELICOPTERO
}TipoInimigo;

typedef enum {
	MENU_NOVO_JOGO,
	MENU_RANKING,
	MENU_SAIR
}MenuOp;

typedef enum {
	PARADO,
	DIREITA,
	ESQUERDA
}EstadoJogador;

typedef struct {
	int x;
	int y;
	int largura;
	int altura;
	int vidas;
	int combustivel;
} Jogador;

typedef struct {
	int pontos;
	char nome[MAX_NOME + 1];
} PontosJogador;

typedef struct {
	int x;
	int y;
	bool ativo;
} Tiro;

typedef struct {
	TipoInimigo tipo;
	int x;
	int y;
	int largura;
	int altura;
	int pontos;
	bool ativo;
} Inimigo;

typedef struct Terreno Terreno;

typedef struct Combustivel Combustivel;


void menu(int op);
void textoPiscante(float intervalo, float* tempo, bool* mostrarTexto);

void inicializaTiros(Tiro* tiros);
void atira(Tiro* tiros, Jogador jogador);
void atualizaTiros(Tiro* tiros);
void desenhaTiros(Tiro* tiros);
void desenhaJogador(Jogador jogador, Texture2D sprite);

void atualizaInimigos(Inimigo* inimigos);
void desenhaInimigos(Inimigo* inimigos, Texture2D sprite_nav, Texture2D sprite_heli);

void checaColisoesTiro(Tiro* tiros, Inimigo* inimigos, int *pontuacao);
void checaColisoesJogador(Jogador* jogador, Inimigo* inimigos);
void checaColisoesMapa(Jogador* jogador, Terreno* terrenos, Tiro* tiros, Combustivel* posto, int* ac_res, int* pontuacao, bool* ÈPosto);

void desenhaInfo(int pontuacao, Jogador jogador);
void desenhaTelaMorte(int pontuacao, PontosJogador* ranking, float intervalo, bool* mostrarTexto, float* tempoDecorrido);


void inicializaRanking(PontosJogador* ranking);
void copiaRanking(PontosJogador* ranking);
void salvaRanking(PontosJogador* ranking);
void printaRanking(PontosJogador* ranking);
void desenhaRanking(PontosJogador* ranking);
void AtualizaPosRank(int pontuacao, PontosJogador* ranking, char nome[MAX_NOME + 1]);

void resetaJogo(Jogador* jogador, Tiro* tiros, Inimigo* inimigos, Terreno* terrenos, Combustivel* combustiveis, int* pontuacao, int* letras, char* nome);


#endif // FUNCOES_H
