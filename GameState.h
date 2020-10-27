//
// Created by SIMMOBILE5 on 17/09/2020.
//

#ifndef CCMEMORYREADER_GAMESTATE_H
#define CCMEMORYREADER_GAMESTATE_H
#include <Windows.h>


class GameState {
public: //all of these will be read in as little endian, if your system doesn't have little endian memory, uhh
    byte upper[1024];
    byte lower[1024];
    short levelNumber;
    short levelsInSet;
    short initialTime;
    short initialChips;
    short chipX;
    short chipY;
    short sliding;
    short buffered; //buffered move in wait
    short levelPlacardVisible; //Whether the "Level Title / Password: ZORK" should be shown
    short bufferedX; //buffered move direction
    short bufferedY;
    short autopsy;
    short slideX;
    short slideY;

    short initialMonsterListLength;
    short initialMonsterList[128]; //initial monster list is always 128 entires with 2 bytes, x and y

    short slipListLength;
    short slipListCapacity;
    short slipListHandle;
    short slipListPtr;
    short slipListSegment;

    short monsterListLength;
    short monsterListCapacity;
    short monsterListHandle;
    short monsterListPtr;
    short monsterListSegment;

    short toggleListLength;
    short toggleListCapacity;
    short toggleListHandle;
    short toggleListPtr;
    short toggleListSegment;

    short trapListLength;
    short trapListCapacity;
    short trapListHandle;
    short trapListPtr;
    short trapListSegment;

    short cloneListLength;
    short cloneListCapacity;
    short cloneListHandle;
    short cloneListPtr;
    short cloneListSegment;

    short teleportListLength;
    short teleportListCapacity;
    short teleportListHandle;
    short teleportListPtr;
    short teleportListSegment;

    byte levelTitle[64];
    byte levelHint[128];
    byte levelPassword[10];

    short viewportX;
    short viewportY;
    short viewportWidth; //always 9 by 9
    short viewportHeight;

    short unusedOffsetX; //actually a delta by how much to shift screen for drawing
    short unusedOffsetY; //recommended not to mess with

    short restartCount;
    short melindaCount; //progress towards "you seem to be having trouble"

    short stepCount;
    short endingTick;
    short hasMouseTarget;
    short mouseTargetX;
    short mouseTargetY;
    short idleTickCount;
    short chipHasMoved;

    short gameStateSize; //why the hell is this at the end
};


#endif //CCMEMORYREADER_GAMESTATE_H
