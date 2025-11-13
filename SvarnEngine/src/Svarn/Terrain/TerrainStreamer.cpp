#include <Svarn/Terrain/TerrainStreamer.h>
#include "Svarn/Core.h"
#include "Svarn/Log.h"

namespace Svarn {
    StreamPlan TerrainStreamer::ComputePlan(const Camera& cam) {
        StreamPlan plan;

        ChunkId chunkId = GetCurrentChunk(cam);

        std::vector<ChunkId> chunkPlan;

        for (int i = -5; i <= 5; i++) {
            for (int j = -5; j <= 5; j++) {
                ChunkId nChunk{chunkId.x + j, chunkId.z + i};
                chunkPlan.push_back(nChunk);
            }
        }

        for (ChunkId chunk : chunkPlan) {
            ChunkMeta meta = GetChunkMeta(chunk);

            if (meta.state == ChunkState::Resident) {
                plan.resident.push_back(chunk);
            } else if (meta.state == ChunkState::Empty) {
                plan.gpuGens.push_back(chunk);
            }
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
        glm::vec2 chunkSize = glm::vec2(m_MetersPerChunk - 1, m_MetersPerChunk - 1);
        return glm::vec2(chunkId.x, chunkId.z) * chunkSize;
    }

    ChunkData TerrainStreamer::GetChunkData(ChunkId id) {
        auto it = m_ChunksData.find(id);
        if (it == m_ChunksData.end()) {
            SV_CORE_ERROR("[GetChunkData] NOT FOUND for id = ({0}, {1})", id.x, id.z);
            SV_CORE_INFO("Current map contents (size) {}:", m_ChunksData.size());
            for (auto& [key, value] : m_ChunksData) {
                SV_CORE_INFO("  stored id = ({0}, {1})", key.x, key.z);
            }
            SV_CORE_ASSERT(false, "Chunk Data Error");
        }

        return it->second;
    }

    void TerrainStreamer::AddChunkData(ChunkId id, std::shared_ptr<Texture> heightMap) {
        ChunkData data;
        data.id = id;
        data.heightmap = heightMap;

        m_ChunksData[id] = data;
    }

    ChunkMeta TerrainStreamer::GetChunkMeta(ChunkId id) {
        auto it = m_ChunksMeta.find(id);

        if (it == m_ChunksMeta.end()) {
            ChunkMeta meta{id};
            m_ChunksMeta[id] = meta;
            return meta;
        }

        return it->second;
    }

    void TerrainStreamer::SetChunkMeta(ChunkId id, ChunkMeta meta) { m_ChunksMeta[id] = meta; }

    TerrainStreamer& GetTerrainStreamer() {
        static TerrainStreamer instance;
        return instance;
    }
}  // namespace Svarn
