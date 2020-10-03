#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define ZIGZAG_WINDOWS
#elif __APPLE__
    #define ZIGZAG_MAC_OSX
    #define ZIGZAG_UNIX_LIKE
#elif __linux__
    #define ZIGZAG_LINUX
    #define ZIGZAG_UNIX_LIKE
#endif


#ifdef ZIGZAG_WINDOWS
    #define ZIGZAG_EXPORT extern "C" __declspec(dllexport)
    #define ZIGZAG_IMPORT extern "C" __declspec(dllimport)
#else
    #define ZIGZAG_EXPORT
    #define ZIGZAG_IMPORT
#endif
