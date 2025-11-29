#define _CRT_SECURE_NO_WARNINGS

#include "Funcoes.h"
#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Map.h"
#include <ctype.h>


void menu(int op)
{
	int larguraTexto;
	int t_fonte = 70;

	larguraTexto = MeasureText("Novo Jogo", t_fonte);
	DrawText("Novo Jogo", LARGURA / 4 + 50 , ALTURA / 3 - 20, t_fonte, RAYWHITE);

	larguraTexto = MeasureText("Ranking", t_fonte);
	DrawText("Ranking", LARGURA / 4 + 50, ALTURA / 2 - 20, t_fonte, RAYWHITE);

	larguraTexto = MeasureText("Sair", t_fonte);
	DrawText("Sair", LARGURA / 4 + 50, ALTURA / 1.5 - 20, t_fonte, RAYWHITE);

	switch (op)
	{
	case MENU_NOVO_JOGO:
		larguraTexto = MeasureText("Novo Jogo", t_fonte);
		DrawRectangle(LARGURA / 4 - 50 , ALTURA / 3, 20, 20, RAYWHITE); // operador em Novo Jogo
		break;
	case MENU_RANKING:
		larguraTexto = MeasureText("Ranking", t_fonte);
		DrawRectangle(LARGURA / 4 - 50, ALTURA / 2, 20, 20, RAYWHITE); // operador em Ranking
		break;
	case MENU_SAIR:
		larguraTexto = MeasureText("Sair", t_fonte);
		DrawRectangle(LARGURA / 4 - 50, ALTURA / 1.5, 20, 20, RAYWHITE); // operador em Sair
		break;
	default:
		break;
	}
}



void textoPiscante(float intervalo, float* tempo, bool* mostrarTexto)
{
	float deltaTempo = GetFrameTime();
	*tempo += deltaTempo;

	if (*tempo >= intervalo)
	{
		*mostrarTexto = !*mostrarTexto;
		*tempo -= intervalo;
	}
}


void inicializaTiros(Tiro* tiros)
{
	for (int i = 0; i < MAX_TIROS; i++) {
		tiros[i].ativo = false; // Inativa todos os tiros
	}
	return;
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
	return;
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
	return;
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
	return;
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
	return;
}

void desenhaInimigos(Inimigo* inimigos)
{
	for (int i = 0; i < MAX_INIMIGOS; i++)
	{
		if (inimigos[i].ativo) // Verifica se inimigo está ativo
		{
			switch (inimigos[i].tipo)
			{
			case NAVIO:
				DrawRectangle(inimigos[i].x, inimigos[i].y, inimigos[i].largura, inimigos[i].altura, DARKGRAY); // Desenha navio
				break;
			case HELICOPTERO:
				DrawRectangle(inimigos[i].x, inimigos[i].y, inimigos[i].largura, inimigos[i].altura, RED); // Desenha helicoptero
				break;
			default:
				break;
			}
		}
	}
	return;
}

void checaColisoesTiro(Tiro* tiros, Inimigo* inimigos, int *pontuacao)
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

						*pontuacao += inimigos[j].pontos; // Adiciona pontos à pontuação

						break;
					}
				}
			}
		}
	}
	return;
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
	return;
}

void desenhaInfo(int pontuacao)
{
	DrawText(TextFormat("Pontuacao: %d", pontuacao), 10, 10, 40, BLACK); // Desenha pontuação na tela
	return;
}

void inicializaRanking(PontosJogador* ranking)
{
	for (int i = 0; i < MAX_RANKING; i++)
	{
		ranking[i].pontos = 0; // Inicializa pontos do ranking como 0
		ranking[i].nome[0] = '\0'; // Inicializa nomes do ranking como string vazia
	}
	return;
}

void copiaRanking(PontosJogador* ranking)
{
	FILE* fp = fopen("ranking.bin", "rb"); // Abre arquivo de ranking para leitura

	if (fp != NULL)
	{
		if (fread(ranking, sizeof(PontosJogador), MAX_RANKING, fp) != MAX_RANKING) // Lê dados do arquivo para o array de ranking
		{
			printf("Erro ao ler o arquivo de ranking.\n");
		}

		fclose(fp); // Fecha o arquivo
	}
	else
	{
		printf("Erro ao abrir arquivo de ranking\n");
		inicializaRanking(ranking);
	}

	return;
}

