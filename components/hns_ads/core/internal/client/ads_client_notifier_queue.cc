/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/client/ads_client_notifier_queue.h"

#include <utility>

namespace hns_ads {

AdsClientNotifierQueue::AdsClientNotifierQueue() = default;

AdsClientNotifierQueue::~AdsClientNotifierQueue() = default;

void AdsClientNotifierQueue::Add(base::OnceClosure notifier) {
  queue_.push(std::move(notifier));
}

void AdsClientNotifierQueue::Process() {
  while (!queue_.empty()) {
    std::move(queue_.front()).Run();
    queue_.pop();
  }
}

}  // namespace hns_ads
