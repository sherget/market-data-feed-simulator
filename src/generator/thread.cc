#include "generator/thread.h"

namespace market_data {

Thread::Thread() = default;

Thread::Thread(WorkerFunction func) : func_(std::move(func)) {}

Thread::~Thread() {
    if (running_) {
        stop();
    }
    if (thread_.joinable()) {
        thread_.join();
    }
}

Thread::Thread(Thread&& other) noexcept
    : func_(std::move(other.func_)),
      thread_(std::move(other.thread_)),
      running_(other.running_.load()) {
    other.running_ = false;
}

Thread& Thread::operator=(Thread&& other) noexcept {
    if (this != &other) {
        if (running_) {
            stop();
        }
        if (thread_.joinable()) {
            thread_.join();
        }
        func_ = std::move(other.func_);
        thread_ = std::move(other.thread_);
        running_ = other.running_.load();
        other.running_ = false;
    }
    return *this;
}

void Thread::start() {
    if (!running_) {
        running_ = true;
        thread_ = std::thread([this]() { func_(); });
    }
}

void Thread::stop() { running_ = false; }

void Thread::join() {
    if (thread_.joinable()) {
        thread_.join();
    }
}

bool Thread::is_running() const { return running_; }

bool Thread::is_joinable() const { return thread_.joinable(); }

}  // namespace market_data
