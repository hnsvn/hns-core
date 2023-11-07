/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/hns_wallet_prefs.h"

#include <string>
#include <utility>
#include <vector>

#include "base/values.h"
#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/browser/tx_state_manager.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/pref_names.h"
#include "hns/components/p3a_utils/feature_usage.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/scoped_user_pref_update.h"
#include "components/sync_preferences/pref_service_syncable.h"

namespace hns_wallet {

namespace {

constexpr int kDefaultWalletAutoLockMinutes = 10;

base::Value::Dict GetDefaultUserAssets() {
  base::Value::Dict user_assets_pref;
  user_assets_pref.Set(kEthereumPrefKey,
                       HnsWalletService::GetDefaultEthereumAssets());
  user_assets_pref.Set(kSolanaPrefKey,
                       HnsWalletService::GetDefaultSolanaAssets());
  user_assets_pref.Set(kFilecoinPrefKey,
                       HnsWalletService::GetDefaultFilecoinAssets());
  user_assets_pref.Set(kBitcoinPrefKey,
                       HnsWalletService::GetDefaultBitcoinAssets());
  return user_assets_pref;
}

base::Value::Dict GetDefaultSelectedNetworks() {
  base::Value::Dict selected_networks;
  selected_networks.Set(kEthereumPrefKey, mojom::kMainnetChainId);
  selected_networks.Set(kSolanaPrefKey, mojom::kSolanaMainnet);
  selected_networks.Set(kFilecoinPrefKey, mojom::kFilecoinMainnet);
  selected_networks.Set(kBitcoinPrefKey, mojom::kBitcoinMainnet);

  return selected_networks;
}

base::Value::Dict GetDefaultSelectedNetworksPerOrigin() {
  base::Value::Dict selected_networks;
  selected_networks.Set(kEthereumPrefKey, base::Value::Dict());
  selected_networks.Set(kSolanaPrefKey, base::Value::Dict());
  selected_networks.Set(kFilecoinPrefKey, base::Value::Dict());
  selected_networks.Set(kBitcoinPrefKey, base::Value::Dict());

  return selected_networks;
}

base::Value::Dict GetDefaultHiddenNetworks() {
  base::Value::Dict hidden_networks;

  base::Value::List eth_hidden;
  eth_hidden.Append(mojom::kGoerliChainId);
  eth_hidden.Append(mojom::kSepoliaChainId);
  eth_hidden.Append(mojom::kLocalhostChainId);
  eth_hidden.Append(mojom::kFilecoinEthereumTestnetChainId);
  hidden_networks.Set(kEthereumPrefKey, std::move(eth_hidden));

  base::Value::List fil_hidden;
  fil_hidden.Append(mojom::kFilecoinTestnet);
  fil_hidden.Append(mojom::kLocalhostChainId);
  hidden_networks.Set(kFilecoinPrefKey, std::move(fil_hidden));

  base::Value::List sol_hidden;
  sol_hidden.Append(mojom::kSolanaDevnet);
  sol_hidden.Append(mojom::kSolanaTestnet);
  sol_hidden.Append(mojom::kLocalhostChainId);
  hidden_networks.Set(kSolanaPrefKey, std::move(sol_hidden));

  // TODO(apaymyshev): fix by
  // https://github.com/hnsvn/hns-browser/issues/31662
  /*
  base::Value::List btc_hidden;
  btc_hidden.Append(mojom::kBitcoinTestnet);
  hidden_networks.Set(kBitcoinPrefKey, std::move(btc_hidden));
  */

  return hidden_networks;
}

}  // namespace

void RegisterLocalStatePrefs(PrefRegistrySimple* registry) {
  registry->RegisterTimePref(kHnsWalletLastUnlockTime, base::Time());
  p3a_utils::RegisterFeatureUsagePrefs(
      registry, kHnsWalletP3AFirstUnlockTime, kHnsWalletP3ALastUnlockTime,
      kHnsWalletP3AUsedSecondDay, nullptr, nullptr);
  registry->RegisterBooleanPref(kHnsWalletP3ANewUserBalanceReported, false);
  registry->RegisterIntegerPref(kHnsWalletP3AOnboardingLastStep, 0);
  registry->RegisterBooleanPref(kHnsWalletP3ANFTGalleryUsed, false);
}

void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry) {
  registry->RegisterBooleanPref(prefs::kDisabledByPolicy, false);
  registry->RegisterIntegerPref(
      kDefaultEthereumWallet,
      static_cast<int>(
          hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension));
  registry->RegisterIntegerPref(
      kDefaultSolanaWallet,
      static_cast<int>(
          hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension));
  registry->RegisterStringPref(kDefaultBaseCurrency, "USD");
  registry->RegisterStringPref(kDefaultBaseCryptocurrency, "BTC");
  registry->RegisterBooleanPref(kShowWalletIconOnToolbar, true);
  registry->RegisterDictionaryPref(kHnsWalletTransactions);
  registry->RegisterDictionaryPref(kHnsWalletP3AActiveWalletDict);
  registry->RegisterDictionaryPref(kHnsWalletKeyrings);
  registry->RegisterBooleanPref(kHnsWalletKeyringEncryptionKeysMigrated,
                                false);
  registry->RegisterDictionaryPref(kHnsWalletCustomNetworks);
  registry->RegisterDictionaryPref(kHnsWalletHiddenNetworks,
                                   GetDefaultHiddenNetworks());
  registry->RegisterDictionaryPref(kHnsWalletSelectedNetworks,
                                   GetDefaultSelectedNetworks());
  registry->RegisterDictionaryPref(kHnsWalletSelectedNetworksPerOrigin,
                                   GetDefaultSelectedNetworksPerOrigin());
  registry->RegisterDictionaryPref(kHnsWalletUserAssets,
                                   GetDefaultUserAssets());
  registry->RegisterIntegerPref(kHnsWalletAutoLockMinutes,
                                kDefaultWalletAutoLockMinutes);
  registry->RegisterDictionaryPref(kHnsWalletEthAllowancesCache);
  registry->RegisterBooleanPref(kSupportEip1559OnLocalhostChain, false);
  registry->RegisterDictionaryPref(kHnsWalletLastTransactionSentTimeDict);
  registry->RegisterTimePref(kHnsWalletLastDiscoveredAssetsAt, base::Time());

  registry->RegisterDictionaryPref(kPinnedNFTAssets);
  registry->RegisterBooleanPref(kAutoPinEnabled, false);
  registry->RegisterBooleanPref(kShouldShowWalletSuggestionBadge, true);
  registry->RegisterBooleanPref(kHnsWalletNftDiscoveryEnabled, false);

  registry->RegisterStringPref(kHnsWalletSelectedWalletAccount, "");
  registry->RegisterStringPref(kHnsWalletSelectedEthDappAccount, "");
  registry->RegisterStringPref(kHnsWalletSelectedSolDappAccount, "");
}

