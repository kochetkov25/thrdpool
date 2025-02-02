#pragma once

#include <condition_variable>
#include <optional>
#include <cassert>
#include <cstdlib>
#include <atomic>
#include <mutex>
#include <deque>

namespace TP
{

    template <typename T>
    class BlockingQueue
    {
        public:
            /*producer*/
            void put(T value)
            {
                std::lock_guard<std::mutex> grd(m_mtx);
                m_buff.push_back(std::move(value));

                m_notEmpty.notify_one();
            }
            /*consumer*/
            std::optional<T> take()
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                while(m_buff.empty())
                {
                    if(m_terminate.load())
                        return std::nullopt;
                    
                    m_notEmpty.wait(lock);
                }
                return takeLocked();
            }

            void drop()
            {
                m_terminate.store(true);
                m_notEmpty.notify_all();
            }

        private:
            std::optional<T> takeLocked()
            {
                assert(!m_buff.empty());

                //T front{std::move(m_buff.front())};
                std::optional front = m_buff.front();

                m_buff.pop_front();
                return front;
            }

        std::deque<T> m_buff; // GUARDED BY m_mtx
        std::mutex m_mtx;

        std::atomic<bool> m_terminate = false;

        std::condition_variable m_notEmpty;
    };

}