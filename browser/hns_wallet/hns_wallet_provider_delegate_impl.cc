/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/hns_wallet_provider_delegate_impl.h"

#include <string>
#include <utility>
#include <vector>

#include "base/functional/bind.h"
#include "hns/browser/hns_wallet/hns_wallet_provider_delegate_impl_helper.h"
#include "hns/browser/hns_wallet/hns_wallet_tab_helper.h"
#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/permission_utils.h"
#include "hns/components/permissions/contexts/hns_wallet_permission_context.h"
#include "components/content_settings/core/common/content_settings.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/visibility.h"
#include "content/public/browser/web_contents.h"

namespace hns_wallet {

namespace {

bool IsAccountInAllowedList(const std::vector<std::string>& allowed_accounts,
                            const std::string& account) {
  for (const auto& allowed_account : allowed_accounts) {
    if (base::CompareCaseInsensitiveASCII(account, allowed_account) == 0) {
      return true;
    }
  }
  return false;
}

void OnRequestPermissions(
    const std::vector<std::string>& accounts,
    HnsWalletProviderDelegate::RequestPermissionsCallback callback,
    const std::vector<blink::mojom::PermissionStatus>& responses) {
  DCHECK(responses.empty() || responses.size() == accounts.size());

  std::vector<std::string> granted_accounts;
  for (size_t i = 0; i < responses.size(); i++) {
    if (responses[i] == blink::mojom::PermissionStatus::GRANTED) {
      granted_accounts.push_back(accounts[i]);
    }
  }
  // The responses array will be empty if operation failed.
  if (responses.empty()) {
    std::move(callback).Run(mojom::RequestPermissionsError::kInternal,
                            absl::nullopt);
  } else {
    std::move(callback).Run(mojom::RequestPermissionsError::kNone,
                            granted_accounts);
  }
}

absl::optional<permissions::RequestType> CoinTypeToPermissionRequestType(
    mojom::CoinType coin_type) {
  switch (coin_type) {
    case mojom::CoinType::ETH:
      return permissions::RequestType::kHnsEthereum;
    case mojom::CoinType::SOL:
      return permissions::RequestType::kHnsSolana;
    default:
      return absl::nullopt;
  }
}

}  // namespace

HnsWalletProviderDelegateImpl::HnsWalletProviderDelegateImpl(
    content::WebContents* web_contents,
    content::RenderFrameHost* const render_frame_host)
    : WebContentsObserver(web_contents),
      web_contents_(web_contents),
      host_id_(render_frame_host->GetGlobalId()),
      weak_ptr_factory_(this) {}

HnsWalletProviderDelegateImpl::~HnsWalletProviderDelegateImpl() = default;

url::Origin HnsWalletProviderDelegateImpl::GetOrigin() const {
  auto* rfh = content::RenderFrameHost::FromID(host_id_);
  return rfh ? rfh->GetLastCommittedOrigin() : url::Origin();
}

bool HnsWalletProviderDelegateImpl::IsTabVisible() {
  return web_contents_
             ? web_contents_->GetVisibility() == content::Visibility::VISIBLE
             : false;
}

void HnsWalletProviderDelegateImpl::ShowPanel() {
  ::hns_wallet::ShowPanel(web_contents_);
}

void HnsWalletProviderDelegateImpl::WalletInteractionDetected() {
  ::hns_wallet::WalletInteractionDetected(web_contents_);
}

void HnsWalletProviderDelegateImpl::ShowWalletOnboarding() {
  ::hns_wallet::ShowWalletOnboarding(web_contents_);
}

void HnsWalletProviderDelegateImpl::ShowAccountCreation(
    mojom::CoinType type) {
  ::hns_wallet::ShowAccountCreation(web_contents_, type);
}

absl::optional<std::vector<std::string>>
HnsWalletProviderDelegateImpl::GetAllowedAccounts(
    mojom::CoinType type,
    const std::vector<std::string>& accounts) {
  if (IsPermissionDenied(type)) {
    return std::vector<std::string>();
  }

  auto permission = CoinTypeToPermissionType(type);
  if (!permission) {
    return absl::nullopt;
  }

  return permissions::HnsWalletPermissionContext::GetAllowedAccounts(
      *permission, content::RenderFrameHost::FromID(host_id_), accounts);
}

void HnsWalletProviderDelegateImpl::RequestPermissions(
    mojom::CoinType type,
    const std::vector<std::string>& accounts,
    RequestPermissionsCallback callback) {
  if (!IsWeb3NotificationAllowed()) {
    std::move(callback).Run(mojom::RequestPermissionsError::kNone,
                            std::vector<std::string>());
    return;
  }
  auto request_type = CoinTypeToPermissionRequestType(type);
  auto permission = CoinTypeToPermissionType(type);
  if (!request_type || !permission) {
    std::move(callback).Run(mojom::RequestPermissionsError::kInternal,
                            absl::nullopt);
    return;
  }
  // Check if there's already a permission request in progress
  auto* rfh = content::RenderFrameHost::FromID(host_id_);
  if (rfh && permissions::HnsWalletPermissionContext::HasRequestsInProgress(
                 rfh, *request_type)) {
    std::move(callback).Run(mojom::RequestPermissionsError::kRequestInProgress,
                            absl::nullopt);
    return;
  }

  permissions::HnsWalletPermissionContext::RequestPermissions(
      *permission, content::RenderFrameHost::FromID(host_id_), accounts,
      base::BindOnce(&OnRequestPermissions, accounts, std::move(callback)));
}

bool HnsWalletProviderDelegateImpl::IsAccountAllowed(
    mojom::CoinType type,
    const std::string& account) {
  auto permission = CoinTypeToPermissionType(type);
  if (!permission) {
    return false;
  }

  const auto allowed_accounts =
      permissions::HnsWalletPermissionContext::GetAllowedAccounts(
          *permission, content::RenderFrameHost::FromID(host_id_), {account});

  return allowed_accounts && IsAccountInAllowedList(*allowed_accounts, account);
}

bool HnsWalletProviderDelegateImpl::IsPermissionDenied(mojom::CoinType type) {
  auto permission = CoinTypeToPermissionType(type);
  if (!permission) {
    return false;
  }

  auto* rfh = content::RenderFrameHost::FromID(host_id_);
  if (!rfh) {
    return false;
  }

  auto* web_contents = content::WebContents::FromRenderFrameHost(rfh);

  return permissions::HnsWalletPermissionContext::IsPermissionDenied(
      *permission, web_contents->GetBrowserContext(),
      url::Origin::Create(rfh->GetLastCommittedURL()));
}

void HnsWalletProviderDelegateImpl::AddSolanaConnectedAccount(
    const std::string& account) {
  if (!web_contents_) {
    return;
  }
  auto* tab_helper =
      hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents_);
  if (tab_helper) {
    tab_helper->AddSolanaConnectedAccount(host_id_, account);
  }
}

