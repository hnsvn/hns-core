/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_ATTESTATION_ATTESTATION_DESKTOP_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_ATTESTATION_ATTESTATION_DESKTOP_H_

#include <string>

#include "base/values.h"
#include "hns/components/hns_rewards/core/attestation/attestation.h"
#include "hns/components/hns_rewards/core/endpoint/promotion/promotion_server.h"

namespace hns_rewards::internal {
class RewardsEngineImpl;

namespace attestation {

class AttestationDesktop : public Attestation {
 public:
  explicit AttestationDesktop(RewardsEngineImpl& engine);
  ~AttestationDesktop() override;

  void Start(const std::string& payload, StartCallback callback) override;

  void Confirm(const std::string& solution, ConfirmCallback callback) override;

 private:
  mojom::Result ParseClaimSolution(const std::string& response,
                                   int* x,
                                   int* y,
                                   std::string* captcha_id);

  void DownloadCaptchaImage(StartCallback callback,
                            mojom::Result result,
                            const std::string& hint,
                            const std::string& captcha_id);

  void OnDownloadCaptchaImage(StartCallback callback,
                              const std::string& hint,
                              const std::string& captcha_id,
                              mojom::Result result,
                              const std::string& image);

  void OnConfirm(ConfirmCallback callback, mojom::Result result);

  endpoint::PromotionServer promotion_server_;
};

}  // namespace attestation
}  // namespace hns_rewards::internal
#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_ATTESTATION_ATTESTATION_DESKTOP_H_
