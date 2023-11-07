/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/permissions/hns_wallet_permission_prompt_android.h"

#include <utility>

#include "hns/browser/permissions/hns_dapp_permission_prompt_dialog_controller_android.h"
#include "hns/components/hns_wallet/browser/permission_utils.h"
#include "hns/components/permissions/contexts/hns_wallet_permission_context.h"
#include "components/permissions/android/permission_prompt/permission_prompt_android.h"
#include "content/public/browser/web_contents.h"

HnsWalletPermissionPrompt::HnsWalletPermissionPrompt(
    content::WebContents* web_contents,
    std::unique_ptr<Delegate> delegate,
    hns_wallet::mojom::CoinType coin_type)
    : web_contents_(web_contents), delegate_(std::move(delegate)) {
  dialog_controller_ =
      std::make_unique<HnsDappPermissionPromptDialogController>(
          this, web_contents_, coin_type);
  dialog_controller_->ShowDialog();
}

HnsWalletPermissionPrompt::~HnsWalletPermissionPrompt() {}

void HnsWalletPermissionPrompt::ConnectToSite(
    const std::vector<std::string>& accounts) {
  has_interacted_with_dialog_ = true;
  dialog_controller_.reset();
  // TODO(SergeyZhukovsky): Use the real option that the user chooses, using
  // `kForever` here is for landing new API changes separately.
  permissions::HnsWalletPermissionContext::AcceptOrCancel(
      accounts, hns_wallet::mojom::PermissionLifetimeOption::kForever,
      web_contents_);
}

void HnsWalletPermissionPrompt::CancelConnectToSite() {
  has_interacted_with_dialog_ = true;
  dialog_controller_.reset();
  permissions::HnsWalletPermissionContext::Cancel(web_contents_);
}

void HnsWalletPermissionPrompt::OnDialogDismissed() {
  if (!dialog_controller_) {
    // Dismissed by clicking on dialog buttons.
    return;
  }
  dialog_controller_.reset();
  // If |has_interacted_with_dialog_| is true, |ConnectToSite| or
  // |CancelConnectToSite| should be recorded instead.
  if (!has_interacted_with_dialog_) {
    permissions::HnsWalletPermissionContext::Cancel(web_contents_);
  }
}

void HnsWalletPermissionPrompt::Delegate::Closing() {
  if (!permission_prompt_) {
    return;
  }
  permission_prompt_->Closing();
}

HnsWalletPermissionPrompt::Delegate::~Delegate() {
  Closing();
}

HnsWalletPermissionPrompt::Delegate::Delegate() {}

HnsWalletPermissionPrompt::Delegate::Delegate(
    const base::WeakPtr<permissions::PermissionPromptAndroid>&
        permission_prompt)
    : permission_prompt_(permission_prompt) {}
