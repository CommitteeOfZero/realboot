#pragma once

#include <QString>
#include <cstdint>
#include "Windows.h"

const QString game_TechSupportUrl = "https://discord.gg/rq4GGCh";

#if defined(GAME_CHAOSCHILD)
const QString game_LauncherTitle = "Chaos;Child English Launcher";
const QString game_PatchConfPath = "Committee of Zero/CC";
const QString game_GameConfPath = "mages_dmm/CHAOS CHILD";
const QString game_LaunchCommand = "Game.exe chaoschild";
const QString game_ReleaseUrl =
    "https://github.com/CommitteeOfZero/cc-patch/releases";
const bool game_ipcEnabled = true;
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"CHAOS;CHILD";
#endif