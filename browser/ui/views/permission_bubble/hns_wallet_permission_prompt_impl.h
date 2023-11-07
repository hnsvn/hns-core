/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_PERMISSION_BUBBLE_HNS_WALLET_PERMISSION_PROMPT_IMPL_H_
#define HNS_BROWSER_UI_VIEWS_PERMISSION_BUBBLE_HNS_WALLET_PERMISSION_PROMPT_IMPL_H_

#include "base/memory/raw_ptr.h"
#include "base/time/time.h"
#include "components/permissions/permission_prompt.h"

class Browser;

class HnsWalletPermissionPromptImpl : public permissions::PermissionPrompt {
 public:
  HnsWalletPermissionPromptImpl(Browser* browser,
                                  content::WebContents* web_contents,
                                  Delegate& delegate);
  ~HnsWalletPermissionPromptImpl() override;

  HnsWalletPermissionPromptImpl(const HnsWalletPermissionPromptImpl&) =
      delete;
  HnsWalletPermissionPromptImpl& operator=(
      const HnsWalletPermissionPromptImpl&) = delete;

  // permissions::PermissionPrompt:
  bool UpdateAnchor() override;
  TabSwitchingBehavior GetTabSwitchingBehavior() override;
  permissions::PermissionPromptDisposition GetPromptDisposition()
      const override;
  absl::optional<gfx::Rect> GetViewBoundsInScreen() const override;

 private:
  void ShowBubble();

  const raw_ptr<content::WebContents> web_contents_ = nullptr;
  raw_ref<permissions::PermissionPrompt::Delegate> delegate_;
  base::TimeTicks permission_requested_time_;
};

#endif  // HNS_BROWSER_UI_VIEWS_PERMISSION_BUBBLE_HNS_WALLET_PERMISSION_PROMPT_IMPL_H_
