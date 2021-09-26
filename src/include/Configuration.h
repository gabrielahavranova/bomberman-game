#pragma once
#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const SDL_Rect DEF_SRC_RECT {0, 0, 800, 600};
const SDL_Rect DEF_DEST_RECT {0, 0, 800, 600};
const SDL_Rect TEXT_SRC_RECT {0, 0, 800, 800};
const int TILE_DIM = 50;
const int ROWS_NBR = WINDOW_HEIGHT / TILE_DIM;
const int COLS_NBR = WINDOW_WIDTH / TILE_DIM;
const int FPS = 60;
const int MAX_BOMB_SIZE = 4;
const int MAX_BOMB_CNT = 4;
const int MAX_SPEED = 4;
const int DIFF = 15;
const bool FULLSCREEN = false;


