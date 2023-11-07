/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TRANSFER_TRANSFER_OBSERVER_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TRANSFER_TRANSFER_OBSERVER_H_

#include <cstdint>

#include "base/observer_list_types.h"
#include "base/time/time.h"

namespace hns_ads {

struct AdInfo;

class TransferObserver : public base::CheckedObserver {
 public:
  // Invoked when an ad will be transferred
  virtual void OnWillTransferAd(const AdInfo& ad, const base::Time time) {}

  // Invoked when an ad is transferred
  virtual void OnDidTransferAd(const AdInfo& ad) {}

  // Invoked when an ad transfer is canceled
  virtual void OnCanceledTransfer(const AdInfo& ad, const int32_t tab_id) {}

  // Invoked when an ad fails to transfer
  virtual void OnFailedToTransferAd(const AdInfo& ad) {}
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_TRANSFER_TRANSFER_OBSERVER_H_
