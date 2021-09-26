#include "./include/ComputerPlayer.h"


ComputerPlayer::ComputerPlayer(int start_pos_x, int start_pos_y, const std::string & name,
        int name_pos_x, int name_pos_y, const char * kitty_src, const char * dead_kitty_src) {
    pos_coords2D = { start_pos_x, start_pos_y };

    player_texture = TextureManager::loadTexture(kitty_src);
    dead_player_texture = TextureManager::loadTexture(dead_kitty_src);
    TextureManager::textures.push_back(player_texture);
    TextureManager::textures.push_back(dead_player_texture);

    player_name = name;
    name_dest_rect = {name_pos_x, name_pos_y, 200, 50};
    score_dest_rect = {name_pos_x + 220,name_pos_y, 30, 50};
    my_tile = getTile2D(pos_coords2D.x, pos_coords2D.y);
    setNameTexture();
}

void ComputerPlayer::move(const SDL_Keycode &, Map & map) {
    if (state != RANDOM && SDL_GetTicks() % 1000 == 0) {
        if (!checkBoxesLeft(map)) state = RANDOM;
    }
    my_tile = getTile2D(pos_coords2D.x, pos_coords2D.y);
    getCurrentMap(map);

    claimBoost(map.isBooster(my_tile.x, my_tile.y));

    int tmp;
    if (!path.empty()) {
        moveTowards(path.front());
    }
    else if (!safe(my_tile.x, my_tile.y, tmp)) {
        if (!places_to_bomb.empty()) std::queue <Vector2D> ().swap(places_to_bomb); // clears the queue
        hide();
    }
    else if (!places_to_bomb.empty()) {
        plantBomb(map);
        places_to_bomb.pop();
    }
    else {
        if (rand() % 80 == 0) {
            if (state == RANDOM) {
                int x = rand() % (COLS_NBR - 2) + 1;
                int y = rand() % (ROWS_NBR - 2) + 1;
                if (isSteppable(x, y)) {
                    my_map[y][x] = '@';
                };
            }
            Vector2D dest = findSafeToBombTile(my_tile);
            if (dest == my_tile) places_to_bomb.emplace(my_tile);
            else if (dest.x != 0 && dest.y != 0) {
                getPath(dest);
                places_to_bomb.emplace(dest);
            }
        }
    }
}

void ComputerPlayer::hide() {
    Vector2D safety = findNearestSafeTile(my_tile);

    if (safety.x == 0 && safety.y == 0 ) {
        safety = findLeastDangerousTile(my_tile);
    }
    getPath(safety);
}

bool ComputerPlayer::safe(int x, int y, int & distance_to_nearest_bomb) {
    int min_dist = MAX_BOMB_SIZE;

    for (int col = x; col < x + MAX_BOMB_SIZE; col++) {
        if (isBomb(col, y)) {
            min_dist = (min_dist > (col - x) ? (col - x) : min_dist);
            break;
        }
        else if (!isSteppable(col, y) || col >= COLS_NBR - 1) break;
    }

    for (int col = x; col > x - MAX_BOMB_SIZE; col--) {
        if (isBomb(col, y)) {
            min_dist = (min_dist > (x - col) ? (x - col) : min_dist);
            break;
        }
        else if (!isSteppable(col, y) || col <= 1) break;
    }

    for (int row = y; row < y + MAX_BOMB_SIZE; row++) {
        if (isBomb(x, row)) {
            min_dist = (min_dist > (row - y) ? (row - y) : min_dist);
            break;
        }
        else if (!isSteppable(x, row) || row >= ROWS_NBR - 1) break;
    }

    for (int row = y; row > y - MAX_BOMB_SIZE; row--) {
        if (isBomb(x, row)) {
            min_dist = (min_dist > (y - row) ? (y - row) : min_dist);
            break;
        }
        if (!isSteppable(x, row) || row <= 1) break;
    }

    if (min_dist >= MAX_BOMB_SIZE) return true;
    distance_to_nearest_bomb = min_dist;
    return false;
}

