/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/ios/browser/api/hns_wallet/hns_wallet_api.h"

#include "base/strings/sys_string_conversions.h"
#include "hns/components/hns_wallet/browser/blockchain_registry.h"
#include "hns/components/hns_wallet/browser/ethereum_provider_impl.h"
#include "hns/components/hns_wallet/browser/solana_provider_impl.h"
#include "hns/components/hns_wallet/resources/grit/hns_wallet_script_generated.h"
#include "hns/ios/browser/api/hns_wallet/hns_wallet.mojom.objc+private.h"
#include "hns/ios/browser/api/hns_wallet/hns_wallet_provider_delegate_ios+private.h"
#include "hns/ios/browser/api/hns_wallet/hns_wallet_provider_delegate_ios.h"
#include "hns/ios/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/ios/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/ios/browser/hns_wallet/keyring_service_factory.h"
#include "hns/ios/browser/hns_wallet/tx_service_factory.h"
#include "components/grit/hns_components_resources.h"
#include "ios/chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/shared/model/browser_state/chrome_browser_state_manager.h"
#include "ui/base/resource/resource_bundle.h"

HnsWalletProviderScriptKey const HnsWalletProviderScriptKeyEthereum =
    @"ethereum_provider.js";
HnsWalletProviderScriptKey const HnsWalletProviderScriptKeySolana =
    @"solana_provider.js";
HnsWalletProviderScriptKey const HnsWalletProviderScriptKeySolanaWeb3 =
    @"solana_web3.js";
HnsWalletProviderScriptKey const HnsWalletProviderScriptKeyWalletStandard =
    @"wallet_standard.js";

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

@implementation HnsWalletAPI {
  ChromeBrowserState* _mainBrowserState;  // NOT OWNED
  NSMutableDictionary<NSNumber* /* HnsWalletCoinType */,
                      NSDictionary<HnsWalletProviderScriptKey, NSString*>*>*
      _providerScripts;
}

- (instancetype)initWithBrowserState:(ChromeBrowserState*)mainBrowserState {
  if ((self = [super init])) {
    _mainBrowserState = mainBrowserState;
    _providerScripts = [[NSMutableDictionary alloc] init];
  }
  return self;
}

+ (id<HnsWalletBlockchainRegistry>)blockchainRegistry {
  auto* registry = hns_wallet::BlockchainRegistry::GetInstance();
  return [[HnsWalletBlockchainRegistryMojoImpl alloc]
      initWithBlockchainRegistry:registry->MakeRemote()];
}

- (nullable id<HnsWalletEthereumProvider>)
    ethereumProviderWithDelegate:(id<HnsWalletProviderDelegate>)delegate
               isPrivateBrowsing:(bool)isPrivateBrowsing {
  auto* browserState = _mainBrowserState;
  if (isPrivateBrowsing) {
    browserState = browserState->GetOffTheRecordChromeBrowserState();
  }

  auto* json_rpc_service =
      hns_wallet::JsonRpcServiceFactory::GetServiceForState(browserState);
  if (!json_rpc_service) {
    return nil;
  }

  auto* tx_service =
      hns_wallet::TxServiceFactory::GetServiceForState(browserState);
  if (!tx_service) {
    return nil;
  }

  auto* keyring_service =
      hns_wallet::KeyringServiceFactory::GetServiceForState(browserState);
  if (!keyring_service) {
    return nil;
  }

  auto* hns_wallet_service =
      hns_wallet::HnsWalletServiceFactory::GetServiceForState(browserState);
  if (!hns_wallet_service) {
    return nil;
  }

  auto provider = std::make_unique<hns_wallet::EthereumProviderImpl>(
      ios::HostContentSettingsMapFactory::GetForBrowserState(browserState),
      json_rpc_service, tx_service, keyring_service, hns_wallet_service,
      std::make_unique<hns_wallet::HnsWalletProviderDelegateBridge>(
          delegate),
      browserState->GetPrefs());
  return [[HnsWalletEthereumProviderOwnedImpl alloc]
      initWithEthereumProvider:std::move(provider)];
}

