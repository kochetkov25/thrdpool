#include "thrdpool.hpp"

#include <iostream>

namespace TP
{

    ThreadPool::ThreadPool(size_t workers)
    {
        startThreads(workers);   
    }

    ThreadPool::~ThreadPool()
    {
        stop();
    }

    void ThreadPool::startThreads(size_t count)
    {
        m_workers.clear();
        m_terminate.store(false);

        for(size_t i = 0; i < count; i++)
        {
            m_workers.emplace_back([this](){
                workerRoutine();
            });
        }
    }

    void ThreadPool::workerRoutine()
    {
        while(!m_terminate.load())
        {
            auto task = m_tasks.take();
            if(m_terminate.load())
                return;
        
            if(task.has_value())
                (*task)();
        }
    }

    void ThreadPool::submit(Task&& task)
    {
        if(m_terminate.load())
        {
            std::cout<<"You should start ThreadPool to submit on Task!"<<"\n";
            return;
        }
        m_tasks.put(task);
    }

    void ThreadPool::start(size_t workers)
    {
        if(!m_terminate.load())
        {
            std::cout<<"Threads already in work!"<<"\n";
            return;
        }
        m_terminate.store(false);
        startThreads(workers);
    }

    void ThreadPool::join()
    {
        m_terminate.store(true);
        for(auto& worker : m_workers)
        {
            if(worker.joinable())
                worker.join();
        }
    }

    void ThreadPool::stop()
    {
        m_tasks.drop();
        join();
    }
}