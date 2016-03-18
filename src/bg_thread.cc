#include "bg_thread.h"
namespace pink {
void BGThread::Schedule(void (*function)(void*), void* arg) {
  pthread_mutex_lock(&mu_);
  queue_.push_back(BGItem(function, arg));
  pthread_cond_signal(&signal_);
  pthread_mutex_unlock(&mu_);
}

void *BGThread::ThreadMain() {
  running_ = true;
  while (!exit_) {
    pthread_mutex_lock(&mu_);
    while (queue_.empty() && !exit_) {
      pthread_cond_wait(&signal_, &mu_);
    }
    if (exit_) {
      pthread_mutex_unlock(&mu_);
      continue;
    }
    void (*function)(void*) = queue_.front().function;
    void* arg = queue_.front().arg;
    queue_.pop_front();
    pthread_mutex_unlock(&mu_);
    (*function)(arg);
  }
  return NULL;
}
}
