#ifndef MAP_H
#define MAP_H

#include "raylib.h"

#define VEL_CENARIO 2.0f
#define TAMANHO_BLOCO 30
#define QTD_BLOCOS 65

typedef struct Map {
    float rolagem; // pos vertical do mapa
} Map;

void inicializaMapa(Map* mapa);
void atualizaMapa(Map* mapa);
void desenhaMapa(Map* mapa);

#endif 
