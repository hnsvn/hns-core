/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_wallet_handler.h"

#include <string>
#include <utility>
#include <vector>

#include "base/feature_list.h"
#include "base/functional/bind.h"
#include "base/notreached.h"
#include "base/values.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/components/hns_wallet/browser/blockchain_registry.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/hns_wallet/common/value_conversion_utils.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_ui.h"
#include "ui/base/l10n/l10n_util.h"

HnsWalletHandler::HnsWalletHandler() = default;
HnsWalletHandler::~HnsWalletHandler() = default;

namespace {

base::Value::Dict MakeSelectValue(const std::u16string& name,
                                  ::hns_wallet::mojom::DefaultWallet value) {
  base::Value::Dict item;
  item.Set("value", static_cast<int>(value));
  item.Set("name", name);
  return item;
}

absl::optional<hns_wallet::mojom::CoinType> ToCoinType(
    absl::optional<int> val) {
  if (!val) {
    return absl::nullopt;
  }
  auto result = static_cast<hns_wallet::mojom::CoinType>(*val);
  if (result != hns_wallet::mojom::CoinType::ETH &&
      result != hns_wallet::mojom::CoinType::FIL &&
      result != hns_wallet::mojom::CoinType::SOL) {
    NOTREACHED();
    return absl::nullopt;
  }
  return result;
}

}  // namespace

