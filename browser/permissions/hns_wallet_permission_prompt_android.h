/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_PERMISSIONS_HNS_WALLET_PERMISSION_PROMPT_ANDROID_H_
#define HNS_BROWSER_PERMISSIONS_HNS_WALLET_PERMISSION_PROMPT_ANDROID_H_

#include <memory>
#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "hns/browser/permissions/hns_dapp_permission_prompt_dialog_controller_android.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "components/permissions/permissions_client.h"

namespace content {
class WebContents;
}

namespace permissions {
class PermissionPromptAndroid;
}

class HnsWalletPermissionPrompt
    : public HnsDappPermissionPromptDialogController::Delegate,
      public permissions::PermissionsClient::PermissionMessageDelegate {
 public:
  class Delegate {
   public:
    Delegate();
    Delegate(const base::WeakPtr<permissions::PermissionPromptAndroid>&
                 permission_prompt);
    virtual ~Delegate();
    virtual void Closing();

   private:
    base::WeakPtr<permissions::PermissionPromptAndroid> permission_prompt_;
  };

  HnsWalletPermissionPrompt(content::WebContents* web_contents,
                              std::unique_ptr<Delegate> delegate,
                              hns_wallet::mojom::CoinType coin_type);
  ~HnsWalletPermissionPrompt() override;

 protected:
  // HnsDappPermissionPromptDialogController::Delegate:
  void OnDialogDismissed() override;
  void ConnectToSite(const std::vector<std::string>& accounts) override;
  void CancelConnectToSite() override;

 private:
  std::unique_ptr<HnsDappPermissionPromptDialogController> dialog_controller_;
  raw_ptr<content::WebContents> web_contents_ = nullptr;
  std::unique_ptr<Delegate> delegate_;

  bool has_interacted_with_dialog_ = false;
};

#endif  // HNS_BROWSER_PERMISSIONS_HNS_WALLET_PERMISSION_PROMPT_ANDROID_H_
