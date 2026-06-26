#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
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
  ThreadPool(int num_threads = std::thread::hardware_concurrency());

  ~ThreadPool();

  bool is_running();
  bool is_busy();

  void wait();
  void push_task(std::function<void()> fn);
};
