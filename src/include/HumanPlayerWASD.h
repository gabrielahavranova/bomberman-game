#pragma once

#include "Player.h"

/** @brief class HumanPlayerWASD inherits from Player, defines Human Player playing with WASD + space
 * */
class HumanPlayerWASD: public Player {
public:
    /** @brief HumanPlayerWASD constructor initializes initial position - pos_coords,
     * loads its unique textures and sets up name of the player */
    HumanPlayerWASD();

    ~HumanPlayerWASD() override = default;

    /** @brief moves the player according to input
     * @param action[in] - pressed key
     * @param map[in] - reference to map */
    void move (const SDL_Keycode &, Map & map) override;
};


