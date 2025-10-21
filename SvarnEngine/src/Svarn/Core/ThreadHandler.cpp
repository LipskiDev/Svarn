#include "Svarn/Core/ThreadHandler.h"
#include <svpch.h>

namespace Svarn {

    void ThreadHandler::enqueue(Job j) {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_Jobs.push(j);
        }

        m_CV.notify_one();
    }

    void ThreadHandler::start(uint32_t count) {
        m_IsRunning = true;
        for (uint32_t i = 0; i < count; i++) {
            m_Workers.emplace_back([this] { workerLoop(); });
        }
    }

    void ThreadHandler::stop() {
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

    void ThreadHandler::workerLoop() {
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

    ThreadHandler& GetThreadHandler() {
        static ThreadHandler instance(std::thread::hardware_concurrency() - 1);
        return instance;
    }
}  // namespace Svarn
