#pragma once

#ifdef _WIN32
    #ifdef ENGINE_EXPORT
        #define TRACER_API __declspec(dllexport)
    #else
        #define TRACER_API __declspec(dllimport)
    #endif
#else
    #define TRACER_API __attribute__((visibility("default")))
#endif