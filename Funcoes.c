#include "Funcoes.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>

void menu(int op)
{
	DrawText("Novo Jogo", LARGURA / 4, ALTURA / 3 - 20, 60, RAYWHITE);
	DrawText("Ranking", LARGURA / 4, ALTURA / 2 - 20, 60, RAYWHITE);
	DrawText("Sair", LARGURA / 4, ALTURA / 1.5 - 20, 60, RAYWHITE);
	switch (op)
	{
	case 0:
		DrawRectangle(LARGURA / 4 - 50, ALTURA / 3, 20, 20, RAYWHITE); // operador em Novo Jogo
		break;
	case 1:
		DrawRectangle(LARGURA / 4 - 50, ALTURA / 2, 20, 20, RAYWHITE); // operador em Ranking
		break;
	case 2:
		DrawRectangle(LARGURA / 4 - 50, ALTURA / 1.5, 20, 20, RAYWHITE); // operador em Sair
		break;
	default:
		break;
	}
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

void atualizaTiros(Tiro* tiros)
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

void desenhaTiros(Tiro* tiros)
{
	for (int i = 0; i < MAX_TIROS; i++)
	{
		if (tiros[i].ativo)
		{
			DrawRectangle(tiros[i].x, tiros[i].y, LARG_TIRO, ALT_TIRO, RED); // Desenha o tiro caso ativo
		}
	}
}


void spawnaInimigos(Inimigo* inimigos, Spawnpoint* mapa)
{
	for (int i = 0; i < MAX_INIMIGOS; i++)
	{
		inimigos[i].ativo = true; // Ativa inimigo
		inimigos[i].largura = LARG_INIMIGO; // Define largura do inimigo
		inimigos[i].altura = ALT_INIMIGO; // Define altura do inimigo
		inimigos[i].x = mapa[i].x;
		inimigos[i].y = mapa[i].y;
	}
}

void atualizaInimigos(Inimigo* inimigos)
{
	for (int i = 0; i < MAX_INIMIGOS; i++)
	{
		if (inimigos[i].ativo)
		{
			inimigos[i].y += VEL_INIMIGO; // Atualiza posição do inimigo
			if (inimigos[i].y > ALTURA)
			{
				inimigos[i].ativo = false; // Desativa inimigo caso esteja fora da tela
			}
		}
	}
}

void desenhaInimigos(Inimigo* inimigos)
{
	for (int i = 0; i < MAX_INIMIGOS; i++)
	{
		if (inimigos[i].ativo)
		{
			DrawRectangle(inimigos[i].x, inimigos[i].y, inimigos[i].largura, inimigos[i].altura, GREEN); // Desenha o inimigo caso ativo
		}
	}
}

void checaColisoesTiro(Tiro* tiros, Inimigo* inimigos)
{
	for (int i = 0; i < MAX_TIROS; i++)
	{
		if (tiros[i].ativo)
		{
			for (int j = 0; j < MAX_INIMIGOS; j++)
			{
				if (inimigos[j].ativo)
				{
					Rectangle r_Tiro = { tiros[i].x, tiros[i].y, LARG_TIRO, ALT_TIRO }; // Cria retangulo do tiro
					Rectangle r_Inimigo = { inimigos[j].x, inimigos[j].y, inimigos[j].largura, inimigos[j].altura }; // Cria retangulo do inimigo
					if (CheckCollisionRecs(r_Tiro, r_Inimigo)) // Checa colisao entre os retangulos
					{
						tiros[i].ativo = false;
						inimigos[j].ativo = false; // Desativa ambos caso haja colisao
						break;
					}
				}
			}
		}
	}
}

void checaColisoesJogador(Jogador* jogador, Inimigo* inimigo)
{
	for (int i = 0; i < MAX_INIMIGOS; i++)
	{
		if (inimigo[i].ativo)
		{
			Rectangle r_Inimigo = { inimigo[i].x, inimigo[i].y, inimigo[i].largura, inimigo[i].altura }; // Cria retangulo do inimigo
			Rectangle r_Jogador = { jogador->x, jogador->y, jogador->largura, jogador->altura }; // Cria retangulo do jogador
			if (CheckCollisionRecs(r_Inimigo, r_Jogador))
			{
				jogador->vidas--; // Diminui vida do jogador caso haja colisao
			}
		}
	}
}