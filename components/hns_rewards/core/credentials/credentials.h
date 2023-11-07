/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_H_
#define HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_H_

#include "hns/components/hns_rewards/core/credentials/credentials_redeem.h"
#include "hns/components/hns_rewards/core/credentials/credentials_trigger.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"

namespace hns_rewards::internal {
namespace credential {

class Credentials {
 public:
  virtual ~Credentials() = default;

  virtual void Start(const CredentialsTrigger& trigger,
                     ResultCallback callback) = 0;

  virtual void RedeemTokens(const CredentialsRedeem& redeem,
                            LegacyResultCallback callback) = 0;

 protected:
  virtual void Blind(ResultCallback callback,
                     const CredentialsTrigger& trigger) = 0;

  virtual void Claim(ResultCallback callback,
                     const CredentialsTrigger& trigger,
                     mojom::CredsBatchPtr creds) = 0;

  virtual void Unblind(ResultCallback callback,
                       const CredentialsTrigger& trigger,
                       mojom::CredsBatchPtr creds) = 0;

  virtual void Completed(ResultCallback callback,
                         const CredentialsTrigger& trigger,
                         mojom::Result result) = 0;
};

}  // namespace credential
}  // namespace hns_rewards::internal

#endif  // HNS_COMPONENTS_HNS_REWARDS_CORE_CREDENTIALS_CREDENTIALS_H_
