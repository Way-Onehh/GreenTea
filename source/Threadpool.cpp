#include <GreenTea/Threadpool.hpp>
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
namespace GreenTea 
{
    Threadpool::Threadpool(int thread_num)
    {
        for (size_t i = 0; i < thread_num; ++i) {    
            workers.emplace_back([this]  {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(dq_mtx);
  
                        dq_cv.wait(lock,  [this] { return !is_run.load(std::memory_order_acquire) || !dq.empty();  });

                        if (!is_run.load(std::memory_order_acquire) && dq.empty())  return;
                        if (dq.empty())  continue;

                        task = std::move(dq.front()); 
                        dq.pop_front();
                    }
                    task();
                }
            });
        }
    }
    
    Threadpool::~Threadpool() {
        {
            std::lock_guard lock(dq_mtx);
            std::lock_guard lock2(pq_mtx);
            is_run.store(false); 
        }
        dq_cv.notify_all(); 
        pq_cv.notify_all(); 
        for (auto& worker : workers) {
            if (worker.joinable())  worker.join(); 
        }
    }

    void Threadpool::submit(std::function<void()> task)
    {
        std::unique_lock ul(dq_mtx);
        dq.push_back(task);
        dq_cv.notify_one();
    }

    void Threadpool::run()
    {
        is_run.store(true,std::memory_order_release);
        while (is_run.load(std::memory_order_acquire)) {
            long long wait_time_num = 0;
            std::function<void()> task_to_execute;
            
            auto now = std::chrono::steady_clock::now();
            int64_t now_num = std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count(); 
            //status 1 计算任务剩余时间 或者 提交任务
            {
                std::unique_lock<std::mutex> lock(pq_mtx);
                if(pq.empty()) continue;
                const auto & next_task = pq.top();
                if (now_num >= next_task.time_point_num)  {
                    task_to_execute = next_task.task; 
                    pq.pop();
                } else {
                    wait_time_num = next_task.time_point_num  - now_num;
                }
            }
            
            //status 2 休眠剩余时间可唤醒 或者 执行任务
            if (task_to_execute) {
                std::unique_lock<std::mutex> lock(dq_mtx);
                dq.push_back(std::move(task_to_execute));
                dq_cv.notify_one();
            } else {
                std::unique_lock<std::mutex> lock(pq_mtx);
                pq_cv.wait_for(lock, std::chrono::nanoseconds(wait_time_num));
            }
        }
    }

    void Threadpool::close()
    {
        is_run.store(false,std::memory_order_release);
    }
}