void RegisterLocalStatePrefsForMigration(PrefRegistrySimple* registry) {
  // Added 04/2023
  registry->RegisterTimePref(kHnsWalletP3ALastReportTime, base::Time());
  registry->RegisterTimePref(kHnsWalletP3AFirstReportTime, base::Time());
  registry->RegisterListPref(kHnsWalletP3AWeeklyStorage);
}

void RegisterProfilePrefsForMigration(
    user_prefs::PrefRegistrySyncable* registry) {
  // Added 10/2021
  registry->RegisterBooleanPref(kHnsWalletUserAssetEthContractAddressMigrated,
                                false);
  // Added 09/2021
  registry->RegisterIntegerPref(
      kHnsWalletWeb3ProviderDeprecated,
      static_cast<int>(mojom::DefaultWallet::HnsWalletPreferExtension));

  // Added 25/10/2021
  registry->RegisterIntegerPref(
      kDefaultWalletDeprecated,
      static_cast<int>(mojom::DefaultWallet::HnsWalletPreferExtension));

  // Added 02/2022
  registry->RegisterBooleanPref(
      kHnsWalletEthereumTransactionsCoinTypeMigrated, false);

  // Added 22/02/2022
  registry->RegisterListPref(kHnsWalletCustomNetworksDeprecated);
  registry->RegisterStringPref(kHnsWalletCurrentChainId,
                               hns_wallet::mojom::kMainnetChainId);

  // Added 04/2022
  registry->RegisterDictionaryPref(kHnsWalletUserAssetsDeprecated);

  // Added 06/2022
  registry->RegisterBooleanPref(
      kHnsWalletUserAssetsAddPreloadingNetworksMigrated, false);

  // Added 10/2022
  registry->RegisterBooleanPref(
      kHnsWalletDeprecateEthereumTestNetworksMigrated, false);

  // Added 10/2022
  registry->RegisterBooleanPref(kHnsWalletUserAssetsAddIsNFTMigrated, false);

  // Added 11/2022
  p3a_utils::RegisterFeatureUsagePrefs(
      registry, kHnsWalletP3AFirstUnlockTime, kHnsWalletP3ALastUnlockTime,
      kHnsWalletP3AUsedSecondDay, nullptr, nullptr);
  registry->RegisterTimePref(kHnsWalletLastUnlockTime, base::Time());
  registry->RegisterTimePref(kHnsWalletP3ALastReportTime, base::Time());
  registry->RegisterTimePref(kHnsWalletP3AFirstReportTime, base::Time());
  registry->RegisterListPref(kHnsWalletP3AWeeklyStorage);

  // Added 12/2022
  registry->RegisterBooleanPref(kShowWalletTestNetworksDeprecated, false);

  // Added 02/2023
  registry->RegisterBooleanPref(kHnsWalletTransactionsChainIdMigrated, false);

  // Added 03/2023
  registry->RegisterIntegerPref(kHnsWalletDefaultHiddenNetworksVersion, 0);

  // Added 03/2023
  registry->RegisterBooleanPref(kHnsWalletUserAssetsAddIsERC1155Migrated,
                                false);

  // Added 04/2023
  registry->RegisterBooleanPref(kHnsWalletSolanaTransactionsV0SupportMigrated,
                                false);

  // Added 06/2023
  registry->RegisterIntegerPref(
      kHnsWalletSelectedCoinDeprecated,
      static_cast<int>(hns_wallet::mojom::CoinType::ETH));

  // Added 07/2023
  registry->RegisterBooleanPref(kHnsWalletUserAssetsAddIsSpamMigrated, false);

  // Added 07/2023
  registry->RegisterBooleanPref(kHnsWalletTransactionsFromPrefsToDBMigrated,
                                false);

  // Added 08/2023
  registry->RegisterBooleanPref(kHnsWalletCustomNetworksFantomMainnetMigrated,
                                false);
}

