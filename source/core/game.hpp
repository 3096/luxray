#pragma once

#include <switch.h>

extern "C" {

typedef struct {
    u64 base;
    u64 size;
} DmntMemoryRegionExtents;

typedef struct {
    u64 process_id;
    u64 title_id;
    DmntMemoryRegionExtents main_nso_extents;
    DmntMemoryRegionExtents heap_extents;
    DmntMemoryRegionExtents alias_extents;
    DmntMemoryRegionExtents address_space_extents;
    u8 main_nso_build_id[0x20];
} DmntCheatProcessMetadata;

Result dmntchtInitialize();
void dmntchtExit();
Result dmntchtHasCheatProcess(bool* out);
Result dmntchtGetCheatProcessEvent(Event* event);
Result dmntchtForceOpenCheatProcess();
Result dmntchtGetCheatProcessMetadata(DmntCheatProcessMetadata* out_metadata);
Result dmntchtReadCheatProcessMemory(u64 address, void* buffer, size_t size);
}

class GameStateHandler {
   private:
    static constexpr uint64_t SW_TID = 0x0100ABF008968000;
    static constexpr uint64_t SH_TID = 0x01008DB008C2C000;
    
    DmntCheatProcessMetadata m_dmntCheatProcessMetadata;

    bool ensureHasCheatProcess_();

   public:
    GameStateHandler();
    ~GameStateHandler();

    inline bool gameIsRunning() { return ensureHasCheatProcess_(); }
};
