//
// Created by gabi on 03/04/2020.
//
#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>

/**@brief "static" class to manage texture loading and drawing * */
class TextureManager {
public:
    /**@brief loads texture from file
    * @param file_name[in] path to the file
    * */
    static SDL_Texture * loadTexture(const char * file_name);

    /**@brief draws given texture
    * @param tex[in] texture to draw
    * @param src[in] texture's source rectangle
    * @param dest[in] destination rectangle where to draw at
    * */
    static void draw(SDL_Texture * tex, SDL_Rect src, SDL_Rect dest);

    /**@brief initializes renderer and font
    * @param pointer to a window
    * */
    static void init(SDL_Window * window);

    /**@brief calls SDL methods to free resources allocated
    * */
    static void cleanUp();

    static SDL_Renderer * renderer ;

    static SDL_Color text_color;

    static TTF_Font * font;

    static std::vector <SDL_Texture *> textures;
};