- (nullable id<HnsWalletSolanaProvider>)
    solanaProviderWithDelegate:(id<HnsWalletProviderDelegate>)delegate
             isPrivateBrowsing:(bool)isPrivateBrowsing {
  auto* browserState = _mainBrowserState;
  if (isPrivateBrowsing) {
    browserState = browserState->GetOffTheRecordChromeBrowserState();
  }

  auto* keyring_service =
      hns_wallet::KeyringServiceFactory::GetServiceForState(browserState);
  if (!keyring_service) {
    return nil;
  }

  auto* hns_wallet_service =
      hns_wallet::HnsWalletServiceFactory::GetServiceForState(browserState);
  if (!hns_wallet_service) {
    return nil;
  }

  auto* tx_service =
      hns_wallet::TxServiceFactory::GetServiceForState(browserState);
  if (!tx_service) {
    return nil;
  }

  auto* json_rpc_service =
      hns_wallet::JsonRpcServiceFactory::GetServiceForState(browserState);
  if (!json_rpc_service) {
    return nil;
  }
  auto* host_content_settings_map =
      ios::HostContentSettingsMapFactory::GetForBrowserState(browserState);
  if (!host_content_settings_map) {
    return nil;
  }

  auto provider = std::make_unique<hns_wallet::SolanaProviderImpl>(
      *host_content_settings_map, keyring_service, hns_wallet_service,
      tx_service, json_rpc_service,
      std::make_unique<hns_wallet::HnsWalletProviderDelegateBridge>(
          delegate));
  return [[HnsWalletSolanaProviderOwnedImpl alloc]
      initWithSolanaProvider:std::move(provider)];
}

- (NSString*)resourceForID:(int)resource_id {
  // The resource bundle is not available until after WebMainParts is setup
  auto& resource_bundle = ui::ResourceBundle::GetSharedInstance();
  std::string resource_string = "";
  if (resource_bundle.IsGzipped(resource_id)) {
    resource_string =
        std::string(resource_bundle.LoadDataResourceString(resource_id));
  } else {
    resource_string =
        std::string(resource_bundle.GetRawDataResource(resource_id));
  }
  return base::SysUTF8ToNSString(resource_string);
}

- (NSDictionary<HnsWalletProviderScriptKey, NSString*>*)
    providerScriptsForCoinType:(HnsWalletCoinType)coinType {
  auto cachedScript = _providerScripts[@(coinType)];
  if (cachedScript) {
    return cachedScript;
  }
  auto resource_ids =
      ^std::vector<std::pair<HnsWalletProviderScriptKey, int>> {
    switch (coinType) {
      case HnsWalletCoinTypeEth:
        return {std::make_pair(
            HnsWalletProviderScriptKeyEthereum,
            IDR_HNS_WALLET_SCRIPT_ETHEREUM_PROVIDER_SCRIPT_BUNDLE_JS)};
      case HnsWalletCoinTypeSol:
        return {std::make_pair(
                    HnsWalletProviderScriptKeySolana,
                    IDR_HNS_WALLET_SCRIPT_SOLANA_PROVIDER_SCRIPT_BUNDLE_JS),
                std::make_pair(HnsWalletProviderScriptKeySolanaWeb3,
                               IDR_HNS_WALLET_SOLANA_WEB3_JS),
                std::make_pair(HnsWalletProviderScriptKeyWalletStandard,
                               IDR_HNS_WALLET_STANDARD_JS)};
      case HnsWalletCoinTypeFil:
        // Currently not supported
        return {std::make_pair(@"", 0)};
      case HnsWalletCoinTypeBtc:
        // Currently not supported
        return {std::make_pair(@"", 0)};
    }
    return {std::make_pair(@"", 0)};
  }
  ();
  const auto scripts = [[NSMutableDictionary alloc] init];
  for (auto resource : resource_ids) {
    auto key = resource.first;
    auto resource_id = resource.second;
    scripts[key] = [self resourceForID:resource_id];
  }
  _providerScripts[@(coinType)] = [scripts copy];
  return scripts;
}

@end
