#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdbool.h>

typedef struct {
	int x;
	int y;
	int largura;
	int altura;
	int vidas;
} Jogador;

typedef struct {
	int x;
	int y;
	bool ativo;
} Tiro;

typedef struct {
	int x;
	int y;
	int largura;
	int altura;
	bool ativo;
} Inimigo;

typedef struct {
	int x;
	int y;
} Spawnpoint;

#define VEL_JOGADOR 7
#define LARG_JOGADOR 20
#define ALT_JOGADOR 20

#define LARG_INIMIGO 20
#define ALT_INIMIGO 20
#define VEL_INIMIGO 1
#define MAX_INIMIGOS 5

#define LARG_TIRO 5
#define ALT_TIRO 10
#define VEL_TIRO 8
#define MAX_TIROS 8

#define LARGURA 600
#define ALTURA 600

void menu(int op);

void inicializaTiros(Tiro* tiros);
void atira(Tiro* tiros, Jogador jogador);
void atualizaTiros(Tiro* tiros);
void desenhaTiros(Tiro* tiros);

void spawnaInimigos(Inimigo* inimigos, Spawnpoint* mapa);
void atualizaInimigos(Inimigo* inimigos);
void desenhaInimigos(Inimigo* inimigos);

void checaColisoesTiro(Tiro* tiros, Inimigo* inimigos);
void checaColisoesJogador(Jogador* jogador, Inimigo* inimigos);

#endif // FUNCOES_H
