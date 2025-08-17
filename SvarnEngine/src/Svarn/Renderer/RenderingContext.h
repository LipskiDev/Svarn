#pragma once

namespace Svarn {

    class RenderingContext {
        public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

}  // namespace Svarn
