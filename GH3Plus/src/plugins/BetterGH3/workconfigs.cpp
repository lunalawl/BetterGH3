#include "main.h"
#include "window.h"
#include "configAspyr.h"
#include <Windows.h>
#include <iostream>

#include "inicpp.h"

using namespace std;

void WorkDgVoodoo()
{
    // load dgvoodoo config
    // fyi this inicpp will not parse it if the version tag is present due to it not being in a header
    ini::IniFile dgVoodooConfig;
    try
    {
        dgVoodooConfig.load("dgVoodoo.conf");
    }
    catch(...)
    {
        MessageBox(
            NULL,
            L"dgVoodoo config failed to parse.\nTry removing the \"Version\" tag at the top of dgVoodoo.conf",
            L"BetterGH3",
            MB_OK | MB_ICONERROR
        );
        exit(0);
    }

    bool DisableAndPassThru = dgVoodooConfig["DirectX"]["DisableAndPassThru"].as<bool>();

    // dgvoodoo loads before we do, so the game has to restart for the change to take effect, just kill dort game
    if (UseDgVoodoo)
    {
        if (DisableAndPassThru)
        {
            cout << "Need to restart for dgVoodoo" << endl;
            dgVoodooConfig["DirectX"]["DisableAndPassThru"] = false;
            dgVoodooConfig.save("dgVoodoo.conf");
            // show a message so it doesnt look like a phantom crash
            MessageBox(
                NULL,
                L"dgVoodoo is now enabled, launch the game one more time to apply\n\nYou won't see this message again unless you disable it again",
                L"BetterGH3",
                MB_OK | MB_ICONINFORMATION
            );
            exit(0);
        }
        cout << "Using dgVoodoo" << endl;
    }
    else
    {
        if (!DisableAndPassThru)
        {
            cout << "Need to restart for dgVoodoo" << endl;
            dgVoodooConfig["DirectX"]["DisableAndPassThru"] = true;
            dgVoodooConfig.save("dgVoodoo.conf");
            // show a message so it doesnt look like a phantom crash
            MessageBox(
                NULL,
                L"dgVoodoo is now disabled, launch the game one more time to apply\n\nYou won't see this message again unless you enable it again",
                L"BetterGH3",
                MB_OK | MB_ICONINFORMATION
            );
            exit(0);
        }
        cout << "NOT using dgVoodoo" << endl;
    }
}

void WorkGfxConfig()
{
    // get display info
    DEVMODEA dm{};
    dm.dmSize = sizeof(dm);
    EnumDisplaySettingsA(NULL, ENUM_CURRENT_SETTINGS, &dm);
    int w = dm.dmPelsWidth;
    int h = dm.dmPelsHeight;

    cout << "Got display resolution: " << w << "x" << h << endl;
    if (UseDgVoodoo)
    {
        // get the aspect ratio so dgvoodoo scales to 4:3/16:9 accordingly
        double aspect = (double)w / (double)h;
        bool widescreen = (fabs(aspect - (16.0 / 9.0)) < fabs(aspect - (4.0 / 3.0)));

        if (widescreen)
        {
            cout << "Widescreen display detected" << endl;
        }
        else
        {
            cout << "Non-widescreen display detected" << endl;
        }

        // dgvoodoo uses the default res (1280x720, 1024x768) as a base and does all the magic to fit your display
        // i have no clue how it does it, it's magic joel, it's magic
        int aspW = widescreen ? 1280 : 1024;
        int aspH = widescreen ? 720 : 768;
        UpdateAspyrConfig(aspW, aspH);
    }
    else
    {
        // allow manually setting resolution when not using dgvoodoo, help those older machines a bit
        if (ManualResW && ManualResH)
        {
            cout << "Using manual resolution: " << ManualResW << "x" << ManualResH << endl;
            UpdateAspyrConfig(ManualResW, ManualResH);
        }
        else
        {
            UpdateAspyrConfig(w, h);
        }

        // borderless window on non-dgvoodoo
        BorderlessWindowHack();
        cout << "Borderless Window patch applied" << endl;
    }

    cout << "Aspyr config edited" << endl;

    if (UncappedFPS)
    {
        UncappedFPSHack();
        cout << "Uncapped FPS patch applied" << endl;
    }
}
