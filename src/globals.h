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
const uint8_t game_ExtraControllerData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x02, 0x01, 0x00, 0x05,
    0x09, 0x08, 0x04, 0x03, 0x0B, 0x06, 0x07, 0x00, 0x00, 0x00};
const QString game_Custom1ButtonLabel = "Delusion trigger (Left)";
const QString game_Custom2ButtonLabel = "Delusion trigger (Right)";
#endif