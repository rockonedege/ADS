#pragma once

#ifndef _ADSLIB_H_
#pragma message("must #include <AdsLib.h> before this file")
#endif

namespace appinfo
{
    struct {
        const int major = 0;
        const int minor = 0;
        const int patch = 0;
        const int build = 1;
        const char* to_string = "0.0.0.1";
        const wchar_t* to_wstring = L"0.0.0.1";
    } const version;

    const char* name = "Ads";

    const char* meta_text =
        "==============================================================================\n"
        "\tAds version 0.0.0.1 for Windows x86, " __DATE__ ".\n"
#if defined(_DEBUG)
        "\n\t!!THIS IS A DEBUG BUILD, NOT FOR PRODUCTION USAGES. !!\n"
#endif
        "==============================================================================\n";
}

namespace test_twincat_server
{
    struct remote_target {
        const char* ip_v4;
        AmsAddr server;
    };

    // example for TWinCAT2
    const remote_target tc2 = {
        "10.8.9.36",                    // ip v4
        {
            { 10, 0, 96, 139, 1, 1 },   // AMS Net Id
            AMSPORT_R0_PLC              // AMS Port number
        }
    };

    // example for TWinCAT3
    const remote_target tc3 = {
        "192.168.0.232",                 // ip v4
        {
            { 192, 168, 0, 231, 1, 1 },  // AMS Net Id
            AMSPORT_R0_PLC_TC3           // AMS Port number
        }
    };

    const remote_target& tc = tc2;
}
