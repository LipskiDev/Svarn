#pragma once

#include "Svarn/Renderer/Mesh.h"
namespace Svarn {
    class Primitives {
        public:
        static std::shared_ptr<Mesh> Sphere(int radius, int slices, int stacks);
        static std::shared_ptr<Mesh> Quad();
        static std::shared_ptr<Mesh> Grid(int width, int height, int size);
    };
}  // namespace Svarn
