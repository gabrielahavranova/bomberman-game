#pragma once

#include <cstdint>
#include <vector>
#include <map>
#include "Player.h"
#include "Map.h"
#include "TextureManager.h"

class Player;
class Map;

/** @brief trivial structure with int x and int y positions */
struct Coordinates {
    Coordinates (const int & x, const int & y):x(x), y(y) {}
    int x;
    int y;
};

/** @brief computes the explosion position and size
 * @details computes exact tiles hit by explosion of the bomb,
 * draws itself and also takes care of killing players and destroying
 * objects hit by explosion
 * */
class Explosion {
public:
    /** @brief Explosion constructor
     *
     * @details computes x and y coordinates of start and end of the explosion
     * (x_start_pos, x_end_pos, y_start_pos, y_end_pos)
     *
     * @param[in] size is size of the explosion - determined by player's bomb_size attribute
     * @param[in] bomb_x_pos x position of exploded bomb
     * @param[in] bomb_y_pos y position of exploded bomb
     * @param[in] end_time determines end of the explosion in global time
     * @param[in] pointer to a map, needed to check tiles and players */
    Explosion (const int & size, const int & bomb_x_pos, const int & bomb_y_pos,
            const uint32_t & end_time, Map * map);

    /** @brief renders explosion to the map */
    void draw() const;

    /** @brief checks whether any player is hit by explosion
     *
     * @param[in] players map of players needed to loop through */
    void checkPlayers (std::map <std::string, Player *> & players);

    /** @brief checks whether any destroyable objects are hit and destroys them accordingly */
    void destroyObjects() const;

    Coordinates start_pos;
    uint32_t end_time;
private:
    int size;
    Map * map;
    int x_start_pos, x_end_pos, y_start_pos, y_end_pos;
};


