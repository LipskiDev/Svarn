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

#ifdef SV_ENABLE_ASSERTS
#define SV_ASSERT(x, ...)                                   \
    {                                                       \
        if (!(x)) {                                         \
            SV_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
        }                                                   \
    }
#define SV_CORE_ASSERT(x, ...)                                   \
    {                                                            \
        if (!(x)) {                                              \
            SV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
        }                                                        \
    }
#else
#define SV_ASSERT(x, ...)
#define SV_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SV_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)
