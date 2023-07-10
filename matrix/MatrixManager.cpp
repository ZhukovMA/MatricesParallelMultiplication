#include "MatrixManager.h"
#include <iostream>

namespace matrix{

MatrixManager::MatrixManager(size_t number_of_threads) : stop(false){
    // сreating the specified number of daemon threads that are waiting for items in the queue
    for(size_t i = 0; i < number_of_threads; ++i){
        threads.emplace_back([&]{
                                    while(true){
                                        std::function<void()> task;

                                        std::unique_lock<std::mutex> u_lock(queue_mutex);
                                        condition.wait(u_lock, [&]{ return stop || !tasks.empty(); });
                                        if(stop && tasks.empty())
                                            return;
                                        task = std::move(tasks.front());
                                        tasks.pop();
                                        u_lock.unlock();
                                        

                                        task();
                                    }
                                });
        }
}

MatrixManager::~MatrixManager()
{   
    std::unique_lock<std::mutex> u_lock(queue_mutex);
    stop = true;
    u_lock.unlock();

    condition.notify_all();

    for(std::thread &thread : threads)
        thread.join();
}

}