void ClearJsonRpcServiceProfilePrefs(PrefService* prefs) {
  DCHECK(prefs);
  prefs->ClearPref(kHnsWalletCustomNetworks);
  prefs->ClearPref(kHnsWalletHiddenNetworks);
  prefs->ClearPref(kHnsWalletSelectedNetworks);
  prefs->ClearPref(kHnsWalletSelectedNetworksPerOrigin);
  prefs->ClearPref(kSupportEip1559OnLocalhostChain);
}

void ClearKeyringServiceProfilePrefs(PrefService* prefs) {
  DCHECK(prefs);
  prefs->ClearPref(kHnsWalletKeyrings);
  prefs->ClearPref(kHnsWalletAutoLockMinutes);
  prefs->ClearPref(kHnsWalletSelectedWalletAccount);
  prefs->ClearPref(kHnsWalletSelectedEthDappAccount);
  prefs->ClearPref(kHnsWalletSelectedSolDappAccount);
}

void ClearTxServiceProfilePrefs(PrefService* prefs) {
  DCHECK(prefs);
  // Remove this when we remove kHnsWalletTransactions.
  prefs->ClearPref(kHnsWalletTransactions);
}

void ClearHnsWalletServicePrefs(PrefService* prefs) {
  DCHECK(prefs);
  prefs->ClearPref(kHnsWalletUserAssets);
  prefs->ClearPref(kDefaultBaseCurrency);
  prefs->ClearPref(kDefaultBaseCryptocurrency);
  prefs->ClearPref(kHnsWalletEthAllowancesCache);
}

