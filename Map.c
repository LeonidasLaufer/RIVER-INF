#include "Map.h"
#include "Funcoes.h"
#include "raylib.h"

int layoutRio[QTD_BLOCOS] = {
	// Spawn e Safezone
	250, 250, 250, 250, 250, 50, 50, 50, 50, 50,
	250, 250, 250, 250, 250,
	// Inicio do gameplay
	60, 70, 80, 90, 100, 120, 140, 160, 180, 200,
	50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
	220, 220, 220, 220, 220, 220, 220, 220, 220, 220,
	250, 250, 250, 250, 250, 50, 50, 50, 50, 50,
	20, 20, 20, 20, 20, 20, 20, 20, 20, 20
};

int layoutIlhas[QTD_BLOCOS] = {
	// Spawn limpo
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	// Obstaculos
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 60, 60, 60, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 30, 30, 30, 30, 30, 0, 0, 0
};

void inicializaMapa(Map* mapa) {
	mapa->rolagem = 0.0f;
}

void atualizaMapa(Map* mapa) {
	mapa->rolagem += VEL_CENARIO;

	int alturaTotalMapa = QTD_BLOCOS * TAMANHO_BLOCO;

	if (mapa->rolagem >= alturaTotalMapa) {
		mapa->rolagem = 0.0f; // Reseta a rolagem quando atinge o fim da tela
	}
}

void DesenhaIlha(int larguraIlha, int y, Color cor) {
	if (larguraIlha > 0) {
		// ccentralizar a ilha na tela
		int xIlha = (LARGURA / 2) - (larguraIlha / 2);

		DrawRectangle(xIlha, y, larguraIlha, TAMANHO_BLOCO, cor);
	}
}

void desenhaMapa(Map* mapa) {
	Color corTerreno = DARKGREEN;
	int alturaTotalMapa = QTD_BLOCOS * TAMANHO_BLOCO;
	int deslocamento = alturaTotalMapa - ALTURA;


	for (int i = 0; i < QTD_BLOCOS; i++) {

		int yInvertido = (QTD_BLOCOS - 1 - i) * TAMANHO_BLOCO;
		int yAtual = yInvertido + (int)mapa->rolagem - deslocamento;


		int yLoop = yAtual - alturaTotalMapa;

		int larguraMargem = layoutRio[i];
		int larguraIlha = layoutIlhas[i];

		// Margens no inicio
		DrawRectangle(0, yAtual, larguraMargem, TAMANHO_BLOCO, corTerreno);
		DrawRectangle(LARGURA - larguraMargem, yAtual, larguraMargem, TAMANHO_BLOCO, corTerreno);

		// Margens pra ficar o looping
		DrawRectangle(0, yLoop, larguraMargem, TAMANHO_BLOCO, corTerreno);
		DrawRectangle(LARGURA - larguraMargem, yLoop, larguraMargem, TAMANHO_BLOCO, corTerreno);

		// Ilhas 
		DesenhaIlha(larguraIlha, yAtual, corTerreno);
		DesenhaIlha(larguraIlha, yLoop, corTerreno);

		// CONTROLE PARA CRIAR O MAPA (se não quiser que os numeros apareçam, comentar a função abaixo)
		DrawText(TextFormat("%d", i), 10, yAtual + 5, 20, RED);
	}
}
