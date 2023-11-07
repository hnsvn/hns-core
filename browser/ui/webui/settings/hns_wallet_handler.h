/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_WALLET_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_WALLET_HANDLER_H_

#include <string>
#include <utility>

#include "base/memory/weak_ptr.h"
#include "hns/browser/hns_wallet/hns_wallet_pin_service_factory.h"
#include "hns/components/hns_wallet/browser/hns_wallet_pin_service.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "chrome/browser/ui/webui/settings/settings_page_ui_handler.h"

class PrefService;
class Profile;
class TestHnsWalletHandler;

class HnsWalletHandler : public settings::SettingsPageUIHandler {
 public:
  HnsWalletHandler();
  ~HnsWalletHandler() override;
  HnsWalletHandler(const HnsWalletHandler&) = delete;
  HnsWalletHandler& operator=(const HnsWalletHandler&) = delete;

  void SetChainCallbackForTesting(base::OnceClosure callback) {
    chain_callback_for_testing_ = std::move(callback);
  }

 private:
  friend TestHnsWalletHandler;
  // SettingsPageUIHandler overrides:
  void RegisterMessages() override;
  void OnJavascriptAllowed() override {}
  void OnJavascriptDisallowed() override {}

  void GetAutoLockMinutes(const base::Value::List& args);
  void GetSolanaProviderOptions(const base::Value::List& args);
  void RemoveChain(const base::Value::List& args);
  void ResetChain(const base::Value::List& args);
  void GetNetworksList(const base::Value::List& args);
  void GetPrepopulatedNetworksList(const base::Value::List& args);
  void AddChain(const base::Value::List& args);
  void SetDefaultNetwork(const base::Value::List& args);
  void AddHiddenNetwork(const base::Value::List& args);
  void RemoveHiddenNetwork(const base::Value::List& args);
  void IsNftPinningEnabled(const base::Value::List& args);
  void GetPinnedNftCount(const base::Value::List& args);
  void ClearPinnedNft(const base::Value::List& args);

  PrefService* GetPrefs();
  hns_wallet::HnsWalletPinService* GetHnsWalletPinService();

  void OnAddChain(base::Value javascript_callback,
                  const std::string& chain_id,
                  hns_wallet::mojom::ProviderError error,
                  const std::string& error_message);
  void OnHnsWalletPinServiceReset(base::Value javascript_callback,
                                    bool result);

  base::OnceClosure chain_callback_for_testing_;
  base::WeakPtrFactory<HnsWalletHandler> weak_ptr_factory_{this};
};

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_WALLET_HANDLER_H_
