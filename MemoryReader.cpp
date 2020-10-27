//
// Created by SIMMOBILE5 on 16/09/2020.
//

#include "MemoryReader.h"
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <SDL_timer.h>

MemoryReader::MemoryReader(const std::string& programName) {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    bool found = false;
    while (!found) {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,
                                                   NULL); //grab a system snapshot of all current processes

        if (Process32First(snapshot, &entry) == TRUE) { //grab the first process in snapshot
            while (Process32Next(snapshot, &entry) == TRUE) { //keep grabbing processes through the list
                if (stricmp(entry.szExeFile, programName.c_str()) == 0) { //if title matches the one we want
                    processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,
                                                entry.th32ProcessID);
                    found = true;
                    break;
                }
            }
            if (!found) {
                printf("otvdwm.exe not found, trying again in 2 seconds\n");
                SDL_Delay(2000); //delay if nothing found
            }
        }
        CloseHandle(snapshot);
    }

    LPCVOID lpAddress = 0x0; //https://stackoverflow.com/a/13881951 using this method
    MEMORY_BASIC_INFORMATION* basicInformation = new MEMORY_BASIC_INFORMATION;
    while (VirtualQueryEx(processHandle, lpAddress, basicInformation, sizeof(MEMORY_BASIC_INFORMATION)) > 0) {
        if (basicInformation->Protect == PAGE_EXECUTE_READWRITE || basicInformation->Protect == PAGE_READWRITE) //give all sections that can read from, written to, or executed from
            memoryChunks.push_back(*basicInformation);

        lpAddress = reinterpret_cast<LPCVOID>(reinterpret_cast<intptr_t>(lpAddress) + basicInformation->RegionSize);
    }
    delete basicInformation;
}

LPCVOID MemoryReader::findString(const std::string& string) {
    const char* cString = string.c_str();
    SIZE_T cStringSize = string.length() + 1; //+ 1 for null on end

    for (MEMORY_BASIC_INFORMATION chunk : memoryChunks) {
        byte* buffer = new byte[chunk.RegionSize];
        ReadProcessMemory(processHandle, chunk.BaseAddress, buffer, chunk.RegionSize, NULL);
        SIZE_T offset = findSubarray(buffer, (byte *) cString, chunk.RegionSize, cStringSize);
        delete[] buffer;
        if (offset != SIZE_MAX)
            return reinterpret_cast<LPCVOID>(reinterpret_cast<intptr_t>(chunk.BaseAddress) + offset);
    }
    return nullptr;
}

void MemoryReader::readMemory(LPCVOID address, byte *buffer, SIZE_T bytesToRead) {
    ReadProcessMemory(processHandle, address, buffer, bytesToRead, NULL);
}

SIZE_T MemoryReader::findSubarray(byte *super, byte *sub, SIZE_T superSize, SIZE_T subSize) {
    if (superSize == 0 || subSize == 0)
        return SIZE_MAX;

    byte subFirstByte = sub[0];
    for (SIZE_T i=0; i < superSize; i++) {
        if (super[i] == subFirstByte) {
            if (subSize == 1)
                return i;
            for (SIZE_T j=1; j < subSize; j++) {
                if (sub[j] != super[i + j])
                    break;
                if (j == subSize - 1)
                    return i;
            }
        }
    }
    return SIZE_MAX;
}
