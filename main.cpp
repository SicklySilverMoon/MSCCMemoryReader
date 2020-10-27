#include <iostream>
#include <SDL.h>
#include <Windows.h>
#include "MemoryReader.h"
#include "GameState.h"
#include "Renderer.h"

#define WINDOW_TITLE "otvdmw.exe"

MemoryReader* createAndPrepareReader(const std::string& windowTitle, intptr_t* gameStructPtr);
MemoryReader* destroyAndRemakeReader(MemoryReader* reader, const std::string& windowTitle, intptr_t* gameStructPtr);

int main(int argv, char** args) {
    auto* gameStructPtr = new intptr_t;
    MemoryReader* reader = createAndPrepareReader(WINDOW_TITLE, gameStructPtr);

    auto* gameState = new GameState;
    auto* renderer = new Renderer(gameState);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        SDL_PollEvent(&e);
        if (e.type == SDL_QUIT || e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            quit = true;

        if (reader->readMemory(reinterpret_cast<LPCVOID>(*gameStructPtr), reinterpret_cast<byte *>(gameState), sizeof(GameState)) == -1) {
            reader = destroyAndRemakeReader(reader, WINDOW_TITLE, gameStructPtr);
            continue;
        }

        renderer->render();
        SDL_Delay(20);
    }

    return 0;
}

MemoryReader* destroyAndRemakeReader(MemoryReader* reader, const std::string& windowTitle, intptr_t* gameStructPtr) {
    delete reader;
    return createAndPrepareReader(WINDOW_TITLE, gameStructPtr);
}

MemoryReader* createAndPrepareReader(const std::string& windowTitle, intptr_t* gameStructPtr) {
    auto* reader = new MemoryReader(windowTitle);
    LPCVOID floatStringPtr = reader->findString("- floating-point support not loaded\x0D\x0A");
    if (floatStringPtr == nullptr) {
        printf("ERROR: FLOAT STRING NOT FOUND\n");
        return destroyAndRemakeReader(reader, windowTitle, gameStructPtr);
    }
    intptr_t gameStructPtrPtr = reinterpret_cast<intptr_t>(floatStringPtr) + 54;
    //game struct pointer's address is always 54 bytes ahead of this string
    *gameStructPtr = gameStructPtrPtr - 0x1680; //https://github.com/magical/CHIPS.EXE/blob/master/data.asm#L515

    byte buffer[2];
    if (reader->readMemory(reinterpret_cast<LPCVOID>(gameStructPtrPtr), buffer, 2) == -1)
        return destroyAndRemakeReader(reader, windowTitle, gameStructPtr);
    *gameStructPtr += (buffer[0] | (buffer[1] << 8)); //little endian reverse
    return reader;
}