void HnsWalletHandler::RegisterMessages() {
  web_ui()->RegisterMessageCallback(
      "getAutoLockMinutes",
      base::BindRepeating(&HnsWalletHandler::GetAutoLockMinutes,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getSolanaProviderOptions",
      base::BindRepeating(&HnsWalletHandler::GetSolanaProviderOptions,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "removeChain", base::BindRepeating(&HnsWalletHandler::RemoveChain,
                                         base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "resetChain", base::BindRepeating(&HnsWalletHandler::ResetChain,
                                        base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getNetworksList",
      base::BindRepeating(&HnsWalletHandler::GetNetworksList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getPrepopulatedNetworksList",
      base::BindRepeating(&HnsWalletHandler::GetPrepopulatedNetworksList,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "addChain", base::BindRepeating(&HnsWalletHandler::AddChain,
                                      base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "setDefaultNetwork",
      base::BindRepeating(&HnsWalletHandler::SetDefaultNetwork,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "addHiddenNetwork",
      base::BindRepeating(&HnsWalletHandler::AddHiddenNetwork,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "removeHiddenNetwork",
      base::BindRepeating(&HnsWalletHandler::RemoveHiddenNetwork,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "isNftPinningEnabled",
      base::BindRepeating(&HnsWalletHandler::IsNftPinningEnabled,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "getPinnedNftCount",
      base::BindRepeating(&HnsWalletHandler::GetPinnedNftCount,
                          base::Unretained(this)));
  web_ui()->RegisterMessageCallback(
      "clearPinnedNft", base::BindRepeating(&HnsWalletHandler::ClearPinnedNft,
                                            base::Unretained(this)));
}

void HnsWalletHandler::GetAutoLockMinutes(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(
      args[0],
      base::Value(GetPrefs()->GetInteger(kHnsWalletAutoLockMinutes)));
}

void HnsWalletHandler::GetSolanaProviderOptions(
    const base::Value::List& args) {
  base::Value::List list;
  list.Append(MakeSelectValue(
      hns_l10n::GetLocalizedResourceUTF16String(
          IDS_HNS_WALLET_WEB3_PROVIDER_HNS_PREFER_EXTENSIONS),
      ::hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension));
  list.Append(
      MakeSelectValue(hns_l10n::GetLocalizedResourceUTF16String(
                          IDS_HNS_WALLET_WEB3_PROVIDER_HNS),
                      ::hns_wallet::mojom::DefaultWallet::HnsWallet));
  list.Append(MakeSelectValue(hns_l10n::GetLocalizedResourceUTF16String(
                                  IDS_HNS_WALLET_WEB3_PROVIDER_NONE),
                              ::hns_wallet::mojom::DefaultWallet::None));
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args[0], list);
}

void HnsWalletHandler::RemoveChain(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);
  AllowJavascript();

  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  hns_wallet::RemoveCustomNetwork(GetPrefs(), *chain_id, *coin);
  ResolveJavascriptCallback(args[0], base::Value(true));
}

void HnsWalletHandler::ResetChain(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);
  PrefService* prefs = GetPrefs();
  AllowJavascript();

  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  DCHECK(hns_wallet::CustomChainExists(prefs, *chain_id, *coin));
  hns_wallet::RemoveCustomNetwork(prefs, *chain_id, *coin);
  DCHECK(hns_wallet::KnownChainExists(*chain_id, *coin));
  ResolveJavascriptCallback(args[0], base::Value(true));
}

void HnsWalletHandler::GetNetworksList(const base::Value::List& args) {
  CHECK_EQ(args.size(), 2U);
  PrefService* prefs = GetPrefs();

  base::Value::Dict result;
  auto coin = ToCoinType(args[1].GetIfInt());
  if (!coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  result.Set("defaultNetwork",
             hns_wallet::GetCurrentChainId(prefs, *coin, absl::nullopt));

  auto& networks = result.Set("networks", base::Value::List())->GetList();
  for (const auto& it : hns_wallet::GetAllChains(prefs, *coin)) {
    networks.Append(hns_wallet::NetworkInfoToValue(*it));
  }
  auto& knownNetworks =
      result.Set("knownNetworks", base::Value::List())->GetList();
  for (const auto& it : hns_wallet::GetAllKnownChains(prefs, *coin)) {
    knownNetworks.Append(it->chain_id);
  }

  auto& customNetworks =
      result.Set("customNetworks", base::Value::List())->GetList();
  for (const auto& it : hns_wallet::GetAllCustomChains(prefs, *coin)) {
    customNetworks.Append(it->chain_id);
  }

  auto& hiddenNetworks =
      result.Set("hiddenNetworks", base::Value::List())->GetList();
  for (const auto& it : hns_wallet::GetHiddenNetworks(prefs, *coin)) {
    hiddenNetworks.Append(it);
  }

  AllowJavascript();
  ResolveJavascriptCallback(args[0], result);
}

void HnsWalletHandler::GetPrepopulatedNetworksList(
    const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();

  base::Value::List networks;

  auto* blockchain_registry = hns_wallet::BlockchainRegistry::GetInstance();
  if (!blockchain_registry) {
    ResolveJavascriptCallback(args[0], networks);
    return;
  }

  for (const auto& it : blockchain_registry->GetPrepopulatedNetworks()) {
    networks.Append(hns_wallet::NetworkInfoToValue(*it));
  }

  ResolveJavascriptCallback(args[0], networks);
}

void HnsWalletHandler::OnAddChain(base::Value javascript_callback,
                                    const std::string& chain_id,
                                    hns_wallet::mojom::ProviderError error,
                                    const std::string& error_message) {
  base::Value::List result;
  result.Append(error == hns_wallet::mojom::ProviderError::kSuccess);
  result.Append(error_message);
  ResolveJavascriptCallback(javascript_callback, result);
  if (chain_callback_for_testing_) {
    std::move(chain_callback_for_testing_).Run();
  }
}

void HnsWalletHandler::AddChain(const base::Value::List& args) {
  CHECK_EQ(args.size(), 2U);
  AllowJavascript();
  auto* json_rpc_service =
      hns_wallet::JsonRpcServiceFactory::GetServiceForContext(
          Profile::FromWebUI(web_ui()));

  hns_wallet::mojom::NetworkInfoPtr chain =
      hns_wallet::ValueToNetworkInfo(args[1]);

  if (!chain || !json_rpc_service) {
    base::Value::List result;
    result.Append(false);
    result.Append(l10n_util::GetStringUTF8(
        IDS_SETTINGS_WALLET_NETWORKS_SUMBISSION_FAILED));
    ResolveJavascriptCallback(args[0], result);
    return;
  }

  json_rpc_service->AddChain(
      std::move(chain),
      base::BindOnce(&HnsWalletHandler::OnAddChain,
                     weak_ptr_factory_.GetWeakPtr(), args[0].Clone()));
}

void HnsWalletHandler::SetDefaultNetwork(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);

  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  AllowJavascript();
  auto* json_rpc_service =
      hns_wallet::JsonRpcServiceFactory::GetServiceForContext(
          Profile::FromWebUI(web_ui()));
  auto result = json_rpc_service ? json_rpc_service->SetNetwork(
                                       *chain_id, *coin, absl::nullopt)
                                 : false;
  ResolveJavascriptCallback(args[0], base::Value(result));
}

void HnsWalletHandler::AddHiddenNetwork(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);
  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  PrefService* prefs = GetPrefs();
  AllowJavascript();
  hns_wallet::AddHiddenNetwork(prefs, *coin, *chain_id);
  ResolveJavascriptCallback(args[0], base::Value(true));
}

void HnsWalletHandler::RemoveHiddenNetwork(const base::Value::List& args) {
  CHECK_EQ(args.size(), 3U);
  auto* chain_id = args[1].GetIfString();
  auto coin = ToCoinType(args[2].GetIfInt());
  if (!chain_id || !coin) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }

  PrefService* prefs = GetPrefs();
  AllowJavascript();
  hns_wallet::RemoveHiddenNetwork(prefs, *coin, *chain_id);
  ResolveJavascriptCallback(args[0], base::Value(true));
}

PrefService* HnsWalletHandler::GetPrefs() {
  return Profile::FromWebUI(web_ui())->GetPrefs();
}

hns_wallet::HnsWalletPinService*
HnsWalletHandler::GetHnsWalletPinService() {
  return hns_wallet::HnsWalletPinServiceFactory::GetInstance()
      ->GetServiceForContext(Profile::FromWebUI(web_ui()));
}

void HnsWalletHandler::IsNftPinningEnabled(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  ResolveJavascriptCallback(args[0],
                            base::Value(::hns_wallet::IsNftPinningEnabled()));
}

void HnsWalletHandler::GetPinnedNftCount(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();

  auto* service = GetHnsWalletPinService();
  if (!service) {
    ResolveJavascriptCallback(args[0], base::Value());
    return;
  }
  ResolveJavascriptCallback(
      args[0], base::Value(static_cast<int>(service->GetPinnedTokensCount())));
}

void HnsWalletHandler::ClearPinnedNft(const base::Value::List& args) {
  CHECK_EQ(args.size(), 1U);
  AllowJavascript();
  auto* service = GetHnsWalletPinService();
  service->Reset(
      base::BindOnce(&HnsWalletHandler::OnHnsWalletPinServiceReset,
                     weak_ptr_factory_.GetWeakPtr(), args[0].Clone()));
}

void HnsWalletHandler::OnHnsWalletPinServiceReset(
    base::Value javascript_callback,
    bool result) {
  ResolveJavascriptCallback(javascript_callback, base::Value(result));
}
