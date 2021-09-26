#include "./include/Game.h"


Map * map;


Game::Game():map(rand() % 5 + 1) {
    key_actions.emplace(SDLK_w, false);
    key_actions.emplace(SDLK_a, false);
    key_actions.emplace(SDLK_s, false);
    key_actions.emplace(SDLK_d, false);
    key_actions.emplace(SDLK_UP, false);
    key_actions.emplace(SDLK_DOWN, false);
    key_actions.emplace(SDLK_LEFT, false);
    key_actions.emplace(SDLK_RIGHT, false);

    game_state = MENU;

    game_over_backgr_texture = TextureManager::loadTexture("src/assets/black_backgr.png");
    menu_texture = TextureManager::loadTexture("src/assets/menu.png");
    TextureManager::textures.push_back(game_over_backgr_texture);
    TextureManager::textures.push_back(menu_texture);

    highscore = FileManager::loadHighScore();
    if (highscore != 0) std::cout << "highscore: " << highscore << std::endl;
}


bool Game::isArrow(const SDL_Keycode &symbol) {
    return symbol == SDLK_UP || symbol == SDLK_RIGHT || symbol == SDLK_DOWN || symbol == SDLK_LEFT;
}

bool Game::isWASD(const SDL_Keycode &symbol) {
    return symbol == SDLK_w || symbol == SDLK_a || symbol == SDLK_s || symbol == SDLK_d;
}

void Game::initialize1p() {
    number_of_human_players = 1;
    players["WASD"] = new HumanPlayerWASD();
    players["PC1"] = new ComputerPlayer(700, 50, "RoboCat", 420, 0, "src/assets/Robokitty1.png", "src/assets/Robokitty1dead.png");
    players["PC2"] = new ComputerPlayer(50, 50, "Snowball", 20, 0, "src/assets/robokitty2.png", "src/assets/robokitty2dead.png");
    players["PC3"] = new ComputerPlayer(50, 500, "LuckyPaw", 420, 550, "src/assets/robokitty3.png", "src/assets/robokitty3dead.png");

    std::cout << "init finalized 1p" << std::endl;
}

void Game::initialize2p() {
    number_of_human_players = 2;
    players["WASD"] = new HumanPlayerWASD();
    players["ARROWS"] = new HumanPlayerArrows();
    players["PC1"] = new ComputerPlayer(700, 50, "RoboCat", 420, 0, "src/assets/Robokitty1.png", "src/assets/Robokitty1dead.png");
    players["PC2"] = new ComputerPlayer(50, 500, "LuckyPaw", 420, 550, "src/assets/robokitty3.png", "src/assets/robokitty3dead.png");
    std::cout << "init finalized 2p" << std::endl;
}

void Game::handleEvents(SDL_Event & event) {
    SDL_PollEvent(&event);

    SDL_Keycode pressed_key = event.key.keysym.sym;
    switch (event.type) {
        case SDL_QUIT:
            game_state = QUIT;
            break;
        case SDL_KEYDOWN:
            if (pressed_key == SDLK_ESCAPE) game_state = QUIT;
            else {
                if (pressed_key == SDLK_F1 && game_state != RUNNING) {
                    game_state = RUNNING;
                    renderControls();
                    initialize2p();
                }
                else if (pressed_key == SDLK_F2 && game_state != RUNNING) {
                    game_state = RUNNING;
                    renderControls();
                    initialize1p();
                }
                if (game_state == RUNNING) {
                    if (isArrow(pressed_key) || isWASD(pressed_key)) {
                        key_actions.at(pressed_key) = true;
                    }
                    if (pressed_key == SDLK_RSHIFT && number_of_human_players == 2 && players.at("ARROWS")->isAlive()) {
                        players.at("ARROWS")->plantBomb(map);
                    }
                    if (pressed_key == SDLK_SPACE && players.at("WASD")->isAlive()) {
                        players.at("WASD")->plantBomb(map);
                    }
                }
            }
            break;
        case SDL_KEYUP:
            if ((isArrow(pressed_key) || isWASD(pressed_key)) && game_state == RUNNING) {
                key_actions.at(pressed_key) = false;
            }
            break;
        default:
            break;
    }
    if (game_state != RUNNING) return;

    for (auto & key:key_actions) {
        if (key.second == true) {
            if (isArrow(key.first) && number_of_human_players == 2 && players.at("ARROWS")->isAlive())
                players.at("ARROWS")->move(key.first, map);
            else if (isWASD(key.first) && players.at("WASD")->isAlive())
                players.at("WASD")->move(key.first, map);
        }
    }
    if (players.at("PC1")->isAlive()) players.at("PC1")->move(SDLK_0, map);
    if (players.at("PC2")->isAlive())players.at("PC2")->move(SDLK_0, map);
    if (number_of_human_players == 1 && (players.at("PC3")->isAlive()))
        players.at("PC3")->move(SDLK_0, map);
}

