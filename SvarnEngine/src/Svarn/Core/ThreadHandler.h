#pragma once

#include <mutex>
#include <queue>

namespace Svarn {
    class ThreadHandler {
        using Job = std::function<void()>;

        explicit ThreadHandler(uint32_t workerCount = std::thread::hardware_concurrency() - 1) { start(workerCount); }

        ~ThreadHandler() { stop(); }

        void enqueue(Job j) {
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                m_Jobs.push(j);
            }

            m_CV.notify_one();
        }

        void stop() {
            {
                std::lock_guard<std::mutex> lock(m_Mutex);
                m_IsRunning = false;
            }

            m_CV.notify_all();
            for (auto& t : m_Workers) {
                if (t.joinable()) t.join();
            }

            m_Workers.clear();
        }

        private:
        void start(uint32_t count) {
            m_IsRunning = true;
            for (uint32_t i = 0; i < count; i++) {
                m_Workers.emplace_back([this] { workerLoop(); });
            }
        }

        void workerLoop() {
            while (true) {
                Job job;
                {
                    std::unique_lock<std::mutex> lock(m_Mutex);
                    m_CV.wait(lock, [this] { return !m_Jobs.empty() || !m_IsRunning; });

                    if (!m_IsRunning && m_Jobs.empty()) return;
                    job = std::move(m_Jobs.front());
                    m_Jobs.pop();
                }
                job();
            }
        }

        std::vector<std::thread> m_Workers;
        std::queue<Job> m_Jobs;
        std::mutex m_Mutex;
        std::condition_variable m_CV;
        bool m_IsRunning = false;
    };
}  // namespace Svarn
