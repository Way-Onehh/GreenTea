#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <atomic>
#include <chrono>
#include <deque>
#include <functional>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

namespace GreenTea {
class Threadpool final{
public:
    Threadpool(int threadNum = std::thread::hardware_concurrency());
    
    ~Threadpool();

    template<typename Duration>
    void submit_for(std::function<void()>&&task, Duration delay)
    {
        auto now = std::chrono::steady_clock::now();
        auto target_time_point = now + delay;
        submit_until(std::forward<std::function<void()>>(task),target_time_point);
    }

    template<typename TimePoinType>
    void submit_until(std::function<void()> &&task,TimePoinType time_point)
    {
        auto time_point_num = std::chrono::time_point_cast<std::chrono::nanoseconds>(time_point).time_since_epoch().count();
        {
            std::unique_lock ul(pq_mtx);
            pq.push({time_point_num,std::forward<std::function<void()>>(task)});
        }
        pq_cv.notify_one();
    }

    void submit(std::function<void()>);

    void run();

    void close();
private:
    struct task_info
    {
        long long time_point_num;
        std::function<void()> task;
        bool operator<(const task_info& rhs) const {
            return time_point_num > rhs.time_point_num;  
        }
    };

    std::priority_queue<task_info> pq;
    std::mutex pq_mtx;
    std::condition_variable pq_cv;

    std::deque<std::function<void()>> dq;
    std::mutex dq_mtx;
    std::condition_variable dq_cv;

    std::vector<std::thread> workers;
    std::atomic_bool is_run;
};
}
#endif