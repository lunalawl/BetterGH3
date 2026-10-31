#include "configAspyr.h"
#include <Windows.h>
#include <iostream>
#include <shlobj.h>

#include "pugixml.hpp"

using namespace std;

bool FreshAspyrConfig;
bool FreshAspyrConfigLogged;

void UpdateAspyrConfig(int width, int height)
{
    // get the path
    char localAppData[MAX_PATH];

    if (FAILED(SHGetFolderPathA(
        NULL,
        CSIDL_LOCAL_APPDATA,
        NULL,
        SHGFP_TYPE_CURRENT,
        localAppData)))
    {
        return;
    }

    std::string configPath = std::string(localAppData) +
        "\\Aspyr\\Guitar Hero III\\AspyrConfig.xml";

    // init
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(configPath.c_str());

    // declaration and tags
    pugi::xml_node declaration = doc.prepend_child(pugi::node_declaration);
    declaration.append_attribute("version") = "1.0";
    declaration.append_attribute("encoding") = "utf-8";

    // create root if we're making a fresh file
    if (!result)
    {
        doc.append_child("r");
    }

    pugi::xml_node root = doc.child("r");

    // update setting
    auto ForceXmlSetting = [&](const char* id, int value)
        {
            // edit options that already exist
            for (pugi::xml_node node : root.children("s"))
            {
                if (std::string(node.attribute("id").value()) == id)
                {
                    node.text() = value;
                    return;
                }
            }

            // add the option if it doesn't exist already
            pugi::xml_node node = root.append_child("s");
            node.append_attribute("id") = id;
            node.text() = value;

            // if we have to add any new setting just assume the config didnt exist in the first place
            // i reeeallly hate this but i couldn't get anything else to work
            // https://tenor.com/view/taiho-shichau-zo-teehee-anime-youre-under-arrest-ova-gif-21342147
            if (!FreshAspyrConfigLogged)
            {
                // this is what i call "lawlslop"
                FreshAspyrConfig = true;
                cout << "Aspyr config does not exist" << endl;
                FreshAspyrConfigLogged = true;
            }
        };

    // options to force
    ForceXmlSetting("Video.Width", width);
    ForceXmlSetting("Video.Height", height);
    // max out graphics if we're launching for the first time, gh3 likes to not do this
    if (FreshAspyrConfig)
    {
        ForceXmlSetting("Options.GraphicsQuality", 0); // 0 = high lod
        ForceXmlSetting("Options.Crowd", 1);
        ForceXmlSetting("Options.Physics", 1);
        ForceXmlSetting("Options.Flares", 1);
    }
    ForceXmlSetting("Options.FrontRowCamera", 0); // fuck front row camera, fuck it

    doc.save_file(configPath.c_str(), "\t", pugi::format_default,
        pugi::encoding_utf8);
}
