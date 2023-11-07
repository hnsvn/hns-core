/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_WALLET_HNS_WALLET_SERVICE_DELEGATE_IMPL_H_
#define HNS_BROWSER_HNS_WALLET_HNS_WALLET_SERVICE_DELEGATE_IMPL_H_

#include <memory>
#include <string>

#include "base/containers/flat_map.h"
#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "hns/browser/hns_wallet/external_wallets_importer.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service_delegate.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "chrome/browser/ui/browser_tab_strip_tracker.h"
#include "chrome/browser/ui/browser_tab_strip_tracker_delegate.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"

namespace content {
class BrowserContext;
class WebContents;
}  // namespace content

namespace hns_wallet {

class HnsWalletServiceDelegateImpl : public HnsWalletServiceDelegate,
                                       public TabStripModelObserver,
                                       public BrowserTabStripTrackerDelegate {
 public:
  explicit HnsWalletServiceDelegateImpl(content::BrowserContext* context);
  HnsWalletServiceDelegateImpl(const HnsWalletServiceDelegateImpl&) =
      delete;
  HnsWalletServiceDelegateImpl& operator=(
      const HnsWalletServiceDelegateImpl&) = delete;
  ~HnsWalletServiceDelegateImpl() override;

  static void SetActiveWebContentsForTesting(
      content::WebContents* web_contents);

  void IsExternalWalletInstalled(mojom::ExternalWalletType,
                                 IsExternalWalletInstalledCallback) override;
  void IsExternalWalletInitialized(
      mojom::ExternalWalletType,
      IsExternalWalletInitializedCallback) override;
  void GetImportInfoFromExternalWallet(mojom::ExternalWalletType type,
                                       const std::string& password,
                                       GetImportInfoCallback callback) override;

  bool AddPermission(mojom::CoinType coin,
                     const url::Origin& origin,
                     const std::string& account) override;
  bool HasPermission(mojom::CoinType coin,
                     const url::Origin& origin,
                     const std::string& account) override;
  bool ResetPermission(mojom::CoinType coin,
                       const url::Origin& origin,
                       const std::string& account) override;
  bool IsPermissionDenied(mojom::CoinType coin,
                          const url::Origin& origin) override;

  absl::optional<url::Origin> GetActiveOrigin() override;

  void ClearWalletUIStoragePartition() override;

  void AddObserver(HnsWalletServiceDelegate::Observer* observer) override;
  void RemoveObserver(HnsWalletServiceDelegate::Observer* observer) override;

  // TabStripModelObserver:
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;
  void TabChangedAt(content::WebContents* contents,
                    int index,
                    TabChangeType change_type) override;

  // BrowserTabStripTrackerDelegate:
  bool ShouldTrackBrowser(Browser* browser) override;

 private:
  friend class HnsWalletServiceDelegateImplUnitTest;

  void ContinueIsExternalWalletInitialized(mojom::ExternalWalletType type,
                                           IsExternalWalletInitializedCallback,
                                           bool init_success);
  void ContinueGetImportInfoFromExternalWallet(mojom::ExternalWalletType type,
                                               const std::string& password,
                                               GetImportInfoCallback callback,
                                               bool init_success);

  absl::optional<url::Origin> GetActiveOriginInternal();
  void FireActiveOriginChanged();

  raw_ptr<content::BrowserContext> context_ = nullptr;
  base::flat_map<mojom::ExternalWalletType,
                 std::unique_ptr<ExternalWalletsImporter>>
      importers_;

  BrowserTabStripTracker browser_tab_strip_tracker_;
  base::ObserverList<HnsWalletServiceDelegate::Observer> observer_list_;

  base::WeakPtrFactory<HnsWalletServiceDelegateImpl> weak_ptr_factory_;
};

}  // namespace hns_wallet

#endif  // HNS_BROWSER_HNS_WALLET_HNS_WALLET_SERVICE_DELEGATE_IMPL_H_
