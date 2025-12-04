#include "map.h"
#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>

void CarregarMapa(const char* arquivo, Jogador* jogador, Inimigo* inimigos, Terreno* terrenos, Combustivel* combustiveis) {

	for (int i = 0; i < MAX_INIMIGOS; i++) {
		inimigos[i].ativo = false;
	}
	for (int i = 0; i < MAX_TERRENO; i++) {
		terrenos[i].ativo = false;
	}
	for (int i = 0; i < MAX_COMBUSTIVEL; i++) {
		combustiveis[i].ativo = false;
	}

	FILE* fp = fopen(arquivo, "r"); 
	if (fp == NULL) {
		printf("Erro ao abrir o arquivo do mapa.\n");
		return;
	}


	char ch;
	int linha = 0;
	int coluna = 0;
	int c_inimigo = 0;
	int c_terreno = 0;
	int c_gas = 0;


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
		float posY = linha * TAMANHO_QUADRADO;

		switch (ch)
		{
		case 'T':
			if (c_terreno < MAX_TERRENO) {
				terrenos[c_terreno].x = posX;
				terrenos[c_terreno].y = posY;
				terrenos[c_terreno].ativo = true;
				terrenos[c_terreno].destrutivel = false;
				c_terreno++;
			}
			break;

		case 'P':
			if (c_terreno < MAX_TERRENO) {
				terrenos[c_terreno].x = posX;
				terrenos[c_terreno].y = posY;
				terrenos[c_terreno].ativo = true;
				terrenos[c_terreno].destrutivel = true;
				c_terreno++;
			}
			break;

		case 'G':
			if (c_gas < MAX_COMBUSTIVEL) {
				combustiveis[c_gas].x = posX;
				combustiveis[c_gas].y = posY;
				combustiveis[c_gas].ativo = true;
				c_gas++;
			}
			break;

		case 'N':
			if (c_inimigo < MAX_INIMIGOS) {
				inimigos[c_inimigo].x = posX;
				inimigos[c_inimigo].y = posY;
				inimigos[c_inimigo].largura = TAMANHO_QUADRADO;
				inimigos[c_inimigo].altura = TAMANHO_QUADRADO;
				inimigos[c_inimigo].tipo = NAVIO;
				inimigos[c_inimigo].pontos = 30;
				inimigos[c_inimigo].ativo = true;
				c_inimigo++;
			}
			break;

		case 'X':
			if (c_inimigo < MAX_INIMIGOS) {
				inimigos[c_inimigo].x = posX;
				inimigos[c_inimigo].y = posY;
				inimigos[c_inimigo].largura = TAMANHO_QUADRADO;
				inimigos[c_inimigo].altura = TAMANHO_QUADRADO;
				inimigos[c_inimigo].tipo = HELICOPTERO;
				inimigos[c_inimigo].pontos = 30;
				inimigos[c_inimigo].ativo = true;
				c_inimigo++;
			}
			break;

		case 'A':
			jogador->x = posX;
			jogador->y = posY;

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

			if (terrenos[i].y > NUM_LINHAS * TAMANHO_QUADRADO) {
				terrenos[i].ativo = false;
			}
		}
	}

	for (int i = 0; i < MAX_COMBUSTIVEL; i++) {
		if (combustiveis[i].ativo) {
			combustiveis[i].y += velocidade;
			if (combustiveis[i].y > NUM_LINHAS * TAMANHO_QUADRADO) {
				combustiveis[i].ativo = false;
			}
		}
	}
}

void DesenhaMapa(Terreno* terrenos, Combustivel* combustiveis)
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
			DrawRectangle(combustiveis[i].x, combustiveis[i].y, TAMANHO_QUADRADO, TAMANHO_QUADRADO, ORANGE);
			DrawText("F", combustiveis[i].x + 12, combustiveis[i].y + 5, 20, BLACK);
		}
	}
}