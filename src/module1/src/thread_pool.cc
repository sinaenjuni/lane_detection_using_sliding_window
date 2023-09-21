#include "thread_pool.h"


ThreadPool::ThreadPool(size_t num_threads)
    : num_threads_(num_threads), stop_all(false) 
{
    worker_threads_.reserve(num_threads_); // 용량 확보
    for (size_t i = 0; i < num_threads_; ++i) {
        worker_threads_.emplace_back([this]() { this->WorkerThread(); }); 
        // 초기화와 동시에 벡터에 요소 추가
    }
}

bool ThreadPool::is_empty(){
    return jobs_.size();
}

void ThreadPool::WorkerThread() {
    while (true) {
        std::unique_lock<std::mutex> lock(m_job_q_);
        cv_job_q_.wait(lock, [this]() { return !this->jobs_.empty() || stop_all; });
        if (stop_all && this->jobs_.empty()) {
            return;
        }

        // 맨 앞의 job 을 뺀다.
        std::function<void()> job = std::move(jobs_.front());
        jobs_.pop();
        lock.unlock();

        // 해당 job 을 수행한다 :)
        job();
    }
}


void ThreadPool::EnqueueJob(std::function<void()> job) {
    if (stop_all) {
        throw std::runtime_error("ThreadPool 사용 중지됨");
    }
    {
        std::lock_guard<std::mutex> lock(m_job_q_);
        jobs_.push(std::move(job));
    }
    cv_job_q_.notify_one();
}


ThreadPool::~ThreadPool() {
    stop_all = true;
    cv_job_q_.notify_all();

    for (auto& t : worker_threads_) {
        t.join();
    }
}


