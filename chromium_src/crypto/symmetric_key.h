/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CRYPTO_SYMMETRIC_KEY_H_
#define HNS_CHROMIUM_SRC_CRYPTO_SYMMETRIC_KEY_H_

#define DeriveKeyFromPasswordUsingPbkdf2                                    \
  DeriveKeyFromPasswordUsingPbkdf2Sha256(                                   \
      Algorithm algorithm, const std::string& password,                     \
      const std::string& salt, size_t iterations, size_t key_size_in_bits); \
  static std::unique_ptr<SymmetricKey> DeriveKeyFromPasswordUsingPbkdf2
#include "src/crypto/symmetric_key.h"  // IWYU pragma: export
#undef DeriveKeyFromPasswordUsingPbkdf2

#endif  // HNS_CHROMIUM_SRC_CRYPTO_SYMMETRIC_KEY_H_
