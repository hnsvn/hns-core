/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/api/hns_wallet_api.h"

#include <memory>
#include <string>

#include "base/feature_list.h"
#include "base/json/json_writer.h"
#include "base/values.h"
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_service.h"
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_service_factory.h"
#include "hns/browser/ethereum_remote_client/features.h"
#include "hns/browser/ethereum_remote_client/pref_names.h"
#include "hns/browser/extensions/ethereum_remote_client_util.h"
#include "hns/common/extensions/api/hns_wallet.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/grit/hns_generated_resources.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/profiles/profile.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/web_contents.h"
#include "extensions/browser/extension_prefs.h"
#include "extensions/browser/extension_util.h"
#include "extensions/common/constants.h"

namespace {

EthereumRemoteClientService* GetEthereumRemoteClientService(
    content::BrowserContext* context) {
  return EthereumRemoteClientServiceFactory::GetInstance()->GetForContext(
      context);
}

base::Value::Dict MakeSelectValue(const std::u16string& name,
                                  ::hns_wallet::mojom::DefaultWallet value) {
  base::Value::Dict item;
  item.Set("value", base::Value(static_cast<int>(value)));
  item.Set("name", base::Value(name));
  return item;
}

}  // namespace

namespace extensions {
namespace api {

ExtensionFunction::ResponseAction HnsWalletReadyFunction::Run() {
  if (browser_context()->IsTor()) {
    return RespondNow(Error("Not available in Tor context"));
  }

  auto* service = GetEthereumRemoteClientService(browser_context());
  service->CryptoWalletsExtensionReady();
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction HnsWalletNotifyWalletUnlockFunction::Run() {
  if (browser_context()->IsTor()) {
    return RespondNow(Error("Not available in Tor context"));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  ::hns_wallet::UpdateLastUnlockPref(profile->GetPrefs());

  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction HnsWalletLoadUIFunction::Run() {
  auto* service = GetEthereumRemoteClientService(browser_context());
  // If the extension is already ready, respond right away
  if (service->IsCryptoWalletsReady()) {
    return RespondNow(NoArguments());
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto* prefs = profile->GetPrefs();
  prefs->SetBoolean(kERCOptedIntoCryptoWallets, true);
  service->MaybeLoadCryptoWalletsExtension(
      base::BindOnce(&HnsWalletLoadUIFunction::OnLoaded, this));
  return RespondLater();
}

void HnsWalletLoadUIFunction::OnLoaded() {
  Respond(NoArguments());
}

ExtensionFunction::ResponseAction
HnsWalletShouldPromptForSetupFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  bool should_prompt =
      !profile->GetPrefs()->GetBoolean(kERCOptedIntoCryptoWallets);
  return RespondNow(WithArguments(should_prompt));
}

ExtensionFunction::ResponseAction HnsWalletGetWalletSeedFunction::Run() {
  // make sure the passed in enryption key is 32 bytes.
  absl::optional<hns_wallet::GetWalletSeed::Params> params =
      hns_wallet::GetWalletSeed::Params::Create(args());
  EXTENSION_FUNCTION_VALIDATE(params);
  if (params->key.size() != 32) {
    return RespondNow(Error("Invalid input key size"));
  }

  auto* service = GetEthereumRemoteClientService(browser_context());

  base::Value::BlobStorage blob;
  std::string derived = service->GetWalletSeed(params->key);

  if (derived.empty()) {
    return RespondNow(Error("Error getting wallet seed"));
  }

  blob.assign(derived.begin(), derived.end());

  return RespondNow(WithArguments(base::Value(blob)));
}

ExtensionFunction::ResponseAction HnsWalletGetProjectIDFunction::Run() {
  std::string project_id = extensions::GetInfuraProjectID();
  return RespondNow(WithArguments(project_id));
}

ExtensionFunction::ResponseAction HnsWalletGetHnsKeyFunction::Run() {
  std::string hns_key = extensions::GetHnsKey();
  return RespondNow(WithArguments(hns_key));
}

ExtensionFunction::ResponseAction HnsWalletResetWalletFunction::Run() {
  auto* service = GetEthereumRemoteClientService(browser_context());
  service->ResetCryptoWallets();
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction HnsWalletGetWeb3ProviderFunction::Run() {
  Profile* profile = Profile::FromBrowserContext(browser_context());
  auto default_wallet =
      ::hns_wallet::GetDefaultEthereumWallet(profile->GetPrefs());
  std::string extension_id;
  // This API is used so an extension can know when to prompt to
  // be the default Dapp provider. Since the new wallet is not an
  // extension at all, we can just re-use the Crypto Wallets ID.
  // We also don't want to prompt in Crypto Wallets when it's set
  // to Hns Wallet.
  if (default_wallet == ::hns_wallet::mojom::DefaultWallet::HnsWallet ||
      default_wallet ==
          ::hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension ||
      default_wallet == ::hns_wallet::mojom::DefaultWallet::CryptoWallets) {
    extension_id = ethereum_remote_client_extension_id;
  }
  return RespondNow(WithArguments(extension_id));
}

ExtensionFunction::ResponseAction
HnsWalletGetWeb3ProviderListFunction::Run() {
  base::Value::List list;
  list.Append(MakeSelectValue(
      hns_l10n::GetLocalizedResourceUTF16String(
          IDS_HNS_WALLET_WEB3_PROVIDER_HNS_PREFER_EXTENSIONS),
      ::hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension));

  list.Append(
      MakeSelectValue(hns_l10n::GetLocalizedResourceUTF16String(
                          IDS_HNS_WALLET_WEB3_PROVIDER_HNS),
                      ::hns_wallet::mojom::DefaultWallet::HnsWallet));

  if (base::FeatureList::IsEnabled(ethereum_remote_client::features::
                                       kCryptoWalletsForNewInstallsFeature) ||
      extensions::ExtensionPrefs::Get(browser_context())
          ->HasPrefForExtension(ethereum_remote_client_extension_id)) {
    list.Append(MakeSelectValue(
        hns_l10n::GetLocalizedResourceUTF16String(
            IDS_HNS_WALLET_WEB3_PROVIDER_CRYPTO_WALLETS_DEPRECATED),
        ::hns_wallet::mojom::DefaultWallet::CryptoWallets));
  }

  list.Append(MakeSelectValue(hns_l10n::GetLocalizedResourceUTF16String(
                                  IDS_HNS_WALLET_WEB3_PROVIDER_NONE),
                              ::hns_wallet::mojom::DefaultWallet::None));
  std::string json_string;
  base::JSONWriter::Write(list, &json_string);
  return RespondNow(WithArguments(json_string));
}

ExtensionFunction::ResponseAction
HnsWalletIsNativeWalletEnabledFunction::Run() {
  return RespondNow(WithArguments(::hns_wallet::IsNativeWalletEnabled()));
}

}  // namespace api
}  // namespace extensions
