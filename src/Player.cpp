#include "./include/Player.h"


class Bomb;

Player::Player() {
    player_src_rect = {0, 0, TILE_DIM, TILE_DIM };
    player_dest_rect = {0, 0, TILE_DIM, TILE_DIM};
}

void Player::render() {
    player_dest_rect.x = pos_coords2D.x;
    player_dest_rect.y = pos_coords2D.y;
    renderScore();
    TextureManager::draw(player_texture, player_src_rect, player_dest_rect);
}


Vector2D Player::getTile2D(const int &x_pos, const int &y_pos) {
    Vector2D result;
    if (x_pos % 50 < 15) result.x = x_pos / 50;
    else result.x = x_pos / 50 + 1;

    if (y_pos % 50 < 15) result.y = y_pos / 50;
    else result.y = y_pos / 50 + 1;

    return result;
}

void Player::plantBomb(Map & map) {
    if (bombs_active >= max_bombs ) return;

    Vector2D my_tile = getTile2D(pos_coords2D.x, pos_coords2D.y);

    //in the case that player wants to plant 2 bombs at the same place
    if (!map.isSteppable(my_tile.x, my_tile.y)) return;

    bombs_active++;
    bomb_queue.emplace(SDL_GetTicks() + 3000);

    Bomb(my_tile.y, my_tile.x, bomb_size, map);
}

void Player::update() {
    if (!bomb_queue.empty() && bomb_queue.front() <= SDL_GetTicks()) {
        bombs_active--;
        bomb_queue.pop();
    }
}

void Player::death() {
    player_texture = dead_player_texture;

    score /= 100;
    updateScoreLength();
    updateScoreTexture();
    alive = false;
}

void Player::claimBoost(const char & boost_type) {
    int old_score = score;
    switch (boost_type) {
        case 's':
            if (speed < MAX_SPEED) speed++;
            else score += 50;
            break;
        case 'b':
            if (bomb_size < MAX_BOMB_SIZE) bomb_size++;
            else score += 50;
            break;
        case 'c':
            if (max_bombs < MAX_BOMB_CNT) max_bombs++;
            else score += 50;
            break;
        case 'm':
            score += 200;
            break;
        default:
            break;
    }
    if (old_score != score) updateScoreTexture();
}


void Player::updateScoreLength() {
    score_length = static_cast<int>(log10(score)) + 1;
}

void Player::renderScore() {
    TextureManager::draw(name_texture, TEXT_SRC_RECT, name_dest_rect);
    TextureManager::draw(score_texture, TEXT_SRC_RECT, score_dest_rect);
}

void Player::setNameTexture() {
    SDL_Surface * surface =  TTF_RenderText_Solid(TextureManager::font, player_name.c_str(), TextureManager::text_color);

    name_texture = SDL_CreateTextureFromSurface(TextureManager::renderer, surface);
    TextureManager::textures.push_back(name_texture);

    SDL_FreeSurface(surface);
    updateScoreTexture();
}

void Player::updateScoreTexture() {
    updateScoreLength();

    std::string str_score = std::to_string(score);
    const char * chars_score = str_score.c_str();

    SDL_Surface * surface = TTF_RenderText_Solid(TextureManager::font, chars_score, TextureManager::text_color);

    score_texture = SDL_CreateTextureFromSurface(TextureManager::renderer, surface);
    TextureManager::textures.push_back(score_texture);

    score_dest_rect.x = name_dest_rect.x + name_dest_rect.w + TILE_DIM;
    score_dest_rect.w = score_length * 30;
    SDL_FreeSurface(surface);
}



