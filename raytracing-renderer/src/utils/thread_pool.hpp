#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <iostream>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
private:
  std::vector<std::thread> threads;
  std::atomic<bool> running;
  std::queue<std::function<void()>> tasks;
  std::mutex mtx;
  std::condition_variable cv_work, cv_finished;
  std::atomic<int> busy;

public:
  ThreadPool(int num_threads = std::thread::hardware_concurrency()) : running(true), busy(0) {
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

  ~ThreadPool() {
    running = false;

    cv_work.notify_all();

    for (auto &t : threads) {
      if (t.joinable()) {
        t.join();
      }
    }
  }

  bool is_running() { return running; }

  void wait() {
    std::unique_lock<std::mutex> lk(mtx);
    cv_finished.wait(lk, [this]() { return tasks.empty() && (busy == 0); });
  }

  void push_task(std::function<void()> fn) {
    {
      std::unique_lock<std::mutex> lk(mtx);
      tasks.push(fn);
    }

    cv_work.notify_one();
  }
};
