#include <svpch.h>

#include <Svarn/Renderer/Primitives.h>
#include "Svarn/Core.h"
#include "Svarn/Renderer/Buffer.h"
#include "Svarn/Renderer/VertexArray.h"

namespace Svarn {
    std::shared_ptr<Mesh> Primitives::Sphere(int radius, int slices, int stacks) {
        if (radius <= 0.0f || slices < 3 || stacks < 2) {
            SV_ASSERT(false, "Cannot create a sphere Mesh.");
        }
        const int cols = slices + 1;  // +1 to duplicate the seam for proper UVs
        const int rows = stacks + 1;  // include both poles

        std::vector<Vertex> verts;
        std::vector<uint32_t> indices;

        verts.reserve(cols * rows);
        indices.reserve(slices * stacks * 6);

        const float PI = 3.14159265358979323846f;

        // Build vertices
        for (int y = 0; y < rows; ++y) {
            float v = static_cast<float>(y) / stacks;  // [0,1]
            float theta = v * PI;                      // [0, pi] (lat)
            float sinT = std::sin(theta);
            float cosT = std::cos(theta);

            for (int x = 0; x < cols; ++x) {
                float u = static_cast<float>(x) / slices;  // [0,1], seam duplicated at x==slices
                float phi = u * 2.0f * PI;                 // [0, 2pi] (lon)
                float sinP = std::sin(phi);
                float cosP = std::cos(phi);

                // Unit sphere direction
                float nx = sinT * cosP;
                float ny = cosT;
                float nz = sinT * sinP;

                // Position
                Vertex vert;
                vert.position = glm::vec3(radius * nx, radius * ny, radius * nz);

                // Normal (outward)
                vert.normal = glm::vec3(nx, ny, nz);

                // UVs
                vert.uv = glm::vec2(u, 1.0f - v);

                verts.push_back(vert);

                // Analytic tangent: derivative wrt φ
                float tx = -sinP;
                float ty = 0.0f;
                float tz = cosP;
                // This is already normalized

                vert.tangent = glm::vec4(tx, ty, tz, 1.0);

                // Analytic bitangent: derivative wrt θ
                float bx = cosT * cosP;
                float by = -sinT;
                float bz = cosT * sinP;
                float lenB = std::sqrt(bx * bx + by * by + bz * bz);
                if (lenB > 0.0f) {
                    bx /= lenB;
                    by /= lenB;
                    bz /= lenB;
                }

                vert.bitangent = glm::vec4(bx, by, bz, 1.0);
            }
        }

        // Build triangle indices (counter-clockwise when looking from outside)
        for (int y = 0; y < stacks; ++y) {
            for (int x = 0; x < slices; ++x) {
                uint32_t i0 = static_cast<uint32_t>(y * cols + x);
                uint32_t i1 = static_cast<uint32_t>(y * cols + x + 1);
                uint32_t i2 = static_cast<uint32_t>((y + 1) * cols + x);
                uint32_t i3 = static_cast<uint32_t>((y + 1) * cols + x + 1);

                // skip degenerate tris at exact poles? Not needed with this grid;
                // they collapse naturally as sin(theta)=0 at poles.

                // two triangles for each quad
                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i1);
                indices.push_back(i1);
                indices.push_back(i2);
                indices.push_back(i3);
            }
        }
        std::shared_ptr<Mesh> mesh;
        mesh.reset(Mesh::Create(verts, indices));
        return mesh;
    }

    std::shared_ptr<Mesh> Primitives::FullscreenQuad() {
        std::vector<Vertex> verts;
        std::vector<uint32_t> indices;

        verts.reserve(4);
        indices.reserve(6);

        // Shared attributes
        const glm::vec3 N = glm::vec3(0.f, 0.f, 1.f);       // facing +Z
        const glm::vec4 T = glm::vec4(1.f, 0.f, 0.f, 1.f);  // tangent +X
        const glm::vec4 B = glm::vec4(0.f, 1.f, 0.f, 1.f);  // bitangent +Y

        // Interleaved vertex attributes: [x, y, z] + [u, v]
        // NDC positions cover the whole screen; UVs in [0,1]
        verts.push_back(Vertex{
            glm::vec3(-1.f, -1.f, 0.f), N, glm::vec2(0.f, 0.f), T, B  // bottom-left
        });
        verts.push_back(Vertex{
            glm::vec3(1.f, -1.f, 0.f), N, glm::vec2(1.f, 0.f), T, B  // bottom-right
        });
        verts.push_back(Vertex{
            glm::vec3(1.f, 1.f, 0.f), N, glm::vec2(1.f, 1.f), T, B  // top-right
        });
        verts.push_back(Vertex{
            glm::vec3(-1.f, 1.f, 0.f), N, glm::vec2(0.f, 1.f), T, B  // top-left
        });

        // Two CCW triangles (as seen from +Z)
        indices = {0, 1, 2, 0, 2, 3};

        std::shared_ptr<Mesh> mesh;
        mesh.reset(Mesh::Create(verts, indices));
        return mesh;
    }
}  // namespace Svarn
