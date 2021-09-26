#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include "Map.h"
#include "HumanPlayerArrows.h"
#include "HumanPlayerWASD.h"
#include "Player.h"
#include <map>
#include <cmath>
#include "Configuration.h"
#include "ComputerPlayer.h"
#include <memory>

class Map;
class Player;

/** @brief descripts state of the game */
enum GameState { RUNNING, GAMEOVER, MENU, QUIT };

/** @brief Game class is core class of the game */
class Game {
public:
    /** @brief  Game constructor
     * @details initializes attributes and loads needed textures,
     * calls initialize() at the end*/
    Game();
    /** @brief frees all the allocated textures */
    ~Game();

    /** @brief initializes players, map and highscore */
    void initialize1p();

    /** @brief initializes players, map and highscore */
    void initialize2p();

    /** @brief handles input
     * @param SDL_Event event[in] - describes input event e.g. pressed key, closed win..*/
    void handleEvents(SDL_Event & event);

    /** @brief forwards changes made by handling input, calls update of players and map */
    void update();

    /** @brief calls rendering methods of other visible classes and actually displays it */
    void render();

    /** @brief determines winner and calls saveHighScore */
    void gameOver();

    /** @brief getter to game state needed by other classes */
    GameState getGameState() { return game_state; };

    /** @brief checks whether given symbol is an arrow
     * @returns true if symbol is an arrow, false otherwise */
    static bool isArrow (const SDL_Keycode & symbol);

    /** @brief checks whether given symbol is one of the letters WASD
     * @returns true if symbol is w || a || s || d, false otherwise */
    static bool isWASD (const SDL_Keycode & symbol);

    /** @brief map players, instant access by name
     * a name should be only "WASD" "ARROWS" "PC1" "PC2" and "PC3" */
    std::map <std::string, Player *> players;

    /** @brief renders game over textures  */
    void renderGameOverScreen();

    /** @brief renders main menu texture */
    void renderMenuScreen();
private:
    SDL_Texture * game_over_backgr_texture;
    SDL_Texture * game_over_text_texture1;
    SDL_Texture * game_over_text_texture2;
    SDL_Texture * game_over_text_texture3;
    SDL_Texture * menu_texture;
    SDL_Rect game_over_text_dest_rect1;
    SDL_Rect game_over_text_dest_rect2;
    SDL_Rect game_over_text_dest_rect3;

    GameState game_state;

    int number_of_human_players = 1;

    Map map;

    int highscore = 0;

    /** @brief map of pressed keys
     * @details first argument of the pair is SDL_Keycode,
     * second is bool = true if currently pressed, false otherwise */
    std::map <SDL_Keycode, bool> key_actions;

    /** @brief if currently gained score is higher than highscore, saves it */
    void saveHighScore() const;

    /** @brief loads and creates game over textures
     * @param std::string winner_name[in] - name of the winner to render
     * @param int score[in] score gained of the winner*/
    void loadGameOverText(const std::string & winner_name, int score);

    void renderControls() const;
};

