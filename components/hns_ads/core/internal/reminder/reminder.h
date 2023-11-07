/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_REMINDER_REMINDER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_REMINDER_REMINDER_H_

#include "base/timer/timer.h"
#include "hns/components/hns_ads/core/internal/history/history_manager_observer.h"

namespace hns_ads {

struct HistoryItemInfo;

class Reminder : public HistoryManagerObserver {
 public:
  Reminder();

  Reminder(const Reminder&) = delete;
  Reminder& operator=(const Reminder&) = delete;

  Reminder(Reminder&&) noexcept = delete;
  Reminder& operator=(Reminder&&) noexcept = delete;

  ~Reminder() override;

 private:
  // HistoryManagerObserver:
  void OnDidAddHistory(const HistoryItemInfo& history_item) override;

  base::OneShotTimer timer_;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_REMINDER_REMINDER_H_
