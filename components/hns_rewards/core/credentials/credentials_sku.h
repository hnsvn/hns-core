/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_SKU_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_SKU_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_rewards/core/credentials/credentials_common.h"
#include "hns/components/hns_rewards/core/endpoint/payment/payment_server.h"

namespace hns_rewards::internal {
namespace credential {

class CredentialsSKU : public Credentials {
 public:
  explicit CredentialsSKU(RewardsEngineImpl& engine);
  ~CredentialsSKU() override;

  void Start(const CredentialsTrigger& trigger,
             ResultCallback callback) override;

  void RedeemTokens(const CredentialsRedeem& redeem,
                    LegacyResultCallback callback) override;

 private:
  void OnStart(ResultCallback callback,
               const CredentialsTrigger& trigger,
               mojom::CredsBatchPtr creds);

  void Blind(ResultCallback callback,
             const CredentialsTrigger& trigger) override;

  void OnBlind(ResultCallback callback,
               const CredentialsTrigger& trigger,
               mojom::Result result);

  void RetryPreviousStepSaved(ResultCallback callback, mojom::Result result);

  void Claim(ResultCallback callback,
             const CredentialsTrigger& trigger,
             mojom::CredsBatchPtr creds) override;

  void OnClaim(ResultCallback callback,
               const CredentialsTrigger& trigger,
               mojom::Result result);

  void ClaimStatusSaved(ResultCallback callback,
                        const CredentialsTrigger& trigger,
                        mojom::Result result);

  void FetchSignedCreds(ResultCallback callback,
                        const CredentialsTrigger& trigger);

  void OnFetchSignedCreds(ResultCallback callback,
                          const CredentialsTrigger& trigger,
                          mojom::Result result,
                          mojom::CredsBatchPtr batch);

  void SignedCredsSaved(ResultCallback callback,
                        const CredentialsTrigger& trigger,
                        mojom::Result result);

  void Unblind(ResultCallback callback,
               const CredentialsTrigger& trigger,
               mojom::CredsBatchPtr creds) override;

  void Completed(ResultCallback callback,
                 const CredentialsTrigger& trigger,
                 mojom::Result result) override;

  void OnRedeemTokens(mojom::Result result,
                      const std::vector<std::string>& token_id_list,
                      const CredentialsRedeem& redeem,
                      LegacyResultCallback callback);

  const raw_ref<RewardsEngineImpl> engine_;
  CredentialsCommon common_;
  endpoint::PaymentServer payment_server_;
};

}  // namespace credential
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_SKU_H_
