/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_
#define HNS_COMPONENTS_HNS_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_

#include "base/strings/string_piece.h"

namespace base {
template <typename T>
class NoDestructor;
}  // namespace base

namespace hns {

class PrivateCdnHelper final {
 public:
  PrivateCdnHelper(const PrivateCdnHelper&) = delete;
  PrivateCdnHelper& operator=(const PrivateCdnHelper&) = delete;

  static PrivateCdnHelper* GetInstance();

  bool RemovePadding(base::StringPiece* padded_string) const;

 private:
  friend base::NoDestructor<PrivateCdnHelper>;

  PrivateCdnHelper();
  ~PrivateCdnHelper();
};

}  // namespace hns

#endif  // HNS_COMPONENTS_HNS_PRIVATE_CDN_PRIVATE_CDN_HELPER_H_
