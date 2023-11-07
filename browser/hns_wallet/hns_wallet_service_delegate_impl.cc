/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_wallet/hns_wallet_service_delegate_impl.h"

#include <utility>

#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/permission_utils.h"
#include "hns/components/constants/webui_url_constants.h"
#include "hns/components/permissions/contexts/hns_wallet_permission_context.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/browser/web_contents.h"
#include "url/gurl.h"
#include "url/origin.h"

using content::StoragePartition;

namespace hns_wallet {

namespace {

content::WebContents* g_web_contents_for_testing = nullptr;

content::WebContents* GetActiveWebContents() {
  if (g_web_contents_for_testing) {
    return g_web_contents_for_testing;
  }

  Browser* browser = chrome::FindLastActive();
  return browser ? browser->tab_strip_model()->GetActiveWebContents() : nullptr;
}

void ClearWalletStoragePartition(content::BrowserContext* context,
                                 const GURL& url) {
  CHECK(context);
  auto* partition = context->GetDefaultStoragePartition();
  partition->ClearDataForOrigin(
      StoragePartition::REMOVE_DATA_MASK_ALL,
      StoragePartition::QUOTA_MANAGED_STORAGE_MASK_ALL, url, base::DoNothing());
}

}  // namespace

HnsWalletServiceDelegateImpl::HnsWalletServiceDelegateImpl(
    content::BrowserContext* context)
    : context_(context),
      browser_tab_strip_tracker_(this, this),
      weak_ptr_factory_(this) {
  browser_tab_strip_tracker_.Init();
}

HnsWalletServiceDelegateImpl::~HnsWalletServiceDelegateImpl() = default;

// static
void HnsWalletServiceDelegateImpl::SetActiveWebContentsForTesting(
    content::WebContents* web_contents) {
  g_web_contents_for_testing = web_contents;
}

void HnsWalletServiceDelegateImpl::AddObserver(
    HnsWalletServiceDelegate::Observer* observer) {
  observer_list_.AddObserver(observer);
}

void HnsWalletServiceDelegateImpl::RemoveObserver(
    HnsWalletServiceDelegate::Observer* observer) {
  observer_list_.RemoveObserver(observer);
}

bool HnsWalletServiceDelegateImpl::ShouldTrackBrowser(Browser* browser) {
  return browser->profile() == Profile::FromBrowserContext(context_);
}

void HnsWalletServiceDelegateImpl::IsExternalWalletInstalled(
    mojom::ExternalWalletType type,
    IsExternalWalletInstalledCallback callback) {
  ExternalWalletsImporter importer(type, context_);
  std::move(callback).Run(importer.IsExternalWalletInstalled());
}

void HnsWalletServiceDelegateImpl::IsExternalWalletInitialized(
    mojom::ExternalWalletType type,
    IsExternalWalletInitializedCallback callback) {
  importers_[type] = std::make_unique<ExternalWalletsImporter>(type, context_);
  // Do not try to init the importer when external wallet is not installed
  if (!importers_[type]->IsExternalWalletInstalled()) {
    std::move(callback).Run(false);
    return;
  }
  if (importers_[type]->IsInitialized()) {
    ContinueIsExternalWalletInitialized(type, std::move(callback), true);
  } else {
    importers_[type]->Initialize(base::BindOnce(
        &HnsWalletServiceDelegateImpl::ContinueIsExternalWalletInitialized,
        weak_ptr_factory_.GetWeakPtr(), type, std::move(callback)));
  }
}

void HnsWalletServiceDelegateImpl::ContinueIsExternalWalletInitialized(
    mojom::ExternalWalletType type,
    IsExternalWalletInitializedCallback callback,
    bool init_success) {
  DCHECK(importers_[type]);
  if (init_success) {
    std::move(callback).Run(importers_[type]->IsExternalWalletInitialized());
  } else {
    std::move(callback).Run(false);
  }
}

void HnsWalletServiceDelegateImpl::GetImportInfoFromExternalWallet(
    mojom::ExternalWalletType type,
    const std::string& password,
    GetImportInfoCallback callback) {
  if (!importers_[type]) {
    importers_[type] =
        std::make_unique<ExternalWalletsImporter>(type, context_);
  }
  if (importers_[type]->IsInitialized()) {
    ContinueGetImportInfoFromExternalWallet(type, password, std::move(callback),
                                            true);
  } else {
    importers_[type]->Initialize(base::BindOnce(
        &HnsWalletServiceDelegateImpl::
            ContinueGetImportInfoFromExternalWallet,
        weak_ptr_factory_.GetWeakPtr(), type, password, std::move(callback)));
  }
}

void HnsWalletServiceDelegateImpl::ContinueGetImportInfoFromExternalWallet(
    mojom::ExternalWalletType type,
    const std::string& password,
    GetImportInfoCallback callback,
    bool init_success) {
  DCHECK(importers_[type]);
  if (init_success) {
    DCHECK(importers_[type]->IsInitialized());
    importers_[type]->GetImportInfo(password, std::move(callback));
  } else {
    std::move(callback).Run(false, ImportInfo(), ImportError::kInternalError);
  }
}

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

void HnsWalletServiceDelegateImpl::OnTabStripModelChanged(
    TabStripModel* tab_strip_model,
    const TabStripModelChange& change,
    const TabStripSelectionChange& selection) {
  FireActiveOriginChanged();
}

void HnsWalletServiceDelegateImpl::TabChangedAt(
    content::WebContents* contents,
    int index,
    TabChangeType change_type) {
  if (!contents || contents != GetActiveWebContents()) {
    return;
  }

  FireActiveOriginChanged();
}

void HnsWalletServiceDelegateImpl::FireActiveOriginChanged() {
  mojom::OriginInfoPtr origin_info =
      MakeOriginInfo(GetActiveOriginInternal().value_or(url::Origin()));
  for (auto& observer : observer_list_) {
    observer.OnActiveOriginChanged(origin_info);
  }
}

absl::optional<url::Origin>
HnsWalletServiceDelegateImpl::GetActiveOriginInternal() {
  content::WebContents* contents = GetActiveWebContents();
  return contents ? contents->GetPrimaryMainFrame()->GetLastCommittedOrigin()
                  : absl::optional<url::Origin>();
}

absl::optional<url::Origin> HnsWalletServiceDelegateImpl::GetActiveOrigin() {
  return GetActiveOriginInternal();
}

void HnsWalletServiceDelegateImpl::ClearWalletUIStoragePartition() {
  ClearWalletStoragePartition(context_, GURL(kHnsUIWalletURL));
  ClearWalletStoragePartition(context_, GURL(kHnsUIWalletPanelURL));
}

}  // namespace hns_wallet
