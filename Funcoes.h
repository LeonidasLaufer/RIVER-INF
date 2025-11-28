#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdbool.h>

#define MAX_RANKING 10
#define MAX_NOME 8

#define VEL_JOGADOR 7
#define LARG_JOGADOR 40
#define ALT_JOGADOR 40

#define LARG_INIMIGO 40
#define ALT_INIMIGO 40
#define VEL_INIMIGO 3
#define MAX_INIMIGOS 5

#define LARG_TIRO 5
#define ALT_TIRO 10
#define VEL_TIRO 8
#define MAX_TIROS 8

#define LARGURA 800
#define ALTURA 960

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

typedef struct {
	int x;
	int y;
	int largura;
	int altura;
	int vidas;
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

typedef struct {
	int x;
	int y;
} Spawnpoint;

void menu(int op);

void textoPiscante(float intervalo, float* tempo, bool* mostrarTexto);

void inicializaTiros(Tiro* tiros);
void atira(Tiro* tiros, Jogador jogador);
void atualizaTiros(Tiro* tiros);
void desenhaTiros(Tiro* tiros);

void spawnaInimigos(Inimigo* inimigos, Spawnpoint* mapa);
void atualizaInimigos(Inimigo* inimigos);
void desenhaInimigos(Inimigo* inimigos);

void checaColisoesTiro(Tiro* tiros, Inimigo* inimigos, int *pontuacao);
void checaColisoesJogador(Jogador* jogador, Inimigo* inimigos);

void desenhaInfo(int pontuacao);

void inicializaRanking(PontosJogador* ranking);
void copiaRanking(PontosJogador* ranking);
void salvaRanking(PontosJogador* ranking);
void printaRanking(PontosJogador* ranking);
void desenhaRanking(PontosJogador* ranking);
void AtualizaPosRank(int pontuacao, PontosJogador* ranking, char nome[MAX_NOME + 1]);

void desenhaTelaMorte(int pontuacao, PontosJogador* ranking, float intervalo, bool *mostrarTexto, float *tempoDecorrido);
void resetaJogo(Jogador* jogador, Tiro* tiros, Inimigo* inimigos, Spawnpoint* mapa, int* pontuacao, int *letras, char[MAX_NOME + 1]);



#endif // FUNCOES_H
