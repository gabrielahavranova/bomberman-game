#include "./include/Explosion.h"

Explosion::Explosion (const int &size, const int &bomb_x_pos, const int &bomb_y_pos,
        const uint32_t &end_time, Map * map): start_pos(bomb_y_pos, bomb_x_pos),
        end_time(end_time), size(size),  map(map) {

    for (int i = start_pos.x; i <= start_pos.x + size; i++) {
        x_end_pos = i;
        if (i > 13 || !map->isSteppable(i + 1, start_pos.y)) break;
    }

    for (int i = start_pos.x; i >= start_pos.x - size; i--) {
        x_start_pos = i;
        if (i < 2 || !map->isSteppable(i - 1, start_pos.y)) break;
    }

    for (int i = start_pos.y; i <= start_pos.y + size; i++) {
        y_end_pos = i;
        if (i > 9 || !map->isSteppable(start_pos.x, i + 1)) break;
    }

    for (int i = start_pos.y; i >= start_pos.y - size; i--) {
        y_start_pos = i;
        if (i < 2 || !map->isSteppable(start_pos.x, i - 1)) break;
    }
}

void Explosion::draw() const {
    SDL_Rect src_rect, dest_rect;

    src_rect.x = src_rect.y = 0;
    src_rect.w = src_rect.h = TILE_DIM;
    dest_rect.w = dest_rect.h = TILE_DIM;

    int i = x_start_pos;
    dest_rect.x = x_start_pos * TILE_DIM;
    dest_rect.y = start_pos.y * TILE_DIM;

    while (i <= x_end_pos) {
        TextureManager::draw(map->expl_vertical_texture, src_rect, dest_rect);
        i++;
        dest_rect.x = i * 50;
    }

    i = y_start_pos;
    dest_rect.x = start_pos.x * 50;
    dest_rect.y = y_start_pos * 50;

    while (i <= y_end_pos) {
        TextureManager::draw(map->expl_horizontal_texture, src_rect, dest_rect);
        i++;
        dest_rect.y = i * 50;
    }

    dest_rect.x = start_pos.x * 50;
    dest_rect.y = start_pos.y * 50;
    TextureManager::draw(map->expl_cross_texture, src_rect, dest_rect);
}

void Explosion::checkPlayers(std::map <std::string, Player *> & players) {
    for (auto & pl: players) {
        Player * player = pl.second;

        //check if player is completely out of explosion rectangular range
        
        if (player->getCoords2D().x + 47 <= x_start_pos * TILE_DIM || player->getCoords2D().x >= x_end_pos * TILE_DIM + 47) continue;
        if (player->getCoords2D().y + 47 <= y_start_pos * TILE_DIM || player->getCoords2D().y >= y_end_pos * TILE_DIM + 47) continue;

        //player is at left upper sector
        if (player->getCoords2D().x + 47 <= start_pos.x * TILE_DIM && player->getCoords2D().y + 47 <= start_pos.y * TILE_DIM)
            continue;

        //player is at right upper sector
        if (player->getCoords2D().x >= start_pos.x * TILE_DIM + 47 && player->getCoords2D().y + 47 <= start_pos.y * TILE_DIM)
            continue;

        //left lower sector
        if (player->getCoords2D().x + 47 <= start_pos.x * TILE_DIM && player->getCoords2D().y >= start_pos.y * TILE_DIM + 47)
            continue;

        //right lower sector
        if (player->getCoords2D().x >= start_pos.x * TILE_DIM + 47 && player->getCoords2D().y >= start_pos.y * TILE_DIM + 47)
            continue;

        //if any of the conditions was not fulfilled - player was hit by explosion
        player->death();
    }
}

void Explosion::destroyObjects() const {
    for (int i = start_pos.x; (i < start_pos.x + size) && i < 15; i++) {
        if (map->isWood(i + 1, start_pos.y)) {
            map->destroyObject(i + 1, start_pos.y);
            map->generateBooster(i + 1, start_pos.y);
            break;
        }
        else if (!map->isSteppable(i + 1, start_pos.y)) break;
    }

    for (int i = start_pos.x; (i > start_pos.x - size) && i > 1; i--) {
        if (map->isWood(i - 1, start_pos.y)) {
            map->destroyObject(i - 1, start_pos.y);
            map->generateBooster(i - 1, start_pos.y);
            break;
        }
        else if (!map->isSteppable(i - 1, start_pos.y)) break;
    }

    for (int i = start_pos.y; (i < start_pos.y + size) && i < 11; i++) {

        if (map->isWood(start_pos.x, i + 1)) {
            map->destroyObject(start_pos.x, i + 1);
            map->generateBooster(start_pos.x, i + 1);
            break;
        }
        else if (!map->isSteppable(start_pos.x, i + 1)) break;
    }

    for (int i = start_pos.y; (i > start_pos.y - size) && i > 1; i--) {
        if (map->isWood(start_pos.x, i - 1)) {
            map->destroyObject(start_pos.x, i - 1);
            map->generateBooster(start_pos.x, i - 1);
            break;
        }
        else if (!map->isSteppable(start_pos.x, i - 1)) break;
    }
}
