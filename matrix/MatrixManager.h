#ifndef MATRIX_MANAGER_H
#define MATRIX_MANAGER_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace matrix{

// Thread Pool that performs functions in threads without wasting time on creating and initializing new ones
class MatrixManager{
public:
    MatrixManager(size_t);
    ~MatrixManager();

    // add function for asynchronous execution
    template<class F, class... Args>
    auto create_task(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector< std::thread > threads;

    // queue of tasks to execute
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    // flag of class shutdown
    bool stop;
};

// Adding a task to the work pool, returns future to get the result of the calculation on demand
template<class F, class... Args>
auto MatrixManager::create_task(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
    
    std::future<return_type> result = task->get_future();
    
    std::unique_lock<std::mutex> u_lock(queue_mutex);
    tasks.emplace([task](){ (*task)(); });
    u_lock.unlock();
    
    condition.notify_one();
    return result;
}

}

#endif
