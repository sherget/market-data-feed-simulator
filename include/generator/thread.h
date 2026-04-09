#ifndef THREAD_H
#define THREAD_H

#include <atomic>
#include <functional>
#include <thread>

namespace market_data {

class Thread {
   public:
    using WorkerFunction = std::function<void()>;

    Thread();
    explicit Thread(WorkerFunction func);
    ~Thread();

    Thread(const Thread&) = delete;
    Thread& operator=(const Thread&) = delete;
    Thread(Thread&& other) noexcept;
    Thread& operator=(Thread&& other) noexcept;

    void start();
    void stop();
    void join();
    bool is_running() const;
    bool is_joinable() const;

   private:
    WorkerFunction func_;
    std::thread thread_;
    std::atomic<bool> running_{false};
};

}  // namespace market_data

#endif
