#define _CRT_SECURE_NO_WARNINGS

#include "Funcoes.h"
#include "raylib.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Spawnpoint teste_1[MAX_INIMIGOS] =
{
  {100, -50},
  {200, -100},
  {300, -50},
  {400, -100},
  {500, -50}
};


int main() {
	Jogador jogador = { LARGURA / 2, ALTURA / 2 , LARG_JOGADOR, LARG_JOGADOR, 1 }; //Cria jogador
	Tiro tiros[MAX_TIROS]; //Cria array de tiros
	Inimigo inimigos[MAX_INIMIGOS];

	EstadoJogo Estado = EST_MENU; // Define estado do jogo inicialmenmte como menu

	bool jogoTerminado = false;
	bool inserirRanking = false; // Variavel verifica se o jogador pode ser inserido no ranking

	bool sair = false;
	MenuOp M_op = 0; // Operador do menu principal

	float intervalo = 0.5f; 
	float tempoDecorrido = 0.0f;
	float deltaTempo;
	bool mostrarTexto = true;

	int larguraTexto = 0; // Variavel utilizada para centralizar texto

	int letras = 0; // Letras do nome inserido
	char nomeJogador[MAX_NOME + 1] = { '\0' };

	Rectangle caixaTexto = { (LARGURA - 300) / 2, ALTURA / 2 , 300, 100 }; // Delimita caixa de texto do nome

	int pontuacao = 0;
	PontosJogador ranking[MAX_RANKING]; // Array do ranking

	InitWindow(LARGURA, ALTURA, "River-Inf"); //Inicializa janela
	SetTargetFPS(60);// Ajusta a janela para 60 frames por segundo
	inicializaTiros(tiros); // Inicializa tiros como inativos
	spawnaInimigos(inimigos, teste_1); // Inicializa inimigos


	copiaRanking(ranking); // Carrega ranking de arquivo
	printaRanking(ranking); // Imprime ranking no console

	while (!WindowShouldClose() && !sair) // Enquanto o ESC e o botão de sair nao forem pressionados
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
					Estado = EST_JOGO; // Começa o jogo caso cursor esteja em Novo Jogo
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
			atualizaTiros(tiros); // Atualiza posição e estado dos tiros a cada iteração
			atualizaInimigos(inimigos); // Atualiza posição e estado dos inimigos a cada iteração
			checaColisoesTiro(tiros, inimigos, &pontuacao); // Checa colisões entre tiros e inimigos
			checaColisoesJogador(&jogador, inimigos); // Checa colisões entre jogador e inimigos


			if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) // Movimento para a direita
			{
				jogador.x += VEL_JOGADOR;
				if (jogador.x > LARGURA - jogador.largura)
				{
					jogador.x = LARGURA - jogador.largura;
				}
			}
			else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) // Movimento para a esquerda
			{
				jogador.x -= VEL_JOGADOR;
				if (jogador.x < 0)
				{
					jogador.x = 0;
				}
			}

			if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_K)) // Se barra de espaço ou K for pressionado
			{
				atira(tiros, jogador); // Atira
			}

			if (jogador.vidas == 0) // Caso jogador fique sem vidas
			{
				Estado = EST_MORTE; // Muda estado para Morte
			}


			BeginDrawing();
			ClearBackground(RAYWHITE);
			DrawRectangle(jogador.x, jogador.y, jogador.largura, jogador.altura, BLUE); //Desenha o jogador na tela
			desenhaTiros(tiros); // Desenha o tiro na tela
			desenhaInimigos(inimigos); // Desenha os inimigos na tela
			desenhaInfo(pontuacao); // Desenha a pontuação na tela
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
						resetaJogo(&jogador, tiros, inimigos, teste_1, &pontuacao, &letras, nomeJogador);
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
					if ((tecla >= 32) && (tecla <= 125)) // Verifica se o caractere é imprimível
					{
						nomeJogador[letras] = toupper((char)tecla); // Adiciona caractere ao nome do jogador
						letras++;
						nomeJogador[letras] = '\0'; // Adiciona o terminador de string
					}
					tecla = GetCharPressed(); // Pega o próximo caractere pressionado
				}

				if (IsKeyPressed(KEY_BACKSPACE)) // Permite apagar o último caractere
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
					resetaJogo(&jogador, tiros, inimigos, teste_1, &pontuacao, &letras, nomeJogador);
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
	CloseWindow(); // Fecha a janela
	return 0;
}
