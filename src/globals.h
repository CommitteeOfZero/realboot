#pragma once

#include <QString>
#include <cstdint>
#include "Windows.h"

const QString game_TechSupportUrl = "https://discord.gg/rq4GGCh";

#if defined(GAME_CHAOSCHILD)
const uint8_t game_ExtraControllerData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x02, 0x01, 0x00, 0x05,
    0x09, 0x08, 0x04, 0x03, 0x0B, 0x06, 0x07, 0x00, 0x00, 0x00};
const QString game_Custom1ButtonLabel = "Delusion trigger (Left)";
const QString game_Custom2ButtonLabel = "Delusion trigger (Right)";
const QString game_ReleaseUrl =
    "https://github.com/CommitteeOfZero/cc-patch/releases";
#if defined(GAME_STEAM)
const QString game_LauncherTitle = "CHAOS;CHILD Steam Patch Launcher";
const QString game_PatchConfPath = "Committee of Zero/CCSteam";
const QString game_GameConfPath = "mages_steam/CHAOS CHILD/eng";
const QString game_LaunchCommand = "Game.exe chaoschild EN";
#else
const QString game_LauncherTitle = "CHAOS;CHILD English Launcher";
const QString game_PatchConfPath = "Committee of Zero/CC";
const QString game_GameConfPath = "mages_dmm/CHAOS CHILD";
const QString game_LaunchCommand = "Game.exe chaoschild";
#define IPC_ENABLED
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"CHAOS;CHILD";
#endif
#endif