Vector2D ComputerPlayer::findNearestSafeTile (const Vector2D & start_pos) {
    std::queue <Vector2D> tile_queue;
    uint8_t index = 2;
    int tmp = 0;
    tile_queue.emplace(start_pos);

    Vector2D current;
    if (!isBomb(start_pos.x, start_pos.y)) my_map[start_pos.y][start_pos.x] = 1;
    while (!tile_queue.empty()) {
        current = tile_queue.front();

        if (isSteppable(current.x + 1, current.y) && notVisited(current.x + 1, current.y)) {
            my_map[current.y][current.x +1] = index;
            if (safe(current.x + 1, current.y, tmp)) return Vector2D{current.x + 1, current.y};
            else tile_queue.emplace(Vector2D{current.x + 1, current.y});
        }

        if (isSteppable(current.x - 1, current.y) && notVisited(current.x - 1, current.y)) {
            my_map[current.y][current.x-1] = index;
            if (safe(current.x - 1, current.y, tmp)) return Vector2D{current.x - 1, current.y};
            else tile_queue.emplace(Vector2D{current.x - 1, current.y});
        }
        if (isSteppable(current.x, current.y + 1) && notVisited(current.x, current.y + 1)) {
            my_map[current.y+1][current.x] = index;
            if (safe(current.x, current.y + 1, tmp)) return Vector2D{current.x, current.y + 1};
            else tile_queue.emplace(Vector2D{current.x, current.y + 1});
        }

        if (isSteppable(current.x, current.y - 1) && notVisited(current.x, current.y - 1)) {
            my_map[current.y-1][current.x] = index;
            if (safe(current.x, current.y - 1, tmp)) return Vector2D{current.x, current.y - 1};
            else tile_queue.emplace(Vector2D{current.x, current.y - 1});
        }

        index++;
        tile_queue.pop();

    }
    return Vector2D {0,0};
}


void ComputerPlayer::getCurrentMap(const Map & map) {
    for (auto row = 0; row < ROWS_NBR; row++) {
        for (auto column = 0; column < COLS_NBR; column++) {
            if (map.isSteppable(column, row)) my_map[row][column] = 0;
            else my_map[row][column] = map.map[row][column];
        }
    }
}

void ComputerPlayer::getPath(const Vector2D & dest_tile) {
    Vector2D current = dest_tile;
    my_map[my_tile.y][my_tile.x] = 1;

    uint8_t count_down = my_map[dest_tile.y][dest_tile.x];
    count_down--;

    while (my_map[current.y][current.x] != 1) {
        path.emplace_front(current);
        if (my_map[current.y][current.x+1] == count_down) {
            current = Vector2D{current.x + 1, current.y};
        }
        else if (my_map[current.y][current.x-1] == count_down) {
            current = Vector2D{current.x - 1, current.y};
        }
        else if (my_map[current.y+1][current.x] == count_down) {
            current = Vector2D{current.x, current.y + 1};
        }
        else if (my_map[current.y-1][current.x] == count_down) {
            current = Vector2D{current.x, current.y - 1};
        }
        count_down--;
    }
}

void ComputerPlayer::moveTowards(const Vector2D &next_tile) {
    if (isBomb(next_tile.x, next_tile.y)) {
        path.clear();
        std::queue <Vector2D> ().swap(places_to_bomb);
    }
    if (pos_coords2D.x == next_tile.x * TILE_DIM) {
        if (pos_coords2D.y < next_tile.y * TILE_DIM) moveDown(next_tile);
        else moveUp(next_tile);
    } else {
        if (pos_coords2D.x < next_tile.x * TILE_DIM) moveRight(next_tile);
        else moveLeft(next_tile);
    }

    if (posAlmostEqual(next_tile)) {
        path.pop_front();
    }
}

void ComputerPlayer::moveDown(const Vector2D & next_tile) {
    if (pos_coords2D.y > WINDOW_HEIGHT - 2*TILE_DIM - speed) {
        pos_coords2D.y = WINDOW_HEIGHT - 2*TILE_DIM;
        return;
    }

    if (pos_coords2D.x < next_tile.x * TILE_DIM - DIFF) moveRight(Vector2D{my_tile.x + 1, my_tile.y});
    else if (pos_coords2D.x > next_tile.x * TILE_DIM + DIFF) moveLeft(Vector2D{my_tile.x - 1, my_tile.y});
    else {
        if (((pos_coords2D.y + speed) % 50 <= speed) && !isSteppable(my_tile.x, my_tile.y + 1)) return;//        pos_coords2D.y += 50 - pos_coords2D.y ;

            pos_coords2D.y += speed;
            if (pos_coords2D.x % 50 < DIFF) pos_coords2D.x -= pos_coords2D.x % 50;
            else pos_coords2D.x += 50 - pos_coords2D.x % 50;
    }
}

