#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

//Given file path and renderer, return loaded SDL_Texture
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);

//Given texture and renderer, render the texture
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y);