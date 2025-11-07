#include "raylib.h"
#include <stdlib.h>
#include <stdbool.h>

typedef struct Jogador {
	int x;
	int y;
	int largura;
	int altura;
	int vidas;
} Jogador;

typedef struct tiro {
	int x;
	int y;
	bool ativo;
} Tiro;

#define VEL_JOGADOR 7
#define LARG_JOGADOR 20
#define ALT_JOGADOR 20

#define LARG_TIRO 5
#define ALT_TIRO 10
#define VEL_TIRO 8
#define MAX_TIROS 8

#define LARGURA 600
#define ALTURA 600

void inicializaTiros(Tiro* tiros); 
void atira(Tiro *tiros, Jogador jogador);
void atualizaTiros(Tiro *tiros);
void desenhaTiros(Tiro* tiros);



int main() {
	Jogador jogador = { LARGURA / 2, ALTURA / 2 , LARG_JOGADOR, LARG_JOGADOR, 1 }; //Cria jogador
	Tiro tiros[MAX_TIROS]; //Cria array de tiros

	InitWindow(LARGURA, ALTURA, "Teclas"); //Inicializa janela, com certo tamanho e titulo
	SetTargetFPS(60);// Ajusta a janela para 60 frames por segundo
	inicializaTiros(tiros); // Inicializa tiros como inativos

	//Este laco repete enquanto a janela nao for fechada
	//Utilizamos ele para atualizar o estado do programa / jogo
	while (!WindowShouldClose())
	{
		atualizaTiros(tiros); // Atualiza posição e estado dos tiros a cada iteração
		
		if (IsKeyDown(KEY_RIGHT) || IsKeyDown(68)) // Movimento para a direita
		{
			jogador.x += VEL_JOGADOR;
			if (jogador.x > LARGURA - jogador.largura) 
			{  
				jogador.x = LARGURA - jogador.largura;
			}
		}
		if (IsKeyDown(KEY_LEFT) || IsKeyDown(65)) // Movimento para a esquerda
		{
			jogador.x -= VEL_JOGADOR;
			if (jogador.x < 0) 
			{ 
				jogador.x = 0;
			}
		}

		if (IsKeyPressed(32)) // Atira se barra de espaço for pressionada
		{
			atira(tiros, jogador);
		}

		// Atualiza o que eh mostrado na tela a partir do estado do jogo
		BeginDrawing(); //Inicia o ambiente de desenho na tela
		ClearBackground(RAYWHITE); //Limpa a tela e define cor de fundo
		DrawRectangle(jogador.x, jogador.y, jogador.largura, jogador.altura, BLUE); //Desenha o jogador na tela
		desenhaTiros(tiros); // Desenha o tiro na tela
		EndDrawing(); //Finaliza o ambiente de desenho na tela
	}

	CloseWindow(); // Fecha a janela
	return 0;
}

void inicializaTiros(Tiro* tiros) 
{
	for (int i = 0; i < MAX_TIROS; i++) {
		tiros[i].ativo = false; // Inativa todos os tiros
	}
}

void atira(Tiro* tiros, Jogador jogador)
{
	for (int i = 0; i < MAX_TIROS; i++)
	{
		if (!tiros[i].ativo)
		{
			tiros[i].ativo = true; // Ativa tiro
			tiros[i].x = jogador.x - LARG_TIRO / 2 + LARG_JOGADOR / 2; // Centraliza tiro com jogador
			tiros[i].y = jogador.y - ALT_JOGADOR / 2; // Ajusta altura para sair acima do jogador
			break;
		}
	}
}

void atualizaTiros(Tiro *tiros)
{
	for (int i = 0; i < MAX_TIROS; i++)
	{
		if (tiros[i].ativo)
		{
			if (tiros[i].y < 0)
			{
				tiros[i].ativo = false; // Desativa tiro caso esteja fora da tela
			}
			tiros[i].y -= VEL_TIRO; // Atualiza posição do tiro
		}
	}
}

void desenhaTiros(Tiro *tiros)
{
	for (int i = 0; i < MAX_TIROS; i++)
	{
		if (tiros[i].ativo)
		{
			DrawRectangle(tiros[i].x, tiros[i].y, LARG_TIRO, ALT_TIRO, RED); // Desenha o tiro caso ativo
		}
	}
}