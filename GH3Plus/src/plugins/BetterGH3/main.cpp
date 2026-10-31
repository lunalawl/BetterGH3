#include "main.h"
//#include "getparams.h"
#include "workconfigs.h"
#include <Windows.h>
#include <filesystem>
#include <iostream>

#include "inicpp.h"

using namespace std;
namespace fs = std::filesystem;

void RunChecks()
{
    // -- CHECK 1: can we write files --
    // untested but i'm pretty sure
    fs::path TestFile = "test";
    std::ofstream ofs(TestFile);
    if (ofs.is_open())
    {
        ofs.close();
        fs::remove(TestFile);
    }
    else
    {
        MessageBox(
            NULL,
            L"BetterGH3 cannot run in this directory because it requires administrative privelages. Please move it somewhere that doesn't.\n\n(i.e. \"C:/Games/Guitar Hero III\").",
            L"BetterGH3",
            MB_OK | MB_ICONERROR
        );
        exit(0);
    }

    // -- CHECK 2: are we actually using bettergh3 --
    // thought about checking for more files but eh this seems like enough
    if (!fs::exists("GH3config.ini"))
    {
        MessageBox(
            NULL,
            L"This plugin is intended only for use with BetterGH3.",
            L"BetterGH3.dll Error",
            MB_OK | MB_ICONERROR
        );
        exit(0);
    }

    // -- CHECK 3: registry for previous installs --
    HKEY hKey;
    LSTATUS status = RegOpenKeyExW(
        HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\WOW6432Node\\Aspyr\\Guitar Hero III",
        0,
        KEY_READ,
        &hKey
    );
    DWORD dwType = 0;
    // string that gets added by the gh3 installer
    status = RegQueryValueExW(hKey, L"Path", nullptr, &dwType, nullptr, nullptr);
    RegCloseKey(hKey);

    if (status == ERROR_SUCCESS)
    {
        MessageBox(
            NULL,
            L"A previous installation of Gutar Hero III was detected on this system.\n\nNavigate to \"_Optional Fixes\" and run \"Fix startup issues.bat\".",
            L"BetterGH3",
            MB_OK | MB_ICONERROR
        );
        exit(0);
    }

    // -- CHECK 4: path length --
    // i would like to see someone try to fix this for real, oh well, guess there's nothin' i can do about it
    size_t PathLength = fs::current_path().string().length();
    if (PathLength > 80)
    {
        MessageBox(
            NULL,
            L"The file path for Guitar Hero III is too long. Please move the game someplace with a shorter path.\n\n(i.e. \"C:/Games/Guitar Hero III\")",
            L"BetterGH3",
            MB_OK | MB_ICONERROR
        );
        exit(0);
    }
}

// to resolve externals
// 
// config options
bool UseDgVoodoo;
bool UncappedFPS;
int ManualResW;
int ManualResH;
// indev only
bool BetterConsole;
bool ShowStartupWarn;

void ParseIni()
{ 
    ini::IniFile GH3config;
    GH3config.load("GH3config.ini");
    try
    {
        UseDgVoodoo = GH3config["Graphics"]["UseDgVoodoo"].as<bool>();
        UncappedFPS = GH3config["Graphics"]["UncappedFPS"].as<bool>();
        ManualResW = GH3config["Graphics"]["ManualResW"].as<int>();
        ManualResH = GH3config["Graphics"]["ManualResH"].as<int>();
        // indev only
        BetterConsole = GH3config["IndevOnly"]["BetterConsole"].as<bool>();
        ShowStartupWarn = GH3config["IndevOnly"]["ShowStartupWarn"].as<bool>();
    }
    catch(...)
    {
        // fail if we can't load options
        MessageBox(
            NULL,
            L"GH3config.ini is either corrupt or an option is missing.\nPlease re-install BetterGH3.",
            L"BetterGH3",
            MB_OK | MB_ICONERROR
        );
        exit(0);
    }
}

// i want to make this actually show printfs from q script, a girl can dream
// warehouse logs to this lol, coincidence
void EnableConsole()
{
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONOUT$", "w", stderr);
    SetConsoleTitleA("BetterGH3 Indev (Debug)");
}

void ApplyHack()
{
    RunChecks();

    ParseIni();

    if (BetterConsole)
    {
        EnableConsole();
    }

    // surprise tool that'll help us later
    // edit: i did this like 6 months ago and i have done jack, i'm just gonna remove it for now
    //if (isWine())
    //{
    //    cout << "Running under Wine/Proton" << endl;
    //}
    //else
    //{
    //    cout << "Running on Windows, initializing copilot" << endl;  // lawl
    //}

    if (fs::exists("dgVoodoo.conf"))
    {
        WorkDgVoodoo();
    }
    else
    // exclusive fullscreen jumpscare
    {
        MessageBox(
            NULL,
            L"dgVoodoo.conf is not present, it will launch with its default settings.",
            L"BetterGH3",
            MB_OK | MB_ICONEXCLAMATION
        );
    }
    
    WorkGfxConfig();

    // indev only
    // i have this here so i can read the console output, not super necessary since the trust system is already in place lawl
    if (ShowStartupWarn)
    {
        MessageBox(
            NULL,
            L"This is an Indev build of BetterGH3.\nDO NOT DISTRIBUTE!\n\n<3 2026 lunalawl & co // NeverHax",
            L"BetterGH3 Indev",
            MB_OK | MB_ICONEXCLAMATION
        );
    }

    // we out this bitch
    cout << "Starting game..." << endl;
}
