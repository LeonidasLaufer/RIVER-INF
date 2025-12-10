	#define _CRT_SECURE_NO_WARNINGS

	#include "Funcoes.h"
	#include "raylib.h"
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "Map.h"
	#include <ctype.h>
	#include <math.h>


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

		float velocidadeinimigo = 2.0f;

		for (int i = 0; i < MAX_INIMIGOS; i++)
		{
			if (inimigos[i].ativo){
				inimigos[i].y += velocidadeinimigo;
				if (inimigos[i].y > ALTURA + 100) {
					inimigos[i].ativo = false;
				}
				}
		}
		return;
	}

	void desenhaInimigos(Inimigo* inimigos, Texture2D sprite_nav, Texture2D sprite_heli)
	{
		for (int i = 0; i < MAX_INIMIGOS; i++)
		{
			if (inimigos[i].ativo) // Verifica se inimigo está ativo
			{
				switch (inimigos[i].tipo)
				{
				case NAVIO:
					DrawTexture(sprite_nav, inimigos[i].x, inimigos[i].y, WHITE); // Desenha navio
					break;
				case HELICOPTERO:
					DrawTexture(sprite_heli, inimigos[i].x, inimigos[i].y, WHITE); // Desenha helicoptero
					break;
				default:
					break;
				}
			}
		}
		return;
	}

	void desenhaJogador(Jogador jogador, Texture2D sprite)
	{
		DrawTexture(sprite, jogador.x, jogador.y, WHITE);
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
					inimigo[i].ativo = false;
				}
			}
		}
		return;
	}

	void desenhaInfo(int pontuacao, Jogador jogador)
	{
		DrawText(TextFormat("Score: %d", pontuacao), 10, 10, 40, GOLD); // Desenha pontuação na tela
		DrawText(TextFormat("Combustivel: %d", jogador.combustivel), 300, 10, 40, GOLD);
		DrawText(TextFormat("Vidas: %d", jogador.vidas), 750, 10, 40, GOLD);
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
		jogador->vidas = 3;
		jogador->combustivel = 100;

		inicializaTiros(tiros); // Reinicializa tiros

		*pontuacao = 0; // Reseta pontuação
		*letras = 0;
		nome[0] = '\0';

		const char* listaFases[] = { "fase1.txt", "fase2.txt", "fase3.txt", "fase4.txt", "fase5.txt"};
		int totalFases = sizeof(listaFases) / sizeof(listaFases[0]);

		InicializarEntidades(inimigos, terrenos, combustiveis);

		int qtd_inimigos = 0;
		int qtd_terrenos = 0;
		int qtd_combustiveis = 0;

		for (int i = 0; i < totalFases; i++) {
			float offset = i * -((float)ALTURA_MAPA);
			CarregarTrechoMapa(listaFases[i], jogador, inimigos, &qtd_inimigos, terrenos, &qtd_terrenos, combustiveis, &qtd_combustiveis, offset);
		}
		 return;
	}

	void destruirponte(Terreno* terrenos, int indiceOrigem, int* pontuacao) {
		float origemY = terrenos[indiceOrigem].y;

		int i = indiceOrigem + 1;
		float origemX = terrenos[indiceOrigem].x + TAMANHO_QUADRADO;

		while (i < MAX_TERRENO) {
			if (terrenos[i].ativo && terrenos[i].destrutivel && terrenos[i].y == origemY && fabs(terrenos[i].x - origemX) < 2.0f)
			{
				terrenos[i].ativo = false;
				*pontuacao += terrenos[i].pontos;
				origemX += TAMANHO_QUADRADO;
				i++;
			}
			else
			{
				break;
			}
		}
		i = indiceOrigem - 1;
		origemX = terrenos[indiceOrigem].x - TAMANHO_QUADRADO;

		while (i >= 0) {
			if (terrenos[i].ativo && terrenos[i].destrutivel && terrenos[i].y == origemY && fabs(terrenos[i].x - origemX) < 2.0f) {
				terrenos[i].ativo = false;
				*pontuacao += terrenos[i].pontos;
				origemX -= TAMANHO_QUADRADO;
				i--;
			}
			else {
				break;
			}
		}
	}

	void checaColisoesMapa(Jogador* jogador, Terreno* terrenos, Tiro* tiros, Combustivel* posto, int* ac_res, int* pontuacao, bool* éPosto, bool* vitoria)
	{
		Rectangle r_Jogador = {
			jogador->x + 8,
			jogador->y + 5,
			jogador->largura - 16,
			jogador->altura - 10
		};

		Rectangle r_tiro;
		Rectangle r_posto;



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
					jogador->vidas = 0;
					break;
				}
				for (int j = 0; j < MAX_TIROS; j++)
				{
					if (tiros[j].ativo)
					{
						r_tiro = (Rectangle){ tiros[j].x, tiros[j].y, LARG_TIRO, ALT_TIRO };
						if (CheckCollisionRecs(r_tiro, r_Terreno))
						{
							tiros[j].ativo = false;
							if (terrenos[i].destrutivel)
							{
								terrenos[i].ativo = false;
								*pontuacao += terrenos[i].pontos;
								destruirponte(terrenos, i, pontuacao);
								*vitoria = true;
							}
						}
					}
				}
			}
		}

		*éPosto = false;

		for (int i = 0; i < MAX_COMBUSTIVEL; i++)
		{
			if (posto[i].ativo)
			{
				r_posto = (Rectangle){
					posto[i].x,
					posto[i].y,
					LARG_POSTO,
					ALT_POSTO
				};

				if (CheckCollisionRecs(r_posto, r_Jogador))
				{
					*éPosto = true;
					(*ac_res)++;
					if (*ac_res >= 3 && jogador->combustivel < 100)
					{
						jogador->combustivel++;
						*ac_res = 0;

					}
				}
			}
		}
	}


	void desenhaTelaVitoria(int pontuacao, PontosJogador* ranking, float intervalo, bool* mostrarTexto, float* tempoDecorrido)
	{
		ClearBackground(BLACK);
		int larguraTexto = MeasureText("PARABEINS!", 80);
		DrawText("PARABEINS!", (LARGURA - larguraTexto) / 2, (ALTURA - 80) / 2, 80, GREEN); // Desenha tela de vitoria
	
		if (pontuacao > ranking[MAX_RANKING - 1].pontos) // Verifica se a pontuação é maior que a menor do ranking
		{
			textoPiscante(intervalo, tempoDecorrido, mostrarTexto);
			larguraTexto = MeasureText("NOVO RECORDE!", 50);
			if (*mostrarTexto)
			{
				DrawText("NOVO RECORDE!", (LARGURA - larguraTexto) / 2, (ALTURA - 50) / 2 - 100, 50, GOLD); // Desenha mensagem de novo recorde
			}
		}
			DrawText("Pressione ENTER para continuar", (LARGURA - larguraTexto) / 2, (ALTURA - 30) / 2 + 80, 30, WHITE); // Instrução para sair
		return;
	}