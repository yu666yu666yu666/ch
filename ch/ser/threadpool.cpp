#include "ser.hpp"

class ThreadPool {
public:
    explicit ThreadPool(std::size_t num);
    ~ThreadPool();
    template<class F>
    void add(F&& f);

private:
    void work();
    std::vector<std::thread> threads; 
    std::queue<std::function<void()>> tasks; 
    std::mutex qmutex; 
    std::condition_variable condition; 
    bool stop;
};

template<class F>
void ThreadPool::add(F&& f) {
    {
        std::unique_lock<std::mutex> lock(qmutex);
        if (stop) {
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }
        tasks.emplace(std::forward<F>(f));
    }
    condition.notify_one();
}

ThreadPool::ThreadPool(std::size_t num_threads) : stop(false) {
    for (std::size_t i = 0; i < num_threads; ++i) {
        threads.emplace_back([this] {
            work();
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(qmutex);
        stop = true;
    }

    condition.notify_all();
    for (std::thread &worker : threads) {
        worker.join();
    }
}

void ThreadPool::work() {
    while (true) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(qmutex);
            condition.wait(lock, [this] {
                return stop || !tasks.empty();
            });
            if (stop && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        task(); 
    }
}