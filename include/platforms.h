#ifndef PLATFORMS_H
#define PLATFORMS_H

// Platform specific stuff
#if defined(_WIN64) || defined(_WIN32)
    #define windows
    #define DECLSPEC __declspec(dllexport)
#elif defined __linux__
    #define linux
    #define DECLSPEC __attribute__((visibility("default")))
#endif

#endif // PLATFORMS_H
