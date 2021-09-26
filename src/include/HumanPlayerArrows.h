#pragma once

#include "Player.h"

/** @brief class HumanPlayerArrows inherits from Player, defines Human Player playing with arrows + shift
 * */
class HumanPlayerArrows: public Player {
public:
    /** @brief HumanPlayerArrows constructor initializes initial position - pos_coords,
     * loads its unique textures and sets up name of the player */
    HumanPlayerArrows();

    ~HumanPlayerArrows() override = default;

    /** @brief moves the player according to input
     * @param action[in] - pressed key
     * @param map[in] - reference to map */
    void move(const SDL_Keycode &keysym, Map & map) override;
};


