/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_WALLET_BROWSER_PREF_NAMES_H_
#define HNS_COMPONENTS_HNS_WALLET_BROWSER_PREF_NAMES_H_

extern const char kShouldShowWalletSuggestionBadge[];
extern const char kDefaultEthereumWallet[];
extern const char kDefaultSolanaWallet[];
extern const char kDefaultBaseCurrency[];
extern const char kDefaultBaseCryptocurrency[];
extern const char kHnsWalletTransactions[];
extern const char kShowWalletIconOnToolbar[];
extern const char kHnsWalletLastUnlockTime[];
extern const char kHnsWalletPingReportedUnlockTime[];
extern const char kHnsWalletP3ALastReportTime[];
extern const char kHnsWalletP3AFirstReportTime[];
extern const char kHnsWalletP3AWeeklyStorage[];
extern const char kHnsWalletP3ANFTGalleryUsed[];
extern const char kHnsWalletSelectedWalletAccount[];
extern const char kHnsWalletSelectedEthDappAccount[];
extern const char kHnsWalletSelectedSolDappAccount[];
extern const char kHnsWalletKeyrings[];
extern const char kHnsWalletCustomNetworks[];
extern const char kHnsWalletHiddenNetworks[];
extern const char kHnsWalletSelectedNetworks[];
extern const char kHnsWalletSelectedNetworksPerOrigin[];
extern const char kHnsWalletUserAssets[];
extern const char kHnsWalletEthAllowancesCache[];
// Added 10/2021 to migrate contract address to an empty string for ETH.
extern const char kHnsWalletUserAssetEthContractAddressMigrated[];
// Added 06/2022 to add native assets of preloading networks to user assets.
extern const char kHnsWalletUserAssetsAddPreloadingNetworksMigrated[];
// Added 10/2022 to set is_nft = true for existing ERC721 tokens.
extern const char kHnsWalletUserAssetsAddIsNFTMigrated[];
// Added 03/2023 to add networks hidden by default
extern const char kHnsWalletDefaultHiddenNetworksVersion[];
// Added 03/2023 to set is_erc1155 = false for all existing tokens.
extern const char kHnsWalletUserAssetsAddIsERC1155Migrated[];
// Added 10/2022 to replace ETH selected network with mainnet if selected
// network is one of the Ethereum testnets deprecated on 10/5/2022.
extern const char kHnsWalletDeprecateEthereumTestNetworksMigrated[];
// Added 06/2023 to set is_spam = false for all existing tokens.
extern const char kHnsWalletUserAssetsAddIsSpamMigrated[];
extern const char kHnsWalletAutoLockMinutes[];
extern const char kSupportEip1559OnLocalhostChain[];
// Added 02/2022 to migrate ethereum transactions to be under ethereum coin
// type.
extern const char kHnsWalletEthereumTransactionsCoinTypeMigrated[];

extern const char kHnsWalletP3AFirstUnlockTime[];
extern const char kHnsWalletP3ALastUnlockTime[];
extern const char kHnsWalletP3AUsedSecondDay[];
extern const char kHnsWalletP3AOnboardingLastStep[];
extern const char kHnsWalletP3ANewUserBalanceReported[];

extern const char kHnsWalletP3AActiveWalletDict[];
extern const char kHnsWalletKeyringEncryptionKeysMigrated[];
extern const char kHnsWalletNftDiscoveryEnabled[];
extern const char kHnsWalletLastDiscoveredAssetsAt[];

extern const char kHnsWalletLastTransactionSentTimeDict[];
// Added 02/2023 to migrate transactions to contain the
// chain_id for each one.
extern const char kHnsWalletTransactionsChainIdMigrated[];
// Added 04/2023 to migrate solana transactions for v0 transaction support.
extern const char kHnsWalletSolanaTransactionsV0SupportMigrated[];
// Added 07/2023 to migrate transactions from prefs to DB.
extern const char kHnsWalletTransactionsFromPrefsToDBMigrated[];

// Added 08/09 to migrate Fantom mainnet, previously a preloaded network,
// to a custom network.
extern const char kHnsWalletCustomNetworksFantomMainnetMigrated[];

// DEPRECATED
extern const char kShowWalletTestNetworksDeprecated[];
extern const char kHnsWalletWeb3ProviderDeprecated[];
extern const char kDefaultWalletDeprecated[];
extern const char kHnsWalletCustomNetworksDeprecated[];
extern const char kHnsWalletCurrentChainId[];
extern const char kHnsWalletUserAssetsDeprecated[];
extern const char
    kHnsWalletUserAssetsAddPreloadingNetworksMigratedDeprecated[];
extern const char
    kHnsWalletUserAssetsAddPreloadingNetworksMigratedDeprecated2[];
extern const char kPinnedNFTAssets[];
extern const char kAutoPinEnabled[];
// 06/2023
extern const char kHnsWalletSelectedCoinDeprecated[];

#endif  // HNS_COMPONENTS_HNS_WALLET_BROWSER_PREF_NAMES_H_
