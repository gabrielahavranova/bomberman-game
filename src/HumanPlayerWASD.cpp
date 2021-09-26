//
// Created by gabi on 05/05/2020.
//

#include "./include/HumanPlayerWASD.h"

HumanPlayerWASD::HumanPlayerWASD(): Player() {
    pos_coords2D.x = WINDOW_WIDTH - 2 * TILE_DIM;
    pos_coords2D.y = WINDOW_HEIGHT - 2 * TILE_DIM;

    player_texture = TextureManager::loadTexture("src/assets/cat2.png");
    dead_player_texture = TextureManager::loadTexture("src/assets/dead_cat.png");
    TextureManager::textures.push_back(player_texture);
    TextureManager::textures.push_back(dead_player_texture);

    player_name = "player2";
    name_dest_rect = {20, 550, 200, 50};
    score_dest_rect = {220, 550, 30, 50};
    setNameTexture();
}

void HumanPlayerWASD::move(const SDL_Keycode & action, Map & map) {
    int diff = TILE_DIM / 3;
    Vector2D my_tile = getTile2D(pos_coords2D.x, pos_coords2D.y);
    switch (action) {
        case SDLK_w:
            if (pos_coords2D.y > (TILE_DIM + 1) && (pos_coords2D.x % TILE_DIM < diff || pos_coords2D.x % TILE_DIM > TILE_DIM - diff) ) {
                if (((pos_coords2D.y - speed) % TILE_DIM >= TILE_DIM - speed) && !map.isSteppable(my_tile.x, my_tile.y - 1)) break;
                pos_coords2D.y -= speed;
                if (pos_coords2D.x % TILE_DIM < diff) pos_coords2D.x -= pos_coords2D.x % TILE_DIM;
                else pos_coords2D.x += TILE_DIM - pos_coords2D.x % TILE_DIM;
            }
            break;

        case SDLK_d:
            if (pos_coords2D.x < (WINDOW_WIDTH - 2 * TILE_DIM) &&
                (pos_coords2D.y % TILE_DIM < diff || pos_coords2D.y % TILE_DIM > TILE_DIM - diff)) {

                if (((pos_coords2D.x + speed) % TILE_DIM <= speed) && !map.isSteppable(my_tile.x + 1, my_tile.y)) break;

                pos_coords2D.x += speed;
                if (pos_coords2D.y % TILE_DIM < diff) pos_coords2D.y -= pos_coords2D.y % TILE_DIM;
                else pos_coords2D.y += TILE_DIM - pos_coords2D.y % TILE_DIM;
            }
            break;

        case SDLK_s:
            if (pos_coords2D.y < (WINDOW_HEIGHT - 2 * TILE_DIM) &&
                (pos_coords2D.x % TILE_DIM < diff || pos_coords2D.x % TILE_DIM > TILE_DIM - diff)) {
                if (((pos_coords2D.y + speed) % TILE_DIM <= speed) && !map.isSteppable(my_tile.x, my_tile.y + 1)) break;

                pos_coords2D.y += speed;
                if (pos_coords2D.x % TILE_DIM < diff) pos_coords2D.x -= pos_coords2D.x % TILE_DIM;
                else pos_coords2D.x += TILE_DIM - pos_coords2D.x % TILE_DIM;
            }
            break;

        case SDLK_a:
            if (pos_coords2D.x > (TILE_DIM + 1) && (pos_coords2D.y % TILE_DIM < diff || pos_coords2D.y % TILE_DIM > TILE_DIM - diff)) {
                if (((pos_coords2D.x - speed) % TILE_DIM >= TILE_DIM - speed) && !map.isSteppable(my_tile.x - 1, my_tile.y)) break;
                pos_coords2D.x -= speed;
                if (pos_coords2D.y % TILE_DIM < diff) pos_coords2D.y -= pos_coords2D.y % TILE_DIM;
                else pos_coords2D.y += TILE_DIM - pos_coords2D.y % TILE_DIM;
            }
            break;
        default:
            break;

    }
    claimBoost(map.isBooster(my_tile.x, my_tile.y));
}

