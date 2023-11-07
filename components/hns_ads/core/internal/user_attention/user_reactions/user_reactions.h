/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_REACTIONS_USER_REACTIONS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_REACTIONS_USER_REACTIONS_H_

#include "base/memory/raw_ref.h"
#include "hns/components/hns_ads/core/internal/history/history_manager_observer.h"

namespace hns_ads {

class Account;
struct AdContentInfo;

class UserReactions final : public HistoryManagerObserver {
 public:
  explicit UserReactions(Account& account);

  UserReactions(const UserReactions&) = delete;
  UserReactions& operator=(const UserReactions&) = delete;

  UserReactions(UserReactions&&) noexcept = delete;
  UserReactions& operator=(UserReactions&&) noexcept = delete;

  ~UserReactions() override;

 private:
  // HistoryManagerObserver:
  void OnDidLikeAd(const AdContentInfo& ad_content) override;
  void OnDidDislikeAd(const AdContentInfo& ad_content) override;
  void OnDidMarkAdAsInappropriate(const AdContentInfo& ad_content) override;
  void OnDidSaveAd(const AdContentInfo& ad_content) override;

  const raw_ref<const Account> account_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_USER_ATTENTION_USER_REACTIONS_USER_REACTIONS_H_
