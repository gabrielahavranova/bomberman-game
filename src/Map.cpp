#include "./include/Map.h"

char default_map[ROWS_NBR][COLS_NBR] = {
        {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'},
        {'#','.','.','@','.','@','@','.','@','.','@','@','@','.','.','#'},
        {'#','.','#','.','#','.','#','@','@','#','.','#','@','#','.','#'},
        {'#','@','@','@','@','@','@','.','@','@','@','.','@','@','@','#'},
        {'#','.','#','@','#','@','#','.','.','#','@','#','.','#','@','#'},
        {'#','@','@','.','@','@','@','@','.','@','@','.','@','@','@','#'},
        {'#','@','@','@','@','@','.','@','.','@','@','@','@','@','@','#'},
        {'#','.','#','@','#','@','#','@','@','#','@','#','@','#','.','#'},
        {'#','@','@','@','.','@','@','@','.','@','@','.','@','@','@','#'},
        {'#','.','#','.','#','@','#','@','@','#','@','#','.','#','.','#'},
        {'#','.','.','@','@','.','@','@','.','@','@','.','@','.','.','#'},
        {'#','#','#','#','#','#','#','#','#','#','#','#','#','#','#','#'}
};

void Map::loadTextures() {
    base_brick = TextureManager::loadTexture("src/assets/base_brick.png");
    sand = TextureManager::loadTexture("src/assets/base_sand.png");
    bomb = TextureManager::loadTexture("src/assets/bomb.png");
    wood = TextureManager::loadTexture("src/assets/wood.png");
    booster = TextureManager::loadTexture("src/assets/booster.png");

    expl_horizontal_texture = TextureManager::loadTexture("src/assets/single_beam_horizontal.png");
    expl_vertical_texture = TextureManager::loadTexture("src/assets/single_beam_vertical.png");
    expl_cross_texture = TextureManager::loadTexture("src/assets/cross_beam.png");


    texture_dict.emplace('#', base_brick);
    texture_dict.emplace('.', sand);
    texture_dict.emplace('x', bomb);
    texture_dict.emplace('@', wood);
    texture_dict.emplace('s', booster);
    texture_dict.emplace('b', booster);
    texture_dict.emplace('c', booster);
    texture_dict.emplace('m', booster);
}

void Map::loadMap (char arr[ROWS_NBR][COLS_NBR]) {
    for (auto row = 0; row < ROWS_NBR; row++) {
        for (auto column = 0; column < COLS_NBR; column++) {
            map[row][column] = arr[row][column];
        }
    }
}

Map::Map(int map_id) {
    loadTextures();
    int res = FileManager::loadMap(map_id, *this);
    if (res != 0) {
        loadMap(default_map);
    }
    std::cout << "map sucessfully loaded" << std::endl;
}

void Map::drawMap () {
    char type;

    for (auto row = 0; row < ROWS_NBR; row++) {
        for (auto column = 0; column < COLS_NBR; column++) {
            type = map[row][column];

            src_rect.x = src_rect.y = 0;
            src_rect.w = src_rect.h = TILE_DIM;

            dest_rect.x = column * TILE_DIM;
            dest_rect.y = row * TILE_DIM;
            dest_rect.w = TILE_DIM;
            dest_rect.h = TILE_DIM;

            if (texture_dict[type] == sand || texture_dict[type] == wood || texture_dict[type] == base_brick) {
                TextureManager::draw(texture_dict[type], src_rect, dest_rect);
            } else {
            TextureManager::draw(sand, src_rect, dest_rect);
            TextureManager::draw(texture_dict[type], src_rect, dest_rect);
            }
        }
    }
    for (auto & expl : explosions) expl.draw();
}

bool Map::isSteppable (const int & col, const int & row) const {
    if (map[row][col] == '.' || map[row][col] == 's' ||
        map[row][col] == 'b' || map[row][col] == 'c' || map[row][col] == 'm') return true;
    return false;
}

void Map::addBomb (const int &col, const int &row, Bomb & bomb) {
    map[col][row] = 'x';
    planted_bombs.emplace(bomb);
}

void Map::bombExplosion(const Bomb & bomb) {
    explosions.emplace_back(Explosion(bomb.bomb_size, bomb.col, bomb.row, SDL_GetTicks() + 700, this));
}

void Map::update(std::map <std::string, Player *> & players) {
    if (!planted_bombs.empty() && planted_bombs.front().getExplosionTime() <= SDL_GetTicks()) {
        bombExplosion(planted_bombs.front());
        planted_bombs.pop();
    }

    for (auto & expl : explosions) {
        expl.checkPlayers(players);

        if (expl.end_time - 50 <= SDL_GetTicks()) {
            expl.destroyObjects();
            map[explosions.front().start_pos.y][explosions.front().start_pos.x] = '.';

            explosions.pop_front();
        }
    }
}

bool Map::isWood(const int & col, const int & row) {
    if (map[row][col] == '@') return true;
    return false;
}

void Map::destroyObject (const int & col, const int & row) {
    map[row][col] = '.';
}


void Map::generateBooster (const int & col, const int & row) {
    int random = rand() % 120;

    if (random > 30) return; //nothing to be generated - 3/4 probability
    else if (random <= 5) {
        map[row][col] = 's';
    }
    else if (random <= 10) {
        map[row][col] = 'b';
    }
    else if (random <= 15) {
        map[row][col] = 'c';
    }
    else {
        map[row][col] = 'm';
    }

}

char Map::isBooster (const int & col, const int & row) {
    if (map[row][col] == 's' || map[row][col] == 'b' || map[row][col] == 'c' || map[row][col] == 'm') {
        char res = map[row][col];
        map[row][col] = '.';
        return res;
    }
    return '!';
}

Map::~Map() {
    if (sand != nullptr) SDL_DestroyTexture(sand);
    if (expl_horizontal_texture != nullptr) SDL_DestroyTexture(expl_horizontal_texture);
    if (expl_vertical_texture != nullptr) SDL_DestroyTexture(expl_vertical_texture);
    if (expl_cross_texture != nullptr) SDL_DestroyTexture(expl_cross_texture);
    if (base_brick != nullptr) SDL_DestroyTexture(base_brick);
    if (bomb != nullptr) SDL_DestroyTexture(bomb);
    if (wood != nullptr) SDL_DestroyTexture(wood);
    if (booster != nullptr) SDL_DestroyTexture(booster);
}


Bomb::Bomb(const int & col, const int & row, const int & explosion_size, Map & map):col(col), row(row), bomb_size(explosion_size) {
    explosion_time = SDL_GetTicks() + 3000;
    map.addBomb(col, row, *this);
}

