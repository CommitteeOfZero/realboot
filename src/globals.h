#pragma once

#include <QString>
#include <cstdint>
#include "Windows.h"

const QString game_TechSupportUrl = "https://discord.gg/rq4GGCh";

#if defined(GAME_CHAOSHEADNOAH)
const uint8_t game_ExtraControllerData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x02, 0x01, 0x00, 0x05,
    0x09, 0x08, 0x04, 0x03, 0x0B, 0x06, 0x07, 0x00, 0x00, 0x00};
const QString game_Custom1ButtonLabel = "Delusion trigger (Left)";
const QString game_Custom2ButtonLabel = "Delusion trigger (Right)";
const QString game_ReleaseUrl =
    "https://github.com/CommitteeOfZero/chn-patch/releases";
#if defined(GAME_STEAM)
const QString game_LauncherTitle =
    "CHAOS;HEAD NOAH Overhaul Patch (Steam) Launcher";
const QString game_PatchConfPath = "Committee of Zero/CHNSteam";
const QString game_GameConfPath = "mages_steam/CHAOS;HEAD NOAH";
const QString game_LaunchCommand = "Game.exe";
#else
const QString game_LauncherTitle =
    "CHAOS;HEAD NOAH Overhaul Patch (GoG) Launcher";
const QString game_PatchConfPath = "Committee of Zero/CHNGOG";
const QString game_GameConfPath = "Mages Inc/CHAOS;HEAD NOAH";
const QString game_LaunchCommand = "Game_GOG.exe";
#define IPC_ENABLED
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"CHAOS;HEAD NOAH";
#endif
#endif

#if defined(GAME_STEINSGATE)
const uint8_t game_ExtraControllerData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x02, 0x01, 0x00, 0x05,
    0x09, 0x08, 0x04, 0x03, 0x0B, 0x06, 0x07, 0x00, 0x00, 0x00};
const QString game_Custom1ButtonLabel = "Phone trigger / Zoom in";
const QString game_Custom2ButtonLabel = "Phone trigger / Zoom out";
const QString game_ReleaseUrl =
    "https://github.com/CommitteeOfZero/sghd-patch/releases";
#if defined(GAME_STEAM)
const QString game_LauncherTitle =
    "STEINS;GATE Improvement Patch (Steam) Launcher";
const QString game_PatchConfPath = "Committee of Zero/SGHD";
const QString game_GameConfPath = "mages_steam/STEINS;GATE/eng";
const QString game_LaunchCommand = "Game.exe mages_sgate";
#else
const QString game_LauncherTitle = "STEINS;GATE Improvement Patch Launcher";
const QString game_PatchConfPath = "Committee of Zero/SGHD";
const QString game_GameConfPath = "mages_dmm/STEINS;GATE";
const QString game_LaunchCommand = "Game.exe mages_sgate";
#define IPC_ENABLED
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"STEINS;GATE";
#endif
#endif

#if defined(GAME_ROBOTICSNOTESELITE)
const uint8_t game_ExtraControllerData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x02, 0x01, 0x00, 0x05,
    0x09, 0x08, 0x04, 0x03, 0x0B, 0x06, 0x07, 0x00, 0x00, 0x00};
const QString game_Custom1ButtonLabel = "Unused";
const QString game_Custom2ButtonLabel = "Unused";
const QString game_ReleaseUrl =
    "https://github.com/CommitteeOfZero/rne-patch/releases";
#if defined(GAME_STEAM)
const QString game_LauncherTitle =
    "ROBOTICS;NOTES ELITE Improvement Patch (Steam) Launcher";
const QString game_PatchConfPath = "Committee of Zero/RNESteam";
const QString game_GameConfPath = "mages_steam/Robotics Notes ELITE/eng";
const QString game_LaunchCommand = "Game.exe roboticsnotese EN";
#else
const QString game_LauncherTitle =
    "ROBOTICS;NOTES ELITE Improvement Patch Launcher";
const QString game_PatchConfPath = "Committee of Zero/RNE";
const QString game_GameConfPath = "mages_dmm/Robotics Notes ELITE/eng";
const QString game_LaunchCommand = "Game.exe roboticsnotese EN";
#define IPC_ENABLED
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"ROBOTICS;NOTES ELITE";
#endif
#endif

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
const QString game_PatchConfPath = "Committee of Zero/CCGOG";
const QString game_GameConfPath = "Mages Inc/CHAOS CHILD";
const QString game_LaunchCommand = "Game_GOG.exe chaoschild";
#define IPC_ENABLED
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"CHAOS;CHILD";
#endif
#endif

