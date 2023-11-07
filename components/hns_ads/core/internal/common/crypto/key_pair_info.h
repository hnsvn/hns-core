/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CRYPTO_KEY_PAIR_INFO_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CRYPTO_KEY_PAIR_INFO_H_

#include <cstdint>
#include <vector>

namespace hns_ads::crypto {

struct KeyPairInfo final {
  KeyPairInfo();

  KeyPairInfo(const KeyPairInfo&);
  KeyPairInfo& operator=(const KeyPairInfo&);

  KeyPairInfo(KeyPairInfo&&) noexcept;
  KeyPairInfo& operator=(KeyPairInfo&&) noexcept;

  ~KeyPairInfo();

  bool operator==(const KeyPairInfo&) const;
  bool operator!=(const KeyPairInfo&) const;

  [[nodiscard]] bool IsValid() const;

  std::vector<uint8_t> public_key;
  std::vector<uint8_t> secret_key;
};

}  // namespace hns_ads::crypto

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CRYPTO_KEY_PAIR_INFO_H_
