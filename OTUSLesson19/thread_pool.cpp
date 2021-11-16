#include "thread_pool.h"

#include <iostream>
#include <utility>

ThreadPool::ThreadPool(std::size_t threads_num, std::string metric_name)
        : metrics_(threads_num), metric_name_(std::move(metric_name)) {
    for (std::size_t i = 0; i < threads_num; ++i) {
        workers_.emplace_back(
                [this, i](){
                    while (true) {
                        std::unique_lock<std::mutex> lk(this->cv_m_);
                        condition_.wait(lk, [this](){
                            return !this->tasks_.empty() || this->quit_;
                        });
                        if (this->quit_ && this->tasks_.empty()) {
                            return;
                        }
                        if (!this->tasks_.empty()) {
                            auto f = std::move(this->tasks_.front());
                            this->tasks_.pop();
                            lk.unlock();
                            metrics_[i].command_num += f.get();
                            ++metrics_[i].block_num;
                        }
                    }
                }
        );
    }
}

ThreadPool::~ThreadPool() {
    quit_ = true;
    condition_.notify_all();
    for (auto& worker : workers_) {
        worker.join();
    }
    output_metrics();
}

void ThreadPool::output_metrics() {
    for (const auto& m : metrics_) {
        std::cout << metric_name_ << ": " << m << '\n';
    }
}
