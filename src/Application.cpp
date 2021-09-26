#include "./include/Application.h"

Application::Application():window(nullptr) {
    if (init()) mainLoop();
    else std::cout << "a problem has occured while initializing!" << std::endl;
}

bool Application::init() {
    srand(time(nullptr));
    int flags = 0;
    if (FULLSCREEN) flags = SDL_WINDOW_FULLSCREEN;

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        window = SDL_CreateWindow("BomberCat", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH, WINDOW_HEIGHT, flags);

        if (!window) return false;
        TextureManager::init(window);
    } else return false;
    std::cout << "init finalized" << std::endl;
    return true;
}


void Application::limitFPS(const uint32_t & frame_start_time) {
    const int frame_delay = 1000 / FPS; // 1s / desired FPS
    uint32_t frame_time = SDL_GetTicks() - frame_start_time;

    if (frame_delay > frame_time) { //frame is rendered too fast -> delay it
        SDL_Delay(frame_delay - frame_time);
    }
}

void Application::mainLoop() {
    uint32_t frame_start_time;

    Game game;
    SDL_Event event;

    while (game.getGameState() != QUIT) {
        if (game.getGameState() == RUNNING) {
            frame_start_time = SDL_GetTicks();

            game.handleEvents(event);
            game.update();
            game.render();
            limitFPS(frame_start_time);
        }
        else if (game.getGameState() == GAMEOVER) {
            game.handleEvents(event);
            game.renderGameOverScreen();
        }
        else { //MENU
            game.handleEvents(event);
            game.renderMenuScreen();
        }
    }
    cleanUp();
}

void Application::cleanUp() {
    TextureManager::cleanUp();
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Game cleaned" << std::endl;
}
