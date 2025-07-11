#pragma once

#ifdef SV_PLATFORM_WINDOWS
    #ifdef SV_BUILD_DLL
        #define SVARN_API __declspec(dllexport)
    #else
        #define SVARN_API __declspec(dllimport)
    #endif
#else
    #define SVARN_API __attribute__((visibility("default")))
#endif