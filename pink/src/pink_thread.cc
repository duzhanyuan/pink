// Copyright (c) 2015-present, Qihoo, Inc.  All rights reserved.
// This source code is licensed under the BSD-style license found in the
// LICENSE file in the root directory of this source tree. An additional grant
// of patent rights can be found in the PATENTS file in the same directory.

#include "pink/include/pink_thread.h"
#include "pink/src/pink_thread_name.h"
#include "slash/include/xdebug.h"
#include "pink/include/pink_define.h"

namespace pink {

Thread::Thread()
  : running_(false),
    thread_id_(0),
    ehandle_(nullptr),
    private_(nullptr) {
}

Thread::~Thread() {
}

void* Thread::RunThread(void *arg) {
  Thread* thread = reinterpret_cast<Thread*>(arg);
  if (!(thread->thread_name().empty())) {
    SetThreadName(pthread_self(), thread->thread_name());
  }
  thread->ThreadMain();
  return nullptr;
}

int Thread::StartThread() {
  if (ehandle_ != nullptr && ehandle_->SetEnv(&private_) == -1) {
    return -1;
  }
  bool expect = false;
  if (!running_.compare_exchange_strong(expect, true)) {
    return 0;
  }
  return pthread_create(&thread_id_, nullptr, RunThread, (void *)this);
}

int Thread::StopThread() {
  set_running(false);
  return pthread_join(thread_id_, nullptr);
}

int Thread::JoinThread() {
  return pthread_join(thread_id_, nullptr);
}

}  // namespace pink
