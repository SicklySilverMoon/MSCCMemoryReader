//
// Created by SIMMOBILE5 on 16/09/2020.
//

#ifndef CCMEMORYREADER_MEMORYREADER_H
#define CCMEMORYREADER_MEMORYREADER_H


#include <string>
#include <windows.h>
#include <vector>

class MemoryReader {
private:
    HANDLE processHandle;
    std::vector<MEMORY_BASIC_INFORMATION> memoryChunks;

    static SIZE_T findSubarray(byte super[], byte sub[], SIZE_T superSize, SIZE_T subSize);

public:
    explicit MemoryReader(const std::string& programName);
    LPCVOID findString(const std::string& string);
    void readMemory(LPCVOID address, byte buffer[], SIZE_T bytesToRead);
};


#endif //CCMEMORYREADER_MEMORYREADER_H
