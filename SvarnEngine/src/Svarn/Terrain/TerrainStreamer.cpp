#include <Svarn/Terrain/TerrainStreamer.h>

namespace Svarn {
    StreamPlan TerrainStreamer::ComputePlan(const Camera& cam) {
        StreamPlan plan;

        ChunkId chunkId = GetCurrentChunk(cam);

        ChunkMeta meta = chunksData[chunkId];

        if (meta.state == ChunkState::Resident) {
            plan.resident.push_back(chunkId);
        } else if (meta.state == ChunkState::Empty) {
            plan.gpuGens.push_back(chunkId);
        }
        return plan;
    }

    ChunkId TerrainStreamer::GetCurrentChunk(const Camera& cam) {
        const glm::vec3 cameraPosition = cam.GetPosition();
        const int cameraChunkX = (int)std::floor(cameraPosition.x / m_MetersPerChunk);
        const int cameraChunkZ = (int)std::floor(cameraPosition.z / m_MetersPerChunk);
        return ChunkId(cameraChunkX, cameraChunkZ);
    }

    glm::vec2 TerrainStreamer::GetChunkOffset(const ChunkId& chunkId) {
        glm::vec2 chunkSize = glm::vec2(m_MetersPerChunk, m_MetersPerChunk);
        return glm::vec2(chunkId.x, chunkId.z) * chunkSize;
    }

    TerrainStreamer& GetTerrainStreamer() {
        static TerrainStreamer instance;
        return instance;
    }
}  // namespace Svarn
