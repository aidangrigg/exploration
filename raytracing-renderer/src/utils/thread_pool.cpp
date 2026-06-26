#include "utils/thread_pool.hpp"

ThreadPool::ThreadPool(int num_threads)
    : running(true), busy(0) {
  for (int i = 0; i < num_threads; i++) {
    threads.emplace_back([this] {
      while (true) {
        std::unique_lock<std::mutex> lk(mtx);

        cv_work.wait(lk, [this] { return !tasks.empty() || !running; });

        if (tasks.empty() || !running)
          return;

        auto task = tasks.front();
        tasks.pop();
        busy++;
        lk.unlock();

        // std::cout << "Task started" << std::endl;
        task();
        // std::cout << "Task ended" << std::endl;

        lk.lock();
        busy--;
        cv_finished.notify_one();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  running = false;

  cv_work.notify_all();

  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}

bool ThreadPool::is_running() { return running; }

bool ThreadPool::is_busy() { return busy != 0; }

void ThreadPool::wait() {
  std::unique_lock<std::mutex> lk(mtx);
  cv_finished.wait(lk, [this]() { return tasks.empty() && (busy == 0); });
}

void ThreadPool::push_task(std::function<void()> fn) {
  {
    std::unique_lock<std::mutex> lk(mtx);
    tasks.push(fn);
  }

  cv_work.notify_one();
}
