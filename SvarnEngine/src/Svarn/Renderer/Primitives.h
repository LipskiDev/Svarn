#pragma once

#include "Svarn/Renderer/Mesh.h"
namespace Svarn {
    class Primitives {
        public:
        static Mesh* Sphere(int radius, int slices, int stacks);
    };
}  // namespace Svarn
