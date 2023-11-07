/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_HNS_MUTABLE_PROFILE_OAUTH2_TOKEN_SERVICE_DELEGATE_H_
#define HNS_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_HNS_MUTABLE_PROFILE_OAUTH2_TOKEN_SERVICE_DELEGATE_H_

#include <memory>

#include "base/memory/raw_ptr.h"
#include "components/signin/internal/identity_manager/mutable_profile_oauth2_token_service_delegate.h"

class HnsMutableProfileOAuth2TokenServiceDelegate
    : public MutableProfileOAuth2TokenServiceDelegate {
 public:
  HnsMutableProfileOAuth2TokenServiceDelegate(
      SigninClient* client,
      AccountTrackerService* account_tracker_service,
      network::NetworkConnectionTracker* network_connection_tracker,
      scoped_refptr<TokenWebData> token_web_data,
      signin::AccountConsistencyMethod account_consistency,
      bool revoke_all_tokens_on_load,
#if BUILDFLAG(ENABLE_BOUND_SESSION_CREDENTIALS)
      std::unique_ptr<TokenBindingHelper> token_binding_helper,
#endif  // BUILDFLAG(ENABLE_BOUND_SESSION_CREDENTIALS)
      FixRequestErrorCallback fix_request_error_callback);
  ~HnsMutableProfileOAuth2TokenServiceDelegate() override;
  HnsMutableProfileOAuth2TokenServiceDelegate(
      const HnsMutableProfileOAuth2TokenServiceDelegate&) = delete;
  HnsMutableProfileOAuth2TokenServiceDelegate& operator=(
      const HnsMutableProfileOAuth2TokenServiceDelegate&) = delete;

  void LoadCredentials(const CoreAccountId& primary_account_id,
                       bool is_syncing) override;

 private:
  raw_ptr<AccountTrackerService> account_tracker_service_ = nullptr;
};

#endif  // HNS_COMPONENTS_SIGNIN_INTERNAL_IDENTITY_MANAGER_HNS_MUTABLE_PROFILE_OAUTH2_TOKEN_SERVICE_DELEGATE_H_
