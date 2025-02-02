#pragma once

#include "blocking_queue.hpp"

#include <thread>
#include <vector>
#include <atomic>
#include <functional>

namespace TP
{
    using Task = std::function<void()>;

    class ThreadPool
    {
        public:
            ThreadPool(size_t workers);
            ~ThreadPool();

            /*non copyable*/
            ThreadPool(const ThreadPool&) = delete;
            ThreadPool& operator=(const ThreadPool&) = delete;

            /*non movable*/
            ThreadPool(ThreadPool&&) = delete;
            ThreadPool& operator=(ThreadPool&&) = delete;

            void start(size_t workers);

            void submit(Task&& task);

            void stop();
        private:

            void startThreads(size_t count);
            void workerRoutine();
            void join();

            std::vector<std::thread> m_workers;
            BlockingQueue<Task> m_tasks; // THREAD SAFE
            std::atomic<bool> m_terminate;
    };

} 
