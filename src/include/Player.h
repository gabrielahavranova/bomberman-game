#pragma once

#include "TextureManager.h"
#include <SDL2/SDL.h>
#include "Map.h"
#include <cmath>
#include <queue>
#include "Configuration.h"
#include <string>

/**@brief simple struct to represent pair of coordinates x and y, supports == and != operators * */
struct Vector2D {
    int x;
    int y;
    bool operator==(const Vector2D & second) const { return (x == second.x && y == second.y); }
    bool operator!=(const Vector2D & second) const { return (x != second.x || y != second.y); }
};

class Map;

/**@brief base class Player to represent any player * */
class Player {
public:
    /**@brief initializes src & dest rectangles used to render player's textures * */
    Player();

    /** @brief frees the textures */
    virtual ~Player() = default;

    /**@brief renders Player itself and calls method renderScore * */
    void render();

    /**@brief moves the player
     * @param action[in] code of key pressed
     * @param map[in] reference to the map * */
    virtual void move (const SDL_Keycode & action, Map & map) = 0;

    /**@brief updates the player * */
    void update();

    /**@brief plants bomb on current tile
     * @param map[in] reference to the map
     * */
    virtual void plantBomb (Map & map);

    /**@brief plants bomb on the tile where the Player is standing
     * @param map[in] reference to the map * */
    void death();

    /**@brief getter of current player's position
     * @returns coordinates the player is standing on * */
    Vector2D getCoords2D() const { return pos_coords2D; }

    /**@brief getter of player's score
     * @returns player's current score * */
    int getScore() const { return score; }

    /**@brief getter of player's name
     * @returns player's name */
    std::string getName() const { return player_name; }

    /**@brief checks whether the player is alive
     * @returns true if it's alive, false otherwise * */
    bool isAlive() const { return alive; }
protected:
    std::string player_name;

    /**@brief gets tile from coordinates
     * @param x_pos[in] x position
     * @param y_pos[in] y position
     * @returns coordinates of the tile which is player covering the most of * */
    static Vector2D getTile2D (const int & x_pos, const int & y_pos);

    /**@brief represents current position of the player */
    Vector2D pos_coords2D;

    /**@brief player's list of the planted bombs stored in queue */
    std::queue <uint32_t> bomb_queue;

    bool alive = true;
    int score = 0;


    int score_length = 1;
    short max_bombs = 1;
    short speed = 2;
    short bombs_active = 0;
    short bomb_size = 1;

    /**@brief updates length of the current score = number of digits * */
    void updateScoreLength();

    SDL_Texture * player_texture;
    SDL_Texture * dead_player_texture;
    SDL_Texture * name_texture;
    SDL_Texture * score_texture;

    /**@brief sets texture of player's name */
    void setNameTexture();

    /**@brief updates score texture */
    void updateScoreTexture();

    /**@brief renders player's score */
    void renderScore();

    SDL_Rect player_src_rect, player_dest_rect;
    SDL_Rect name_dest_rect, score_dest_rect;

    /**@brief takes boost from the current tile if there is any
     * @param boost_nbr[in] char which represents the boost
     * */
    void claimBoost (const char & boost_nbr);
};
