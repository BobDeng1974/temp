/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#ifndef PUBLIC_SPINLOCK_H_
#define PUBLIC_SPINLOCK_H_

#include <atomic>

namespace hwcomposer {

class SpinLock {
 public:
  void lock() {
    while (atomic_lock_.test_and_set(std::memory_order_acquire)) {
    }
#ifdef HWC_DEVELOPER_BUILD
    locked_ = true;
#endif
  }

  void unlock() {
    atomic_lock_.clear(std::memory_order_release);
#ifdef HWC_DEVELOPER_BUILD
    locked_ = false;
#endif
  }

#ifdef HWC_DEVELOPER_BUILD
  bool islocked() const {
    return locked_;
  }
#endif
 private:
  std::atomic_flag atomic_lock_ = ATOMIC_FLAG_INIT;
#ifdef HWC_DEVELOPER_BUILD
  bool locked_ = false;
#endif
};

class ScopedSpinLock {
 public:
  explicit ScopedSpinLock(SpinLock& lock) : lock_(lock) {
    lock_.lock();
  }

  ~ScopedSpinLock() {
    lock_.unlock();
  }

 private:
  SpinLock& lock_;
};

class ScopedSpinLocks {
 public:
  ScopedSpinLocks(SpinLock& lock1, SpinLock& lock2)
      : lock1_(lock1), lock2_(lock2) {
    lock1_.lock();
    lock2_.lock();
  }

  ~ScopedSpinLocks() {
    lock1_.unlock();
    lock2_.unlock();
  }

 private:
  SpinLock& lock1_;
  SpinLock& lock2_;
};

}  // namespace hwcomposer
#endif  // PUBLIC_SPINLOCK_H_
