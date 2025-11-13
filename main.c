#include "Funcoes.h"
#include "raylib.h"
#include <stdlib.h>
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

	bool jogoAtivo = false;
	bool sair = false;
	int op = 0; // Operador do menu

	InitWindow(LARGURA, ALTURA, "Teclas"); //Inicializa janela, com certo tamanho e titulo
	SetTargetFPS(60);// Ajusta a janela para 60 frames por segundo
	inicializaTiros(tiros); // Inicializa tiros como inativos
	spawnaInimigos(inimigos, teste_1); // Inicializa inimigos


	while (!WindowShouldClose() && !sair) // Enquanto o ESC e o botão de sair nao forem pressionados
	{
		if (!jogoAtivo) // Se o jogo nao comecou
		{
			if ((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && op != 0)
			{
				op--; // Move o cursor para cima caso nao esteja no topo
			}
			if ((IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) && op != 2)
			{
				op++; // Move o cursor para baixa caso nao esteja no fundo
			}
			if (IsKeyPressed(KEY_ENTER) && op == 0)
			{
				jogoAtivo = true; // Começa o jogo caso ENTER tenha sido pressioando, e caso cursor esteja em Novo Jogo
			}
			if (IsKeyPressed(KEY_ENTER) && op == 2)
			{
				sair = true; // Sai caso ENTER tenha sido pressionado, e caso cursor esteja em Sair
			}


			BeginDrawing();
			ClearBackground(SKYBLUE);
			menu(op); // Desenha o menu
			EndDrawing();
		}
		else
		{
			if (jogador.vidas != 0) // Caso jogo tenha começado
			{
				atualizaTiros(tiros); // Atualiza posição e estado dos tiros a cada iteração
				atualizaInimigos(inimigos); // Atualiza posição e estado dos inimigos a cada iteração
				checaColisoesTiro(tiros, inimigos); // Checa colisões entre tiros e inimigos
				checaColisoesJogador(&jogador, inimigos); // Checa colisões entre jogador e inimigos


				if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) // Movimento para a direita
				{
					jogador.x += VEL_JOGADOR;
					if (jogador.x > LARGURA - jogador.largura)
					{
						jogador.x = LARGURA - jogador.largura;
					}
				}
				if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) // Movimento para a esquerda
				{
					jogador.x -= VEL_JOGADOR;
					if (jogador.x < 0)
					{
						jogador.x = 0;
					}
				}

				if (IsKeyPressed(KEY_SPACE)) // Atira se barra de espaço for pressionada
				{
					atira(tiros, jogador);
				}

				
				BeginDrawing(); 
				ClearBackground(RAYWHITE); 
				DrawRectangle(jogador.x, jogador.y, jogador.largura, jogador.altura, BLUE); //Desenha o jogador na tela
				desenhaTiros(tiros); // Desenha o tiro na tela
				desenhaInimigos(inimigos); // Desenha os inimigos na tela
				EndDrawing(); 
			}


			else
			{
				BeginDrawing();
				ClearBackground(BLACK);
				DrawText("TU MORREU", LARGURA / 4, ALTURA / 2, 50, RED); // Desenha tela de morte
				EndDrawing();
			}
					 		
		}
	}

	CloseWindow(); // Fecha a janela
	return 0;
}
