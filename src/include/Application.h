#pragma once
#include "Game.h"


/** @brief Application is the main class of the game
 *
 * @details This class manages everything from initialization of resources, main game loop and
 * cleaning up.
 */
class Application {
public:
    /** @brief calls init of the application, if successed, calls mainLoop  */
    Application();
private:
    /** @brief initializes SDL_library and window
     * @returns true if succeeded, false if any error occurred */
    bool init();

    /** @brief manages game loop = what to do every frame   */
    void mainLoop();

    /** @brief destroys SDL-allocated resources */
    void cleanUp();

    /** @brief SDL output = main game window */
    SDL_Window * window;

    /** @brief Limits FPS of the game
     *
     * @details computes length of rendering a single frame and delays accordingly
     *
     * @param[in] frame_start_time
     */
    static void limitFPS(const uint32_t & frame_start_time) ;
};