void ComputerPlayer::moveUp(const Vector2D & next_tile) {
    if (pos_coords2D.y < TILE_DIM + speed) {
        pos_coords2D.y = TILE_DIM;
        return;
    }

    if (pos_coords2D.x < next_tile.x * TILE_DIM - DIFF) moveRight(Vector2D{my_tile.x + 1, my_tile.y});
    else if (pos_coords2D.x > next_tile.x * TILE_DIM + DIFF) moveLeft(Vector2D{my_tile.x - 1, my_tile.y});
    else {
        if (((pos_coords2D.y - speed) % TILE_DIM >= TILE_DIM - speed) && !isSteppable(my_tile.x, my_tile.y - 1)) return;//todo round up;

        pos_coords2D.y -= speed;
        if (pos_coords2D.x % TILE_DIM < DIFF) pos_coords2D.x -= pos_coords2D.x % 50;
        else pos_coords2D.x += TILE_DIM - pos_coords2D.x % TILE_DIM;
    }
}

void ComputerPlayer::moveLeft(const Vector2D & next_tile) {
    if (pos_coords2D.x < TILE_DIM + speed) {
        pos_coords2D.x = TILE_DIM;
        return;
    }

    if (pos_coords2D.y < next_tile.y * TILE_DIM - DIFF) moveDown(Vector2D{my_tile.x , my_tile.y + 1});
    else if (pos_coords2D.y > next_tile.y * TILE_DIM + DIFF) moveUp(Vector2D{my_tile.x , my_tile.y - 1});
    else {
        if (((pos_coords2D.x - speed) % TILE_DIM >= TILE_DIM - speed) && !isSteppable(my_tile.x - 1, my_tile.y)) return;//todo round up;

        pos_coords2D.x -= speed;
        if (pos_coords2D.y % TILE_DIM < DIFF) pos_coords2D.y -= pos_coords2D.y % TILE_DIM;
        else pos_coords2D.y += TILE_DIM - pos_coords2D.y % TILE_DIM;
    }
}

void ComputerPlayer::moveRight(const Vector2D & next_tile) {
    if (pos_coords2D.x > WINDOW_WIDTH - 2 * TILE_DIM - speed) {
        pos_coords2D.x = WINDOW_WIDTH - 2 * TILE_DIM;
        return;
    }

    if (pos_coords2D.y < next_tile.y * TILE_DIM - DIFF) moveDown(Vector2D{my_tile.x , my_tile.y + 1});
    else if (pos_coords2D.y > next_tile.y * TILE_DIM + DIFF) moveUp(Vector2D{my_tile.x , my_tile.y - 1});
    else {
        if (((pos_coords2D.x + speed) % TILE_DIM <= speed) && !isSteppable(my_tile.x + 1, my_tile.y)) return;//todo round up;

        pos_coords2D.x += speed;
        if (pos_coords2D.y % TILE_DIM < DIFF) pos_coords2D.y -= pos_coords2D.y % TILE_DIM;
        else pos_coords2D.y += TILE_DIM - pos_coords2D.y % TILE_DIM;
    }
}

bool ComputerPlayer::posAlmostEqual(const Vector2D & dest) const {
    int allowance = speed;

    return (dest.x * TILE_DIM == pos_coords2D.x && dest.y * TILE_DIM + allowance >= pos_coords2D.y &&
            dest.y * TILE_DIM - allowance <= pos_coords2D.y)
           ||
           (dest.y * TILE_DIM == pos_coords2D.y && dest.x * TILE_DIM + allowance >= pos_coords2D.x &&
            dest.x * TILE_DIM - allowance <= pos_coords2D.x);
}


Vector2D ComputerPlayer::findLeastDangerousTile(const Vector2D & start_pos) {
    std::queue <Vector2D> tile_queue;

    int dist_to_bomb, max_dist = 1;

    safe(start_pos.x, start_pos.y, max_dist); //initializes max distance to current
    Vector2D bestTile = start_pos;

    tile_queue.emplace(start_pos);

    Vector2D current;
    int cnt = 0;

    while (!tile_queue.empty()) {
        current = tile_queue.front();
        if (current == start_pos) cnt++;
        if (isSteppable(current.x + 1, current.y)) {
            if (safe(current.x + 1, current.y, dist_to_bomb) || dist_to_bomb > max_dist) {
                bestTile = {current.x + 1, current.y};
                max_dist = dist_to_bomb;
            }
            else tile_queue.emplace(Vector2D{current.x + 1, current.y});
        }

        if (isSteppable(current.x - 1, current.y)) {
            if (safe(current.x - 1, current.y, dist_to_bomb) || dist_to_bomb > max_dist) {
                bestTile = {current.x - 1, current.y};
                max_dist = dist_to_bomb;
            }
            else tile_queue.emplace(Vector2D{current.x - 1, current.y});
        }
        if (isSteppable(current.x, current.y + 1)) {
            if (safe(current.x, current.y + 1, dist_to_bomb) || dist_to_bomb > max_dist) {
                bestTile = {current.x, current.y + 1};
                max_dist = dist_to_bomb;
            }
            else tile_queue.emplace(Vector2D{current.x, current.y + 1});
        }

        if (isSteppable(current.x, current.y - 1)) {
            if (safe(current.x, current.y - 1, dist_to_bomb) || dist_to_bomb > max_dist) {
                bestTile = {current.x, current.y - 1};
                max_dist = dist_to_bomb;
            }
            else tile_queue.emplace(Vector2D{current.x, current.y - 1});
        }

        tile_queue.pop();
        if (cnt == 2) break;
    }
    return bestTile;
}

