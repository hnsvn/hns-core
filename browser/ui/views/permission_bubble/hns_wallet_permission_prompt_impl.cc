/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/permission_bubble/hns_wallet_permission_prompt_impl.h"

#include "hns/browser/hns_wallet/hns_wallet_tab_helper.h"
#include "components/permissions/permission_uma_util.h"

HnsWalletPermissionPromptImpl::HnsWalletPermissionPromptImpl(
    Browser* browser,
    content::WebContents* web_contents,
    Delegate& delegate)
    : web_contents_(web_contents),
      delegate_(delegate),
      permission_requested_time_(base::TimeTicks::Now()) {
  DCHECK(web_contents_);
  ShowBubble();
}

HnsWalletPermissionPromptImpl::~HnsWalletPermissionPromptImpl() {
  hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents_)
      ->CloseBubble();
}

void HnsWalletPermissionPromptImpl::ShowBubble() {
  hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents_)
      ->ShowBubble();
}

bool HnsWalletPermissionPromptImpl::UpdateAnchor() {
  // Don't recreate the view for every BrowserView::Layout() which would cause
  // HnsWalletPermissionPromptImpl being destoryed which leads to bubble
  // dismissed unintentionally.
  return true;
}

permissions::PermissionPrompt::TabSwitchingBehavior
HnsWalletPermissionPromptImpl::GetTabSwitchingBehavior() {
  return permissions::PermissionPrompt::TabSwitchingBehavior::
      kDestroyPromptButKeepRequestPending;
}

permissions::PermissionPromptDisposition
HnsWalletPermissionPromptImpl::GetPromptDisposition() const {
  return permissions::PermissionPromptDisposition::ANCHORED_BUBBLE;
}

absl::optional<gfx::Rect>
HnsWalletPermissionPromptImpl::GetViewBoundsInScreen() const {
  return absl::nullopt;
}