void HnsWalletProviderDelegateImpl::RemoveSolanaConnectedAccount(
    const std::string& account) {
  if (!web_contents_) {
    return;
  }
  auto* tab_helper =
      hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents_);
  if (tab_helper) {
    tab_helper->RemoveSolanaConnectedAccount(host_id_, account);
  }
}

bool HnsWalletProviderDelegateImpl::IsSolanaAccountConnected(
    const std::string& account) {
  if (!web_contents_) {
    return false;
  }
  auto* tab_helper =
      hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents_);
  if (!tab_helper) {
    return false;
  }

  return tab_helper->IsSolanaAccountConnected(host_id_, account);
}

void HnsWalletProviderDelegateImpl::WebContentsDestroyed() {
  web_contents_ = nullptr;
}

// This is for dapp inside iframe navigates away.
void HnsWalletProviderDelegateImpl::RenderFrameHostChanged(
    content::RenderFrameHost* old_host,
    content::RenderFrameHost* new_host) {
  if (!old_host || old_host != content::RenderFrameHost::FromID(host_id_)) {
    return;
  }
  auto* tab_helper =
      hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents_);
  if (tab_helper) {
    tab_helper->ClearSolanaConnectedAccounts(host_id_);
  }
}

void HnsWalletProviderDelegateImpl::PrimaryPageChanged(content::Page& page) {
  if (!web_contents_) {
    return;
  }
  auto* tab_helper =
      hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents_);
  if (tab_helper) {
    tab_helper->ClearSolanaConnectedAccounts(host_id_);
  }
}

}  // namespace hns_wallet
