/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_DELEGATE_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_DELEGATE_H_

namespace hns_ads {

struct ConfirmationInfo;

class ConfirmationDelegate {
 public:
  // Invoked to tell the delegate that the |confirmation| was successfully sent.
  virtual void OnDidConfirm(const ConfirmationInfo& confirmation) {}

  // Invoked to tell the delegate that the |confirmation| failed to send.
  virtual void OnFailedToConfirm(const ConfirmationInfo& confirmation) {}

 protected:
  virtual ~ConfirmationDelegate() = default;
};

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_CONFIRMATIONS_CONFIRMATIONS_DELEGATE_H_
