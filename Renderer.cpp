//
// Created by SIMMOBILE5 on 26/10/2020.
//

#include <SDL.h>
#include <SDL_image.h>
#include "Renderer.h"
#include "Defs.h"

Renderer::Renderer(GameState* gameState) {
    this->gameState = gameState;

    int WIDTH = TILE_SIZE_X * GRID_SIZE_X * 0.5, HEIGHT = TILE_SIZE_Y * GRID_SIZE_Y * 0.5;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        return;

    window = SDL_CreateWindow("MSCC Memory Reader", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetLogicalSize(renderer, TILE_SIZE_X * GRID_SIZE_X, TILE_SIZE_Y * GRID_SIZE_Y);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    tileSheetSolid = IMG_LoadTexture(renderer, "resources/ms_tiles.png");
    tileSheetTransparent = IMG_LoadTexture(renderer, "resources/ms_overlay.png");
}

void Renderer::render() {

    // clear the screen
    SDL_RenderClear(renderer);

    for (int i = 0; i < GRID_SIZE_X * GRID_SIZE_Y; i++) {
        byte upperTile = gameState->upper[i];
        byte lowerTile = gameState->lower[i];

        int x = (i % GRID_SIZE_X) * TILE_SIZE_X;
        int y = (i / GRID_SIZE_Y) * TILE_SIZE_Y;
        SDL_Rect renderCoord = {x, y, TILE_SIZE_X, TILE_SIZE_Y};

        SDL_Rect upperTileCoord = getTileSheetCoord(upperTile);
        if (lowerTile != FLOOR) {
            SDL_Rect lowerTileCoord = getTileSheetCoord(lowerTile);
            SDL_RenderCopy(renderer, tileSheetSolid, &lowerTileCoord, &renderCoord);
            SDL_RenderCopy(renderer, tileSheetTransparent, &upperTileCoord, &renderCoord);
        }
        else
            SDL_RenderCopy(renderer, tileSheetSolid, &upperTileCoord, &renderCoord);
    }

    // flip the backbuffer
    // this means that everything that we prepared behind the screens is actually shown
    SDL_RenderPresent(renderer);
}

SDL_Rect Renderer::getTileSheetCoord(int index) {
    if (index > 0x6F) //The last tile in the sheet
        return {0, 0, TILE_SIZE_X, TILE_SIZE_Y};
    int x = (index / 16) * TILE_SIZE_X;
    int y = (index % 16) * TILE_SIZE_Y;
    return {x, y, TILE_SIZE_X, TILE_SIZE_Y};
}
