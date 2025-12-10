#define _CRT_SECURE_NO_WARNINGS

#include "Funcoes.h"
#include "raylib.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "Map.h"



int main() {

	Jogador jogador = { LARGURA / 2, ALTURA / 2 , LARG_JOGADOR, LARG_JOGADOR, 3, 100 }; //Cria jogador
	EstadoJogador Est_jogador = PARADO;
	
	static Tiro tiros[MAX_TIROS]; //Cria array de tiros
	static Inimigo inimigos[MAX_INIMIGOS];
	static Terreno terrenos[MAX_TERRENO];
	static Combustivel combustiveis[MAX_COMBUSTIVEL];

	EstadoJogo Estado = EST_MENU; // Define estado do jogo inicialmenmte como menu

	bool jogoTerminado = false;
	bool inserirRanking = false; // Variavel verifica se o jogador pode ser inserido no ranking

	bool sair = false;
	MenuOp M_op = 0; // Operador do menu principal

	float intervalo = 0.5f; 
	float tempoDecorrido = 0.0f;
	float deltaTempo;
	bool mostrarTexto = true;

	float velocidadeCenario = 1.0f;

	int larguraTexto = 0; // Variavel utilizada para centralizar texto

	int letras = 0; // Letras do nome inserido
	char nomeJogador[MAX_NOME + 1] = { '\0' };

	Rectangle caixaTexto = { (LARGURA - 300) / 2, ALTURA / 2 , 300, 100 }; // Delimita caixa de texto do nome

	int pontuacao = 0;
	static PontosJogador ranking[MAX_RANKING]; // Array do ranking

	int ac_combustivel = 0;
	int ac_vida = 0;
	int ac_reabastecimento = 0;
	bool ÈPosto = false;

	InitWindow(LARGURA, ALTURA, "River-Inf"); //Inicializa janela
	SetTargetFPS(60);// Ajusta a janela para 60 frames por segundo
	inicializaTiros(tiros); // Inicializa tiros como inativos

	const char* listaFases[] = { "fase1.txt", "fase2.txt", "fase3.txt"};
	int totalFases = sizeof(listaFases) / sizeof(listaFases[0]);

	Texture2D sprite_jog = LoadTexture("Sprites/Jogador.png");
	Texture2D sprite_jogDir = LoadTexture("Sprites/JogDireita.png");
	Texture2D sprite_jogEsq = LoadTexture("Sprites/JogEsquerda.png");
	Texture2D sprite_nav = LoadTexture("Sprites/Navio.png");
	Texture2D sprite_heli = LoadTexture("Sprites/Helicoptero.png");
	Texture2D sprite_posto = LoadTexture("Sprites/Posto.png");
	

	InicializarEntidades(inimigos, terrenos, combustiveis);

	int qtd_inimigos = 0;
	int qtd_terrenos = 0;
	int qtd_combustiveis = 0;

	for (int i = 0; i < totalFases; i++) {
		float offset = i * -((float)ALTURA_MAPA);
		CarregarTrechoMapa(listaFases[i], &jogador, inimigos, &qtd_inimigos, terrenos, &qtd_terrenos, combustiveis, &qtd_combustiveis, offset);
	}


	copiaRanking(ranking); // Carrega ranking de arquivo
	printaRanking(ranking); // Imprime ranking no console

	while (!WindowShouldClose() && !sair) // Enquanto o ESC e o bot„o de sair nao forem pressionados
	{
		switch (Estado)
		{
		case EST_MENU: // Estado do menu
			if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && M_op != MENU_NOVO_JOGO)
			{
				M_op--; // Move o cursor para cima caso nao esteja no topo
			}
			else if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && M_op != MENU_SAIR)
			{
				M_op++; // Move o cursor para baixa caso nao esteja no fundo
			}


			if (IsKeyPressed(KEY_ENTER)) // Se enmter for pressonado
			{
				switch (M_op)
				{
				case MENU_NOVO_JOGO:
					Estado = EST_JOGO; // ComeÁa o jogo caso cursor esteja em Novo Jogo
					break;
				case MENU_RANKING:
					Estado = EST_RANKING; // Entra no ranking caso cursor esteja no ranking
					break;
				case MENU_SAIR: // Sai caso cursor esteja em sair
					sair = true;
					break;
				default:
					M_op = MENU_NOVO_JOGO;
				}
					
			}


			BeginDrawing();
			ClearBackground(SKYBLUE);
			menu(M_op); // Desenha o menu
			EndDrawing();
			break;


		case EST_JOGO:

			if (!ÈPosto)
			{
				ac_combustivel += velocidadeCenario;
				if (ac_combustivel >= 20)
				{
					jogador.combustivel--;
					ac_combustivel = 0;
				}
			}
			
			ac_vida = pontuacao;
			if (ac_vida >= 1000)
			{
				jogador.vidas++;
				ac_vida = 0;
			}

			atualizaTiros(tiros); // Atualiza posiÁ„o e estado dos tiros a cada iteraÁ„o
			atualizaInimigos(inimigos); // Atualiza posiÁ„o e estado dos inimigos a cada iteraÁ„o
			

			AtualizaMapa(terrenos, combustiveis, velocidadeCenario);

			checaColisoesTiro(tiros, inimigos, &pontuacao); // Checa colisıes entre tiros e inimigos
			checaColisoesJogador(&jogador, inimigos); // Checa colisıes entre jogador e inimigos
			checaColisoesMapa(&jogador, terrenos, tiros, combustiveis, &ac_reabastecimento, &pontuacao, &ÈPosto); // Checa colisıes entre jogador e mapa


			if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) // Movimento para a direita
			{
				jogador.x += VEL_JOGADOR;
				if (jogador.x > LARGURA - jogador.largura)
				{
					jogador.x = LARGURA - jogador.largura;
				}
				Est_jogador = DIREITA;
			}
			else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) // Movimento para a esquerda
			{
				jogador.x -= VEL_JOGADOR;
				if (jogador.x < 0)
				{
					jogador.x = 0;
				}
				Est_jogador = ESQUERDA;
			}
			else
			{
				Est_jogador = PARADO;
			}

			if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_K)) // Se barra de espaÁo ou K for pressionado
			{
				atira(tiros, jogador); // Atira
			}

			if (jogador.vidas == 0) // Caso jogador fique sem vidas
			{
				Estado = EST_MORTE; // Muda estado para Morte
			}


			BeginDrawing();
			ClearBackground(BLUE);

			DesenhaMapa(terrenos, combustiveis, sprite_posto);

			switch (Est_jogador)
			{
			case PARADO:
				desenhaJogador(jogador, sprite_jog);
				break;
			case DIREITA:
				desenhaJogador(jogador, sprite_jogDir);
				break;
			case ESQUERDA:
				desenhaJogador(jogador, sprite_jogEsq);
				break;
			default:
				break;
			}
			desenhaTiros(tiros); // Desenha o tiro na tela
			desenhaInimigos(inimigos, sprite_nav, sprite_heli); // Desenha os inimigos na tela
			desenhaInfo(pontuacao, jogador); // Desenha a pontuaÁ„o na tela

			EndDrawing();
			break;

		case EST_MORTE: // Caso jogador tenha morrido
			if (!inserirRanking) // Checa se jogador pode ser inserido nor ranking
			{
				if (IsKeyPressed(KEY_ENTER))
				{
					if (pontuacao > ranking[MAX_RANKING - 1].pontos)
					{
						inserirRanking = !inserirRanking;
					}
					else
					{
						M_op = MENU_NOVO_JOGO;
						resetaJogo(&jogador, tiros, inimigos, terrenos, combustiveis, &pontuacao, &letras, nomeJogador);
						Estado = EST_MENU;
					}
				}
				BeginDrawing();
				desenhaTelaMorte(pontuacao, ranking, intervalo, &mostrarTexto, &tempoDecorrido);
				EndDrawing();
			}
			else
			{
				int tecla = GetCharPressed(); // Pega o caractere pressionado

				while (tecla > 0 && letras < MAX_NOME)
				{
					if ((tecla >= 32) && (tecla <= 125)) // Verifica se o caractere È imprimÌvel
					{
						nomeJogador[letras] = toupper((char)tecla); // Adiciona caractere ao nome do jogador
						letras++;
						nomeJogador[letras] = '\0'; // Adiciona o terminador de string
					}
					tecla = GetCharPressed(); // Pega o prÛximo caractere pressionado
				}

				if (IsKeyPressed(KEY_BACKSPACE)) // Permite apagar o ˙ltimo caractere
				{
					if (letras > 0)
					{
						letras--;
					}
					nomeJogador[letras] = '\0'; // Atualiza o terminador de string
				}

				if (IsKeyPressed(KEY_ENTER) && letras >= 3)
				{
					AtualizaPosRank(pontuacao, ranking, nomeJogador);
					salvaRanking(ranking);
					resetaJogo(&jogador, tiros, inimigos, terrenos, combustiveis, &pontuacao, &letras, nomeJogador); 
					Estado = EST_RANKING;
				}

				BeginDrawing();
				ClearBackground(BLACK);

				textoPiscante(intervalo, &tempoDecorrido, &mostrarTexto);

				larguraTexto = MeasureText("INSIRA SEU NOME", 60);

				if (mostrarTexto)
				{
					DrawText("INSIRA SEU NOME", (LARGURA - larguraTexto) / 2, (ALTURA - 60) / 2 - 200, 60, GOLD);
				}


				DrawText(nomeJogador, caixaTexto.x + 10, caixaTexto.y + 10, 50, RAYWHITE);

				larguraTexto = MeasureText("Pressione ENTER para confirmar", 30);
				DrawText("Pressione ENTER para confirmar", (LARGURA - larguraTexto) / 2, ((ALTURA - 40) / 2) + 300, 30, RAYWHITE);

				larguraTexto = MeasureText(TextFormat("Chars: %d/%d", letras, MAX_NOME), 20);
				DrawText(TextFormat("Chars: %d/%d", letras, MAX_NOME), (LARGURA - larguraTexto) / 2, ((ALTURA - 20) / 2) + 80, 20, RAYWHITE);

				EndDrawing();

			}
			
			break;

		case EST_RANKING:
			BeginDrawing();
			desenhaRanking(ranking); // Desenha o ranking caso ENTER tenha sido pressionado, e caso cursor esteja em Ranking
			EndDrawing();
			if (IsKeyPressed(KEY_ENTER))
			{
				
				M_op = MENU_NOVO_JOGO;
				inserirRanking = false;
				Estado = EST_MENU;

				while (IsKeyDown(KEY_ENTER))
				{
					BeginDrawing();
					EndDrawing();
				}
			}
			break;

		default:
			Estado = EST_MENU;
			break;
		}

		

	}

	UnloadTexture(sprite_jog);
	UnloadTexture(sprite_nav);
	UnloadTexture(sprite_heli);
	UnloadTexture(sprite_posto);
	UnloadTexture(sprite_jogDir);
	UnloadTexture(sprite_jogEsq);

	CloseWindow(); // Fecha a janela
	return 0;
}
