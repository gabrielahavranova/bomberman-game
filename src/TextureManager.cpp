//
// Created by gabi on 03/04/2020.
//

#include "./include/TextureManager.h"

SDL_Renderer * TextureManager::renderer = nullptr;
TTF_Font * TextureManager::font = nullptr;
SDL_Color TextureManager::text_color =  {255, 255, 255, 0};
std::vector <SDL_Texture *> TextureManager::textures;

SDL_Texture * TextureManager::loadTexture(const char * texture) {
    SDL_Surface * temp_surface = IMG_Load(texture);
    SDL_Texture * tex = SDL_CreateTextureFromSurface(renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    return tex;
}

void TextureManager::draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest) {
    SDL_RenderCopy(renderer, tex, &src, &dest);

}

void TextureManager::init(SDL_Window * window) {
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (TextureManager::renderer) {
        SDL_SetRenderDrawColor(TextureManager::renderer, 255, 255, 255, 255);
        std::cout << "Renderer created!" << std::endl;
    } else std::cout << "unable to create renderer!" << std::endl;

    TTF_Init();
    TextureManager::font = TTF_OpenFont("src/assets/font.ttf", 100);
}

void TextureManager::cleanUp() {
    for (SDL_Texture * texture : textures) if (texture != nullptr) SDL_DestroyTexture(texture);
    IMG_Quit();
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
}