void salvaRanking(PontosJogador* ranking)
{
	FILE* fp = fopen("ranking.bin", "wb"); // Abre arquivo de ranking para escrita
	if (fp != NULL)
	{
		if (fwrite(ranking, sizeof(PontosJogador), MAX_RANKING, fp) != MAX_RANKING) // Escreve dados do array de ranking no arquivo
		{
			printf("Erro ao escrever no arquivo de ranking.\n");
		}
		fclose(fp); // Fecha o arquivo
	}
	else
	{
		printf("Erro ao abrir arquivo de ranking\n");
	}
	return;
}


void printaRanking(PontosJogador* ranking)
{
	for (int i = 0; i < MAX_RANKING; i++)
	{
		printf("%d. %s - %d pontos\n", i + 1, ranking[i].nome, ranking[i].pontos); // Imprime o ranking no console
	}
	return;
}


void desenhaTelaMorte(int pontuacao, PontosJogador* ranking, float intervalo, bool *mostrarTexto, float *tempoDecorrido)
{
	ClearBackground(BLACK);
	int larguraTexto = MeasureText("SE FODEU", 80);

	DrawText("SE FODEU", (LARGURA - larguraTexto) / 2, (ALTURA - 80) / 2, 80, RED); // Desenha tela de morte

	if (pontuacao > ranking[MAX_RANKING - 1].pontos) // Verifica se a pontuação é maior que a menor do ranking
	{
		textoPiscante(intervalo, tempoDecorrido, mostrarTexto);

		larguraTexto = MeasureText("NOVO RECORDE!", 50);
		if (*mostrarTexto)
		{
			DrawText("NOVO RECORDE!", (LARGURA - larguraTexto) / 2, (ALTURA - 50) / 2 - 100, 50, GOLD); // Desen ha mensagem de novo recorde
		}
	}
	larguraTexto = MeasureText("Pressione ENTER para continuar", 30);

	DrawText("Pressione ENTER para continuar", (LARGURA - larguraTexto) / 2, (ALTURA - 30) / 2 + 80, 30, WHITE); // Instrução para sair
	return;
}




void desenhaRanking(PontosJogador* ranking)
{
	ClearBackground(BLACK);
	int larguraTexto = MeasureText("RANKING", 80);
	DrawText("RANKING", (LARGURA - larguraTexto) / 2, 50, 80, GOLD); // Desenha título do ranking
	for (int i = 0; i < MAX_RANKING; i++)
	{
		larguraTexto = MeasureText(TextFormat("%d. %s - %d pontos", i + 1, ranking[i].nome, ranking[i].pontos), 40);
		DrawText(TextFormat("%d. %s - %d pontos", i + 1, ranking[i].nome, ranking[i].pontos), 100, 150 + i * 50, 40, WHITE); // Desenha cada entrada do ranking
	}
	larguraTexto = MeasureText("Pressione ENTER para voltar ao menu", 30);
	DrawText("Pressione ENTER para voltar ao menu", (LARGURA - larguraTexto) / 2, ALTURA - 100, 30, WHITE); // Instrução para sair do ranking
	return;
}

void AtualizaPosRank(int pontuacao, PontosJogador* ranking, char nome[MAX_NOME + 1])
{
	PontosJogador jog;
	strcpy(jog.nome, nome);
	jog.pontos = pontuacao;

	for (int i = 0; i < MAX_RANKING; i++)
	{
		if (pontuacao > ranking[i].pontos)
		{
			for (int j = MAX_RANKING - 1; j > i; j--)
			{
				ranking[j] = ranking[j - 1];
			}
			ranking[i] = jog;
			break;
		}
	}
}

void resetaJogo(Jogador* jogador, Tiro* tiros, Inimigo* inimigos, Terreno* terrenos, Combustivel* combustiveis, int* pontuacao, int* letras, char* nome)
{
	jogador->vidas = 1;

	inicializaTiros(tiros); // Reinicializa tiros

	*pontuacao = 0; // Reseta pontuação
	*letras = 0;
	nome[0] = '\0';

	CarregarMapa("fase1.txt", jogador, inimigos, terrenos, combustiveis);
	return;
}

void checaColisoesMapa(Jogador* jogador, Terreno* terrenos)
{
	Rectangle r_Jogador = {
		jogador->x + 8,   
		jogador->y + 5,   
		jogador->largura - 16,
		jogador->altura - 10
	};

	for (int i = 0; i < MAX_TERRENO; i++)
	{
		if (terrenos[i].ativo)
		{
			Rectangle r_Terreno = {
				terrenos[i].x,
				terrenos[i].y,
				TAMANHO_QUADRADO,
				TAMANHO_QUADRADO
			};

			if (CheckCollisionRecs(r_Jogador, r_Terreno))
			{
				jogador->vidas--;
				break;
			}
		}
	}
}