#if defined(GAME_STEINSGATE0)
const uint8_t game_ExtraControllerData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x02, 0x01, 0x00, 0x05,
    0x09, 0x08, 0x04, 0x03, 0x0B, 0x06, 0x07, 0x00, 0x00, 0x00};
const QString game_Custom1ButtonLabel = "RINE trigger / Zoom in";
const QString game_Custom2ButtonLabel = "RINE trigger / Zoom out";
const QString game_ReleaseUrl =
    "https://github.com/CommitteeOfZero/zero-patch/releases";
#if defined(GAME_STEAM)
const QString game_LauncherTitle =
    "STEINS;GATE 0 Improvement Patch (Steam) Launcher";
const QString game_PatchConfPath = "Committee of Zero/SG0Steam";
const QString game_GameConfPath = "mages_steam/STEINS GATE 0/eng";
const QString game_LaunchCommand = "Game.exe steinsgate0 EN";
#else
const QString game_LauncherTitle = "STEINS;GATE 0 English Launcher";
const QString game_PatchConfPath = "Committee of Zero/SG0";
const QString game_GameConfPath = "mages_dmm/STEINS GATE 0";
const QString game_LaunchCommand = "Game.exe steinsgate0";
#define IPC_ENABLED
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"STEINS;GATE 0";
#endif
#endif

#if defined(GAME_ROBOTICSNOTESDASH)
const uint8_t game_ExtraControllerData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x02, 0x01, 0x00, 0x05,
    0x09, 0x08, 0x04, 0x03, 0x0B, 0x06, 0x07, 0x00, 0x00, 0x00};
const QString game_Custom1ButtonLabel = "Unused";
const QString game_Custom2ButtonLabel = "Unused";
const QString game_ReleaseUrl =
    "https://github.com/CommitteeOfZero/rnd-patch/releases";
#if defined(GAME_STEAM)
const QString game_LauncherTitle =
    "ROBOTICS;NOTES DaSH Improvement Patch (Steam) Launcher";
const QString game_PatchConfPath = "Committee of Zero/RNDSteam";
const QString game_GameConfPath = "mages_steam/Robotics Notes DASH/eng";
const QString game_LaunchCommand = "Game.exe roboticsnotesd EN";
#else
const QString game_LauncherTitle =
    "ROBOTICS;NOTES ELITE Improvement Patch Launcher";
const QString game_PatchConfPath = "Committee of Zero/RND";
const QString game_GameConfPath = "mages_dmm/Robotics Notes DASH/eng";
const QString game_LaunchCommand = "Game.exe roboticsnotesd EN";
#define IPC_ENABLED
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"ROBOTICS;NOTES DaSH";
#endif
#endif

#if defined(GAME_ANONYMOUSCODE)
const uint8_t game_ExtraControllerData[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x28, 0x02, 0x01, 0x00, 0x05,
    0x09, 0x08, 0x04, 0x03, 0x0B, 0x06, 0x07, 0x00, 0x00, 0x00};
const QString game_Custom1ButtonLabel = "Unused";
const QString game_Custom2ButtonLabel = "Unused";
const QString game_ReleaseUrl =
    "https://github.com/CommitteeOfZero/ac-patch/releases";
#if defined(GAME_STEAM)
const QString game_LauncherTitle =
    "ANONYMOUS;CODE Improvement Patch (Steam) Launcher";
const QString game_PatchConfPath = "Committee of Zero/ACSteam";
const QString game_GameConfPath = "mages_steam/ANONYMOUS;CODE";
const QString game_LaunchCommand = "game.exe";
#else
const QString game_LauncherTitle = "ANONYMOUS;CODE Improvement Patch Launcher";
const QString game_PatchConfPath = "Committee of Zero/AC";
const QString game_GameConfPath = "mages_dmm/ANONYMOUS;CODE";
const QString game_LaunchCommand = "game.exe";
#define IPC_ENABLED
const uint32_t game_ipcIn = 0x23F4F1DE;
const uint32_t game_ipcOut = 0xF0973746;
const wchar_t game_ipcName[] = L"ANONYMOUS;CODE";
#endif
#endif