#include "./include/HumanPlayerArrows.h"


HumanPlayerArrows::HumanPlayerArrows(): Player() {
    pos_coords2D.x = TILE_DIM;
    pos_coords2D.y = TILE_DIM;

    player_texture = TextureManager::loadTexture("src/assets/cat1.png");
    dead_player_texture = TextureManager::loadTexture("src/assets/dead_cat.png");
    TextureManager::textures.push_back(player_texture);
    TextureManager::textures.push_back(dead_player_texture);

    player_name = "Player1";
    name_dest_rect = {20, 0, 200, 50};
    score_dest_rect = {220, 0, 30, 50};
    setNameTexture();
}


void HumanPlayerArrows::move(const SDL_Keycode & action, Map & map) {
    int diff = 15;
    Vector2D my_tile = getTile2D(pos_coords2D.x, pos_coords2D.y);
    switch (action) {
        case SDLK_UP:
            if (pos_coords2D.y > 51 && (pos_coords2D.x % 50 < diff || pos_coords2D.x % 50 > 50 - diff) ) {
                if (((pos_coords2D.y - speed) % 50 >= 50 - speed) && !map.isSteppable(my_tile.x, my_tile.y - 1)) break;
                pos_coords2D.y -= speed;
                if (pos_coords2D.x % 50 < diff) pos_coords2D.x -= pos_coords2D.x % 50;
                else pos_coords2D.x += 50 - pos_coords2D.x % 50;
            }
            break;

        case SDLK_RIGHT:
            if (pos_coords2D.x < 700 && (pos_coords2D.y % 50 < diff || pos_coords2D.y % 50 > 50 - diff)) {

                if (((pos_coords2D.x + speed) % 50 <= speed) && !map.isSteppable(my_tile.x + 1, my_tile.y)) break;

                pos_coords2D.x += speed;
                if (pos_coords2D.y % 50 < diff) pos_coords2D.y -= pos_coords2D.y % 50;
                else pos_coords2D.y += 50 - pos_coords2D.y % 50;
            }
            break;

        case SDLK_DOWN:
            if (pos_coords2D.y < 500 && (pos_coords2D.x % 50 < diff || pos_coords2D.x % 50 > 50 - diff)) {
                if (((pos_coords2D.y + speed) % 50 <= speed) && !map.isSteppable(my_tile.x, my_tile.y + 1)) break;

                pos_coords2D.y += speed;
                if (pos_coords2D.x % 50 < diff) pos_coords2D.x -= pos_coords2D.x % 50;
                else pos_coords2D.x += 50 - pos_coords2D.x % 50;
            }
            break;

        case SDLK_LEFT:
            if (pos_coords2D.x > 51 && (pos_coords2D.y % 50 < diff || pos_coords2D.y % 50 > 50 - diff)) {
                if (((pos_coords2D.x - speed) % 50 >= 50 - speed) && !map.isSteppable(my_tile.x - 1, my_tile.y)) break;
                pos_coords2D.x -= speed;
                if (pos_coords2D.y % 50 < diff) pos_coords2D.y -= pos_coords2D.y % 50;
                else pos_coords2D.y += 50 - pos_coords2D.y % 50;
            }
            break;
        default:
            break;
    }
    claimBoost(map.isBooster(my_tile.x, my_tile.y));
}


