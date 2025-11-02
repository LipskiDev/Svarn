#pragma once

#include "Svarn/Scene/Camera.h"
namespace Svarn {

    struct ChunkId {
        int32_t x;
        int32_t z;
    };

    inline bool operator==(const ChunkId& a, const ChunkId& b) noexcept { return a.x == b.x && a.z == b.z; }

    struct ChunkIdHash {
        std::size_t operator()(const ChunkId& id) const noexcept {
            const std::uint64_t hi = static_cast<std::uint64_t>(static_cast<std::uint32_t>(id.x));
            const std::uint64_t lo = static_cast<std::uint64_t>(static_cast<std::uint32_t>(id.z));
            const std::uint64_t key = (hi << 32) | lo;
            return std::hash<std::uint64_t>{}(key);
        }
    };

    enum ChunkState { Unloaded, Requested, LoadingCPU, ReadyCPU, Uploading, GeneratingGPU, Resident, Evicting, Empty };

    struct ChunkMeta {
        ChunkId id{};
        ChunkState state = ChunkState::Empty;
        int layer = -1;
        uint64_t lastUsedFrame = 0;
        glm::vec3 origin{0, 0, 0};
    };

    struct StreamPlan {
        std::vector<ChunkId> resident;
        std::vector<ChunkId> cpuLoads;
        std::vector<ChunkId> gpuGens;
        std::vector<ChunkId> evict;
    };

    class TerrainStreamer {
        public:
        StreamPlan ComputePlan(const Camera& cam);
        ChunkId GetCurrentChunk(const Camera& cam);
        glm::vec2 GetChunkOffset(const ChunkId& chunkId);

        private:
        int m_MetersPerChunk = 256;
        std::unordered_map<ChunkId, ChunkMeta, ChunkIdHash> chunksData;
    };

    SVARN_API TerrainStreamer& GetTerrainStreamer();
}  // namespace Svarn