Vector2D ComputerPlayer::findSafeToBombTile(const Vector2D &start_pos) {
    std::queue <Vector2D> tile_queue;

    int tmp = 0;
    tile_queue.emplace(start_pos);

    Vector2D current;
    my_map[start_pos.y][start_pos.x] = 1;
    if (nextToBox(start_pos.x, start_pos.y)) return start_pos;

    while (!tile_queue.empty()) {
        current = tile_queue.front();

        if (isSteppable(current.x + 1, current.y) && notVisited(current.x + 1, current.y)) {
            my_map[current.y][current.x +1] = my_map[current.y][current.x] + 1;
            if (safe(current.x + 1, current.y, tmp)) {
                if (nextToBox(current.x + 1, current.y)) return Vector2D {current.x + 1, current.y};
                else tile_queue.emplace(Vector2D{current.x + 1, current.y});
            }
        }

        if (isSteppable(current.x - 1, current.y) && notVisited(current.x - 1, current.y)) {
            my_map[current.y][current.x-1] = my_map[current.y][current.x] + 1;
            if (safe(current.x - 1, current.y, tmp)) {
                if (nextToBox(current.x - 1, current.y)) return Vector2D {current.x - 1, current.y};
                else tile_queue.emplace(Vector2D{current.x - 1, current.y});
            }
        }
        if (isSteppable(current.x, current.y + 1) && notVisited(current.x, current.y + 1)) {
            my_map[current.y+1][current.x] = my_map[current.y][current.x] + 1;
            if (safe(current.x, current.y + 1, tmp)) {
                if (nextToBox(current.x, current.y + 1)) return Vector2D {current.x , current.y + 1};
                else tile_queue.emplace(Vector2D{current.x, current.y  + 1});
            }
        }

        if (isSteppable(current.x, current.y - 1) && notVisited(current.x, current.y - 1)) {
            my_map[current.y-1][current.x] = my_map[current.y][current.x] + 1;
            if (safe(current.x, current.y - 1, tmp)) {
                if (nextToBox(current.x, current.y - 1)) return Vector2D {current.x, current.y - 1};
                else tile_queue.emplace(Vector2D{current.x, current.y - 1});
            }
        }
        tile_queue.pop();
    }
    return Vector2D {0,0};
}

bool ComputerPlayer::nextToBox(int x, int y) const {
    for (int col = x; col <= x + bomb_size; col++) {
        if (isBox(col, y)) return true;
        else if (!isSteppable(col, y) || col >= COLS_NBR - 1) break;
    }

    for (int col = x; col >= x - bomb_size; col--) {
        if (isBox(col, y)) return true;
        else if (!isSteppable(col, y) || col <= 1) break;
    }

    for (int row = y; row <= y + bomb_size; row++) {
        if (isBox(x, row)) return true;
        else if (!isSteppable(x, row) || row >= ROWS_NBR - 1) break;
    }

    for (int row = y; row >= y - bomb_size; row--) {
        if (isBox(x, row)) return true;
        if (!isSteppable(x, row) || row <= 1) break;
    }
    return false;
}

void ComputerPlayer::plantBomb(Map & map) {
    if (bombs_active >= max_bombs ) return;

    Vector2D my_tile = getTile2D(pos_coords2D.x, pos_coords2D.y);

    //in the case that player wants to plant 2 bombs at the same place
    if (!map.isSteppable(my_tile.x, my_tile.y)) return;

    bombs_active++;
    bomb_queue.emplace(SDL_GetTicks() + 3800);

    Bomb(my_tile.y, my_tile.x, bomb_size, map);
}

bool ComputerPlayer::checkBoxesLeft(const Map &map) const {
    for (const auto & row: map.map) {
        for (const auto & tile: row) if (tile == '@') return true;
    }
 return false;
}

