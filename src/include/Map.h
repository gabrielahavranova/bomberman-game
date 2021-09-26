#pragma once
#include "TextureManager.h"
#include <queue>
#include <deque>
#include "Explosion.h"
#include <iostream>
#include <map>
#include "FileManager.h"
#include "Configuration.h"


class Bomb;
class Player;
class Explosion;

/** @brief class Map represents game map made of tiles
 * */
class Map {
public:
    /** @brief calls loadTextures(), loads map, in case of failure loads default map
     * @param map_id[in] defines id of the map to load*/
    Map(int map_id);

    /** @brief frees allocated textures*/
    ~Map();

    /** @brief draws every tile of the map
     * @details draws texture of every tile, calls draw method for every explosion in the queue */
    void drawMap();

    /** @brief updates planted_bombs and explosions queue
     * @param player[in] passes to the Explosion::checkPlayers method */
    void update(std::map <std::string, Player *> & players);

    /** @brief plants a bomb on the map
     * @param col[in] defines column of the bomb
     * @param row[in] defines row of the bomb
     * @param bomb[in] instance of Bomb, this method stores it in the planted_bombs queue */
    void addBomb (const int & col, const int & row, Bomb & bomb);

    /** @brief creates Explosion instance and puts it into explosions queue
     * @param bomb[in] a bomb to create explosion from */
    void bombExplosion (const Bomb & bomb);

    /** @brief checks whether a tile is steppable
     * @param col[in] tile column
     * @param row[in] tile row
     * @returns true if a player can step on the tile, false otherwise */
    bool isSteppable (const int & col, const int & row) const;

    /** @brief checks whether a tile can be blown up
     * @param col[in] tile column
     * @param row[in] tile row
     * @returns true if the tile is wood/box, false otherwise */
    bool isWood (const int & col, const int & row);

    /** @brief sets tile as plain steppable
     * @param col[in] column of input tile to change
     * @param row[in] row of the input tile to change
     * @returns void, changes tile at the [row][col] position to '.' */
    void destroyObject (const int & col, const int & row);

    /** @brief generates random booster at given tile with 1/4 probability
     * @param col[in] tile column
     * @param row[in] tile row
     **/
    void generateBooster (const int & col, const int & row);

    /** @brief checks wheter on given tile is any of the boosters
     * @param col[in] tile column
     * @param row[in] tile row
     * @returns char representing the booster, '!' if none of the boosters is present
     **/
    char isBooster (const int & col, const int & row);

    /** @brief map is represented by basic 2D array of chars, where each char represents a type of tile */
    char map[ROWS_NBR][COLS_NBR];

    SDL_Texture * expl_horizontal_texture;
    SDL_Texture * expl_vertical_texture;
    SDL_Texture * expl_cross_texture;

private:
    /** @brief saves its @param arr[in] as a member variable map */
    void loadMap(char arr[ROWS_NBR][COLS_NBR]);

    void loadTextures();
    SDL_Texture * sand;
    SDL_Texture * base_brick;
    SDL_Texture * bomb;
    SDL_Texture * wood;
    SDL_Texture * booster;

    /**@brief texture_dict translates char saved in map of tiles to SDL_Textures
     * */
    std::map <char, SDL_Texture *> texture_dict;

    /** @brief rectangle used to render a single tile (defines x, y, width, height)
     * */
    SDL_Rect src_rect, dest_rect;

    /** @brief this queue is used to store planted bombs chronologically */
    std::queue <Bomb> planted_bombs;

    /** @brief this deque is used to store explosions chronologically */
    std::deque <Explosion> explosions;
};

/**@brief small class to represent a single Bomb */
class Bomb {
public:
    /**@brief constructor which sets up explosion time and calls Map::addBomb method
     * @param col[in] column of the bomb
     * @param row[in] row of the bomb
     * @param explosion_size[in] expected size of the explosion
     * @param map[in] reference to a map */
    Bomb (const int & col, const int & row, const int & explosion_size, Map & map);

    /**@brief getter
     * @returns nbr of miliseconds from start of the game at which the bomb should explode */
    uint32_t getExplosionTime() const { return explosion_time; }

    int col, row, bomb_size;
    /**@brief  represents br of miliseconds from start of the game at which the bomb should explode*/
    uint32_t explosion_time;
};