void MigrateObsoleteProfilePrefs(PrefService* prefs) {
  // Added 10/2021 for migrating the contract address for eth in user asset
  // list from 'eth' to an empty string.
  HnsWalletService::MigrateUserAssetEthContractAddress(prefs);

  // Added 04/22 to have coin_type as the top level, also rename
  // contract_address key to address.
  HnsWalletService::MigrateMultichainUserAssets(prefs);

  // Added 06/22 to have native tokens for all preloading networks.
  HnsWalletService::MigrateUserAssetsAddPreloadingNetworks(prefs);

  // Added 10/22 to have is_nft set for existing ERC721 tokens.
  HnsWalletService::MigrateUserAssetsAddIsNFT(prefs);

  // Added 03/23 to add filecoin evm support.
  HnsWalletService::MigrateHiddenNetworks(prefs);

  // Added 03/23 to have is_erc1155 set false for existing ERC1155 tokens.
  HnsWalletService::MigrateUserAssetsAddIsERC1155(prefs);

  // Added 07/23 to have is_spam set false for existing tokens.
  HnsWalletService::MigrateUserAssetsAddIsSpam(prefs);

  // Added 08/09 to add Fantom as a custom network if selected for the default
  // or custom origins.
  HnsWalletService::MigrateFantomMainnetAsCustomNetwork(prefs);

  JsonRpcService::MigrateMultichainNetworks(prefs);

  if (prefs->HasPrefPath(kHnsWalletWeb3ProviderDeprecated)) {
    mojom::DefaultWallet provider = static_cast<mojom::DefaultWallet>(
        prefs->GetInteger(kHnsWalletWeb3ProviderDeprecated));
    mojom::DefaultWallet default_wallet =
        mojom::DefaultWallet::HnsWalletPreferExtension;
    if (provider == mojom::DefaultWallet::None) {
      default_wallet = mojom::DefaultWallet::None;
    }
    prefs->SetInteger(kDefaultEthereumWallet, static_cast<int>(default_wallet));
    prefs->ClearPref(kHnsWalletWeb3ProviderDeprecated);
  }
  if (prefs->HasPrefPath(kDefaultWalletDeprecated)) {
    mojom::DefaultWallet provider = static_cast<mojom::DefaultWallet>(
        prefs->GetInteger(kDefaultWalletDeprecated));
    mojom::DefaultWallet default_wallet =
        mojom::DefaultWallet::HnsWalletPreferExtension;
    if (provider == mojom::DefaultWallet::None) {
      default_wallet = mojom::DefaultWallet::None;
    }
    prefs->SetInteger(kDefaultEthereumWallet, static_cast<int>(default_wallet));
    prefs->ClearPref(kDefaultWalletDeprecated);
  }

  // Added 02/2022.
  // Migrate kHnsWalletTransactions to have coin_type as the top level.
  // Ethereum transactions were at kHnsWalletTransactions.network_id.tx_id,
  // migrate it to be at kHnsWalletTransactions.ethereum.network_id.tx_id.
  if (!prefs->GetBoolean(kHnsWalletEthereumTransactionsCoinTypeMigrated)) {
    auto transactions = prefs->GetDict(kHnsWalletTransactions).Clone();
    prefs->ClearPref(kHnsWalletTransactions);
    if (!transactions.empty()) {
      ScopedDictPrefUpdate update(prefs, kHnsWalletTransactions);
      update->Set(kEthereumPrefKey, std::move(transactions));
    }
    prefs->SetBoolean(kHnsWalletEthereumTransactionsCoinTypeMigrated, true);
  }
  // Added 10/2022
  JsonRpcService::MigrateDeprecatedEthereumTestnets(prefs);

  // Added 12/2022
  JsonRpcService::MigrateShowTestNetworksToggle(prefs);

  // Added 02/2023
  TxStateManager::MigrateAddChainIdToTransactionInfo(prefs);

  // Added 07/2023
  KeyringService::MigrateDerivedAccountIndex(prefs);
}

}  // namespace hns_wallet
