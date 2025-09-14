#include <GreenTea/Threadpool.hpp> 
#include <iostream>
#include <chrono>
 
int main() {
    std::cout << "=== ThreadPool Test ===" << std::endl;
    
    // 创建线程池（默认使用硬件并发数）
    GreenTea::Threadpool pool(4); 
    
    // 启动调度线程 
    std::thread scheduler([&pool] {
        pool.run(); 
    });
 
    // 测试即时任务 
    std::cout << "Submitting immediate tasks..." << std::endl;
    for (int i = 1; i <= 3; ++i) {
        pool.submit([i]  {
            std::cout << "Immediate task " << i << " executed by thread " 
                      << std::this_thread::get_id() << std::endl;
        });
    }
 
    // 测试延迟任务 
    std::cout << "\nSubmitting delayed tasks..." << std::endl;
    for (int i = 1; i <= 3; ++i) {
        pool.submit_for( 
            [i] {
                std::cout << "Delayed task " << i << " executed by thread " 
                          << std::this_thread::get_id() << " at " 
                          << std::chrono::system_clock::now().time_since_epoch().count() 
                          << std::endl;
            },
            std::chrono::seconds(i)  // 分别延迟1/2/3秒 
        );
    }
 
    // 测试定时任务 
    std::cout << "\nSubmitting timed tasks..." << std::endl;
    for (int i = 1; i <= 3; ++i) {
        auto now = std::chrono::steady_clock::now();
        pool.submit_until( 
            [i] {
                std::cout << "Timed task " << i << " executed by thread " 
                          << std::this_thread::get_id() << " at " 
                          << std::chrono::system_clock::now().time_since_epoch().count() 
                          << std::endl;
            },
            now + std::chrono::seconds(i + 3)  // 分别延迟4/5/6秒 
        );
    }
 
    // 等待所有任务完成 
    std::this_thread::sleep_for(std::chrono::seconds(7));
    
    // 关闭线程池 
    std::cout << "\nShutting down..." << std::endl;
    if (scheduler.joinable())  {
        pool.close();
        scheduler.join(); 
    }
 
    std::cout << "=== Test Completed ===" << std::endl;
    return 0;
}