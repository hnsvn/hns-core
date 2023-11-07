/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/hns_wallet_service_delegate_impl_android.h"

#include <utility>
#include <vector>

#include "base/ranges/algorithm.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/permission_utils.h"
#include "hns/components/permissions/contexts/hns_wallet_permission_context.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/android/tab_model/tab_model.h"
#include "chrome/browser/ui/android/tab_model/tab_model_list.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/web_contents.h"
#include "url/gurl.h"

namespace hns_wallet {

namespace {
content::WebContents* GetActiveWebContents(content::BrowserContext* context) {
  auto tab_models = TabModelList::models();
  auto iter = base::ranges::find_if(
      tab_models, [](const auto& model) { return model->IsActiveModel(); });
  if (iter == tab_models.end()) {
    return nullptr;
  }

  auto* active_contents = (*iter)->GetActiveWebContents();
  if (!active_contents) {
    return nullptr;
  }
  DCHECK_EQ(active_contents->GetBrowserContext(), context);
  return active_contents;
}
}  // namespace

HnsWalletServiceDelegateImpl::HnsWalletServiceDelegateImpl(
    content::BrowserContext* context)
    : context_(context), weak_ptr_factory_(this) {}

HnsWalletServiceDelegateImpl::~HnsWalletServiceDelegateImpl() = default;

bool HnsWalletServiceDelegateImpl::AddPermission(mojom::CoinType coin,
                                                   const url::Origin& origin,
                                                   const std::string& account) {
  auto type = CoinTypeToPermissionType(coin);
  if (!type) {
    return false;
  }

  return permissions::HnsWalletPermissionContext::AddPermission(
      *type, context_, origin, account);
}

bool HnsWalletServiceDelegateImpl::HasPermission(mojom::CoinType coin,
                                                   const url::Origin& origin,
                                                   const std::string& account) {
  bool has_permission = false;
  auto type = CoinTypeToPermissionType(coin);
  if (!type) {
    return false;
  }

  bool success = permissions::HnsWalletPermissionContext::HasPermission(
      *type, context_, origin, account, &has_permission);
  return success && has_permission;
}

bool HnsWalletServiceDelegateImpl::ResetPermission(
    mojom::CoinType coin,
    const url::Origin& origin,
    const std::string& account) {
  auto type = CoinTypeToPermissionType(coin);
  if (!type) {
    return false;
  }

  return permissions::HnsWalletPermissionContext::ResetPermission(
      *type, context_, origin, account);
}

bool HnsWalletServiceDelegateImpl::IsPermissionDenied(
    mojom::CoinType coin,
    const url::Origin& origin) {
  auto type = CoinTypeToPermissionType(coin);
  if (!type) {
    return false;
  }

  return permissions::HnsWalletPermissionContext::IsPermissionDenied(
      *type, context_, origin);
}

void HnsWalletServiceDelegateImpl::GetWebSitesWithPermission(
    mojom::CoinType coin,
    GetWebSitesWithPermissionCallback callback) {
  std::vector<std::string> result;
  auto type = CoinTypeToPermissionType(coin);
  if (!type) {
    std::move(callback).Run(result);
    return;
  }
  std::move(callback).Run(
      permissions::HnsWalletPermissionContext::GetWebSitesWithPermission(
          *type, context_));
}

void HnsWalletServiceDelegateImpl::ResetWebSitePermission(
    mojom::CoinType coin,
    const std::string& formed_website,
    ResetWebSitePermissionCallback callback) {
  auto type = CoinTypeToPermissionType(coin);
  if (!type) {
    std::move(callback).Run(false);
    return;
  }
  std::move(callback).Run(
      permissions::HnsWalletPermissionContext::ResetWebSitePermission(
          *type, context_, formed_website));
}

absl::optional<url::Origin> HnsWalletServiceDelegateImpl::GetActiveOrigin() {
  content::WebContents* contents = GetActiveWebContents(context_);
  auto origin = contents
                    ? contents->GetPrimaryMainFrame()->GetLastCommittedOrigin()
                    : absl::optional<url::Origin>();
  return origin;
}

}  // namespace hns_wallet
