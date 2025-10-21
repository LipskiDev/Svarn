#pragma once

#include <mutex>
#include <queue>

namespace Svarn {
    class ThreadHandler {
        public:
        using Job = std::function<void()>;

        explicit ThreadHandler(uint32_t workerCount = std::thread::hardware_concurrency() - 1) { start(workerCount); }

        ~ThreadHandler() { stop(); }

        void enqueue(Job j);

        void stop();

        private:
        void workerLoop();
        void start(uint32_t count);

        std::vector<std::thread> m_Workers;
        std::queue<Job> m_Jobs;
        std::mutex m_Mutex;
        std::condition_variable m_CV;
        bool m_IsRunning = false;
    };

    ThreadHandler& GetThreadHandler();

}  // namespace Svarn
