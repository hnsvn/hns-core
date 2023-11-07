/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_COMMON_MEM_UTILS_H_
#define HNS_COMPONENTS_HNS_WALLET_COMMON_MEM_UTILS_H_

#include <stddef.h>
#include <stdint.h>

namespace hns_wallet {

// When we call memset in end of function to clean local variables
// for security reason, compiler optimizer can remove such call.
// So we use our own function for this purpose.
void SecureZeroData(void* data, size_t size);

// Allocator which will zero out memory when destruct
template <typename T>
struct SecureZeroAllocator {
  SecureZeroAllocator() = default;
  ~SecureZeroAllocator() = default;
  SecureZeroAllocator(const SecureZeroAllocator&) = default;
  SecureZeroAllocator& operator=(const SecureZeroAllocator&) = default;
  SecureZeroAllocator(SecureZeroAllocator&&) = default;
  SecureZeroAllocator& operator=(SecureZeroAllocator&&) = default;

  using value_type = T;
  T* allocate(size_t n) {
    return static_cast<T*>(::operator new(n * sizeof(T)));
  }
  void deallocate(T* p, size_t n) {
    SecureZeroData(p, n);
    ::operator delete(p);
  }
};

}  // namespace hns_wallet

#endif  // HNS_COMPONENTS_HNS_WALLET_COMMON_MEM_UTILS_H_