void Game::update() {
    int alive = 0;
    for (const auto & player:players) if (player.second->isAlive()) alive++;
    if ((number_of_human_players == 2 && !players.at("WASD")->isAlive() && !players.at("ARROWS")->isAlive() ) ||
        (number_of_human_players == 1 && !players.at("WASD")->isAlive()) || alive <= 1) {
        gameOver();
        return;
    }

    map.update(players);
    for (auto & player:players) player.second->update();
}

void Game::render() {
    SDL_RenderClear(TextureManager::renderer);
    map.drawMap();
    for (auto & player:players) player.second->render();
    SDL_RenderPresent(TextureManager::renderer);
}


void Game::gameOver() {
    saveHighScore();
    SDL_Delay(1000);
    game_state = GAMEOVER;

    Player * winner;
    for (const auto & player : players) if (player.second->isAlive()) winner = player.second;
    loadGameOverText(winner->getName(), winner->getScore());
}

void Game::saveHighScore() const {
    int max_score = 0;

    for (const auto & player : players)
        if (player.second->getScore() > max_score) max_score = player.second->getScore();

    if (max_score > highscore) FileManager::saveHighScore(max_score);
}

void Game::renderGameOverScreen() {
    SDL_RenderClear(TextureManager::renderer);

    TextureManager::draw(game_over_backgr_texture, DEF_SRC_RECT, DEF_DEST_RECT);
    TextureManager::draw(game_over_text_texture1, TEXT_SRC_RECT, game_over_text_dest_rect1);
    TextureManager::draw(game_over_text_texture2, TEXT_SRC_RECT, game_over_text_dest_rect2);
    TextureManager::draw(game_over_text_texture3, TEXT_SRC_RECT, game_over_text_dest_rect3);
    SDL_RenderPresent(TextureManager::renderer);
    SDL_Delay(50);
}

void Game::renderMenuScreen() {
    SDL_RenderClear(TextureManager::renderer);

    TextureManager::draw(menu_texture, DEF_SRC_RECT, DEF_DEST_RECT);
    SDL_RenderPresent(TextureManager::renderer);
    SDL_Delay(10);
}

void Game::loadGameOverText(const std::string & winner_name, int score) {
    game_over_text_dest_rect1 = { 200, 300, 400, 50 };
    game_over_text_dest_rect2 = { 240, 350, 350, 40 };
    game_over_text_dest_rect3 = { 250, 400, 300, 40 };

    std::string tmp1 = winner_name + " WINS";
    const char * output_text1 = tmp1.c_str();

    std::string tmp2 = "score: " + std::to_string(score);
    const char * output_text2 = tmp2.c_str();

    std::string tmp3 = "best: " + std::to_string(highscore);
    const char * output_text3 = tmp3.c_str();

    SDL_Surface * surface1 =  TTF_RenderText_Solid(TextureManager::font, output_text1, TextureManager::text_color);
    SDL_Surface * surface2 =  TTF_RenderText_Solid(TextureManager::font, output_text2, TextureManager::text_color);
    SDL_Surface * surface3 =  TTF_RenderText_Solid(TextureManager::font, output_text3, TextureManager::text_color);

    game_over_text_texture1 = SDL_CreateTextureFromSurface(TextureManager::renderer, surface1);
    game_over_text_texture2 = SDL_CreateTextureFromSurface(TextureManager::renderer, surface2);
    game_over_text_texture3 = SDL_CreateTextureFromSurface(TextureManager::renderer, surface3);
    TextureManager::textures.push_back(game_over_text_texture1);
    TextureManager::textures.push_back(game_over_text_texture2);
    TextureManager::textures.push_back(game_over_text_texture3);

    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    SDL_FreeSurface(surface3);
}

Game::~Game() {
    for (auto & player: players) delete player.second;
}

void Game::renderControls() const {
    SDL_RenderClear(TextureManager::renderer);
    SDL_Texture * texture = TextureManager::loadTexture("src/assets/controls.png");
    TextureManager::draw(texture, DEF_SRC_RECT, DEF_DEST_RECT);
    SDL_RenderPresent(TextureManager::renderer);
    SDL_Delay(4000);
    if (texture != nullptr) SDL_DestroyTexture(texture);
}
