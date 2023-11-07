/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_STATE_WRITER_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_STATE_WRITER_H_

#include <string>

namespace hns_rewards::internal {
namespace state {

template <class T, class U>
class Writer {
 public:
  virtual bool ToJson(T writer, const U&) const = 0;

  virtual std::string ToJson(const U&) const = 0;
};

}  // namespace state
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_LEGACY_STATE_WRITER_H_
