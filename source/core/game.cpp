#include "game.hpp"
#include "../util.hpp"

GameStateHandler* GameStateHandler::s_instance = nullptr;

GameStateHandler::GameStateHandler() {
    TRY_THROW(pmdmntInitialize());
    TRY_THROW(pminfoInitialize());
    TRY_THROW(dmntchtInitialize());
    if (R_SUCCEEDED(dmntchtForceOpenCheatProcess())) {
        TRY_THROW(dmntchtGetCheatProcessMetadata(&m_dmntCheatProcessMetadata));
    }
}

GameStateHandler::~GameStateHandler() {
    dmntchtExit();
    pminfoExit();
    pmdmntExit();
}

bool GameStateHandler::ensureHasCheatProcess_() {
    bool hasCheatProcess;
    TRY_THROW(dmntchtHasCheatProcess(&hasCheatProcess));
    if (not hasCheatProcess) {
        if (R_FAILED(dmntchtForceOpenCheatProcess())) {
            return false;
        }
        TRY_THROW(dmntchtGetCheatProcessMetadata(&m_dmntCheatProcessMetadata));
    }
    return m_dmntCheatProcessMetadata.title_id == SW_TID or m_dmntCheatProcessMetadata.title_id == SH_TID;
}
