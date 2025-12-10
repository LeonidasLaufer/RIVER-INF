#include "map.h"
#include "Funcoes.h"
#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>

void InicializarEntidades(Inimigo* inimigos, Terreno* terrenos, Combustivel* combustiveis) {
	for (int i = 0; i < MAX_INIMIGOS; i++) {
		inimigos[i].ativo = false;
	}
	for (int i = 0; i < MAX_TERRENO; i++) {
		terrenos[i].ativo = false;
	}
	for (int i = 0; i < MAX_COMBUSTIVEL; i++) {
		combustiveis[i].ativo = false;
	}
}

void CarregarTrechoMapa(const char* arquivo, Jogador* jogador, Inimigo* inimigos, int* qtd_inimigos, Terreno* terrenos, int* qtd_terrenos, Combustivel* combustiveis, int* qtd_combustiveis, float desvioY) {


	FILE* fp = fopen(arquivo, "r"); 
	if (fp == NULL) {
		printf("Erro ao abrir o arquivo do mapa.\n");
		return;
	}


	char ch;
	int linha = 0;
	int coluna = 0;



	while ((ch = fgetc(fp)) != EOF && linha < NUM_LINHAS) {

		printf("[%c]", ch);


		if (ch == '\r') 
			continue;

		if (ch == '\n') {
			linha++;
			coluna = 0;
			continue;
		}

		if (coluna >= NUM_COLUNAS) {
			continue;
		}

		float posX = coluna * TAMANHO_QUADRADO;
		float posY = (linha * TAMANHO_QUADRADO) + desvioY;

		switch (ch)
		{
		case 'T':
			if (*qtd_terrenos < MAX_TERRENO) {
				int i = *qtd_terrenos;
				terrenos[i].x = posX;
				terrenos[i].y = posY;
				terrenos[i].ativo = true;
				terrenos[i].destrutivel = false;
				(*qtd_terrenos)++;
			}
			break;

		case 'P':
			if (*qtd_terrenos < MAX_TERRENO) {
				int i = *qtd_terrenos;
				terrenos[i].x = posX;
				terrenos[i].y = posY;
				terrenos[i].ativo = true;
				terrenos[i].destrutivel = true;
				terrenos[i].pontos = 60;
				(*qtd_terrenos)++;
			}
			break;

		case 'G':
			if (*qtd_combustiveis < MAX_COMBUSTIVEL) {
				int i = *qtd_combustiveis;
				combustiveis[i].x = posX;
				combustiveis[i].y = posY;
				combustiveis[i].ativo = true;
				(*qtd_combustiveis)++;
			}
			break;

		case 'N':
			if (*qtd_inimigos < MAX_INIMIGOS) {
				int i = *qtd_inimigos;
				inimigos[i].x = posX;
				inimigos[i].y = posY;
				inimigos[i].largura = LARG_NAVIO;
				inimigos[i].altura = ALT_NAVIO;
				inimigos[i].tipo = NAVIO;
				inimigos[i].pontos = 30;
				inimigos[i].ativo = true;
				(*qtd_inimigos)++;
			}
			break;

		case 'X':
			if (*qtd_inimigos < MAX_INIMIGOS) {
				int i = *qtd_inimigos;
				inimigos[i].x = posX;
				inimigos[i].y = posY;
				inimigos[i].largura = LARG_HELI;
				inimigos[i].altura = ALT_HELI;
				inimigos[i].tipo = HELICOPTERO;
				inimigos[i].pontos = 60;
				inimigos[i].ativo = true;
				(*qtd_inimigos)++;
			}
			break;

		case 'A':
			if (desvioY == 0) {
				jogador->x = posX;
				jogador->y = posY;
			}
			break;
		}

		if (ch != '\n') {
			coluna++;
		}
	}

	fclose(fp);
	}

void AtualizaMapa(Terreno* terrenos, Combustivel* combustiveis, float velocidade)
{

	for (int i = 0; i < MAX_TERRENO; i++) {
		if (terrenos[i].ativo) {
			terrenos[i].y += velocidade;

			if (terrenos[i].y > ALTURA + 100) {
				terrenos[i].ativo = false;
			}
		}
	}

	for (int i = 0; i < MAX_COMBUSTIVEL; i++) {
		if (combustiveis[i].ativo) {
			combustiveis[i].y += velocidade;
			if (combustiveis[i].y > ALTURA + 100) {
				combustiveis[i].ativo = false;
			}
		}
	}
}

void DesenhaMapa(Terreno* terrenos, Combustivel* combustiveis, Texture2D sprite_posto)
{
	for (int i = 0; i < MAX_TERRENO; i++) {
		if (terrenos[i].ativo) {
			Color cor;
			if (terrenos[i].destrutivel == true) {
				cor = GRAY;
			}
			else {
				cor = DARKGREEN;
			}DrawRectangle(terrenos[i].x, terrenos[i].y, TAMANHO_QUADRADO, TAMANHO_QUADRADO, cor);

			if (terrenos[i].destrutivel)
				DrawRectangleLines(terrenos[i].x, terrenos[i].y, TAMANHO_QUADRADO, TAMANHO_QUADRADO, BLACK);
		}
	}

	for (int i = 0; i < MAX_COMBUSTIVEL; i++) {
		if (combustiveis[i].ativo) {
			DrawTexture(sprite_posto, combustiveis[i].x, combustiveis[i].y, WHITE);
		}
	}
}