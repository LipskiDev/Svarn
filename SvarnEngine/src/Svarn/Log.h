#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <Svarn/Log/Formatters.h>

#include "Core.h"

namespace Svarn {
    class SVARN_API Log {
        public:
        static void Init();

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
};  // namespace Svarn

// Core log macros
#define SV_CORE_TRACE(...) ::Svarn::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SV_CORE_INFO(...) ::Svarn::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SV_CORE_WARN(...) ::Svarn::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SV_CORE_ERROR(...) ::Svarn::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SV_CORE_FATAL(...) ::Svarn::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define SV_TRACE(...) ::Svarn::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SV_INFO(...) ::Svarn::Log::GetClientLogger()->info(__VA_ARGS__)
#define SV_WARN(...) ::Svarn::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SV_ERROR(...) ::Svarn::Log::GetClientLogger()->error(__VA_ARGS__)
#define SV_FATAL(...) ::Svarn::Log::GetClientLogger()->fatal(__VA_ARGS__)
