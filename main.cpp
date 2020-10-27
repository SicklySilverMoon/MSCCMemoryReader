#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include "MemoryReader.h"
#include "GameState.h"
#include "Renderer.h"

int main(int argv, char** args) {
    MemoryReader reader = MemoryReader("otvdmw.exe");
    LPCVOID floatStringPtr = reader.findString("- floating-point support not loaded\x0D\x0A");
    if (floatStringPtr == nullptr) {
        printf("ERROR: FLOAT STRING NOT FOUND\n");
        SDL_Quit();
        exit(-1);
    }
    intptr_t gameStructPtrPtr = reinterpret_cast<intptr_t>(floatStringPtr) + 54;
    //game struct pointer's address is always 54 bytes ahead of this string
    intptr_t gameStructPtr = gameStructPtrPtr - 0x1680; //https://github.com/magical/CHIPS.EXE/blob/master/data.asm#L515

    byte buffer[2];
    reader.readMemory(reinterpret_cast<LPCVOID>(gameStructPtrPtr), buffer, 2);
    gameStructPtr += (buffer[0] | (buffer[1] << 8)); //little endian reverse

    GameState* gs = new GameState;
    Renderer* renderer = new Renderer(gs);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            quit = true;

        reader.readMemory(reinterpret_cast<LPCVOID>(gameStructPtr), reinterpret_cast<byte *>(gs), sizeof(GameState));
        renderer->render();
        SDL_Delay(20);
    }

    return 0;
}