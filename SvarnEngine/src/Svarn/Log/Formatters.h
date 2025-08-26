#pragma once

#include <glm/glm.hpp>
#include "spdlog/fmt/bundled/base.h"
#include <fmt/format.h>
#include <spdlog/fmt/bundled/format.h>

namespace fmt {
    template <>
    struct formatter<glm::mat4> {
        // no custom format spec for now
        constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

        template <typename FormatContext>
        auto format(const glm::mat4& m, FormatContext& ctx) const {
            // Print as rows (human-friendly), GLM stores columns
            return format_to(ctx.out(),
                             "\n[{: .3f} {: .3f} {: .3f} {: .3f}]\n"
                             "[{: .3f} {: .3f} {: .3f} {: .3f}]\n"
                             "[{: .3f} {: .3f} {: .3f} {: .3f}]\n"
                             "[{: .3f} {: .3f} {: .3f} {: .3f}]",
                             m[0][0], m[1][0], m[2][0], m[3][0], m[0][1], m[1][1], m[2][1], m[3][1], m[0][2], m[1][2], m[2][2], m[3][2], m[0][3],
                             m[1][3], m[2][3], m[3][3]);
        }
    };

    template <>
    struct formatter<glm::vec4> {
        constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

        template <typename FormatContext>
        auto format(const glm::vec4& m, FormatContext& ctx) const {
            // Print as rows (human-friendly), GLM stores columns
            return format_to(ctx.out(), "\n[{: .3f} {: .3f} {: .3f} {: .3f}]\n", m[0], m[1], m[2], m[3]);
        }
    };
};  // namespace fmt
