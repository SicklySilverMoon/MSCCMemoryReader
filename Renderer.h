//
// Created by SIMMOBILE5 on 26/10/2020.
//

#ifndef CCMEMORYREADER_RENDERER_H
#define CCMEMORYREADER_RENDERER_H


#include <SDL_video.h>
#include <SDL_render.h>
#include "GameState.h"

class Renderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* tileSheetSolid;
    SDL_Texture* tileSheetTransparent;
    GameState* gameState;

    SDL_Rect getTileSheetCoord(int index);

public:
    Renderer(GameState* gameState);
    void render();
};


#endif //CCMEMORYREADER_RENDERER_H
