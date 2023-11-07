/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/pref_names.h"

const char kShouldShowWalletSuggestionBadge[] =
    "hns.wallet.should_show_wallet_suggestion_badge";
const char kDefaultEthereumWallet[] = "hns.wallet.default_wallet2";
const char kDefaultSolanaWallet[] = "hns.wallet.default_solana_wallet";
const char kDefaultBaseCurrency[] = "hns.wallet.default_base_currency";
const char kDefaultBaseCryptocurrency[] =
    "hns.wallet.default_base_cryptocurrency";
const char kHnsWalletTransactions[] = "hns.wallet.transactions";
const char kShowWalletIconOnToolbar[] =
    "hns.wallet.show_wallet_icon_on_toolbar";
const char kHnsWalletLastUnlockTime[] =
    "hns.wallet.wallet_last_unlock_time_v2";
const char kHnsWalletPingReportedUnlockTime[] =
    "hns.wallet.wallet_report_unlock_time_ping";
const char kHnsWalletP3ALastReportTime[] =
    "hns.wallet.wallet_p3a_last_report_time";
const char kHnsWalletP3AFirstReportTime[] =
    "hns.wallet.wallet_p3a_first_report_time";
extern const char kHnsWalletP3ANFTGalleryUsed[] =
    "hns.wallet.wallet_p3a_nft_gallery_used";
const char kHnsWalletP3ANewUserBalanceReported[] =
    "hns.wallet.p3a_new_user_balance_reported";
const char kHnsWalletP3AWeeklyStorage[] =
    "hns.wallet.wallet_p3a_weekly_storage";
const char kHnsWalletP3AActiveWalletDict[] =
    "hns.wallet.wallet_p3a_active_wallets";
const char kHnsWalletCustomNetworks[] = "hns.wallet.custom_networks";
const char kHnsWalletHiddenNetworks[] = "hns.wallet.hidden_networks";
const char kHnsWalletSelectedNetworks[] = "hns.wallet.selected_networks";
const char kHnsWalletSelectedNetworksPerOrigin[] =
    "hns.wallet.selected_networks_origin";
const char kHnsWalletSelectedWalletAccount[] =
    "hns.wallet.selected_wallet_account";
const char kHnsWalletSelectedEthDappAccount[] =
    "hns.wallet.selected_eth_dapp_account";
const char kHnsWalletSelectedSolDappAccount[] =
    "hns.wallet.selected_sol_dapp_account";
const char kHnsWalletKeyrings[] = "hns.wallet.keyrings";
const char kHnsWalletUserAssets[] = "hns.wallet.wallet_user_assets";
const char kHnsWalletEthAllowancesCache[] =
    "hns.wallet.eth_allowances_cache";
const char kHnsWalletUserAssetEthContractAddressMigrated[] =
    "hns.wallet.user.asset.eth_contract_address_migrated";
const char kHnsWalletUserAssetsAddPreloadingNetworksMigrated[] =
    "hns.wallet.user.assets.add_preloading_networks_migrated_3";
const char kHnsWalletUserAssetsAddIsNFTMigrated[] =
    "hns.wallet.user.assets.add_is_nft_migrated";
const char kHnsWalletDefaultHiddenNetworksVersion[] =
    "hns.wallet.user.assets.default_hidden_networks_version";
const char kHnsWalletUserAssetsAddIsERC1155Migrated[] =
    "hns.wallet.user.assets.add_is_erc1155_migrated";
const char kHnsWalletDeprecateEthereumTestNetworksMigrated[] =
    "hns.wallet.deprecated_ethereum_test_networks_migrated";
const char kHnsWalletUserAssetsAddIsSpamMigrated[] =
    "hns.wallet.user.assets.add_is_spam_migrated";
const char kHnsWalletAutoLockMinutes[] = "hns.wallet.auto_lock_minutes";
const char kSupportEip1559OnLocalhostChain[] =
    "hns.wallet.support_eip1559_on_localhost_chain";
const char kHnsWalletEthereumTransactionsCoinTypeMigrated[] =
    "hns.wallet.ethereum_transactions.coin_type_migrated";
const char kHnsWalletP3AFirstUnlockTime[] =
    "hns.wallet.p3a_first_unlock_time";
const char kHnsWalletP3ALastUnlockTime[] =
    "hns.wallet.p3a_last_unlock_time";
const char kHnsWalletP3AUsedSecondDay[] = "hns.wallet.p3a_used_second_day";
const char kHnsWalletP3AOnboardingLastStep[] =
    "hns.wallet.p3a_last_onboarding_step";
const char kHnsWalletKeyringEncryptionKeysMigrated[] =
    "hns.wallet.keyring_encryption_keys_migrated";
const char kHnsWalletLastTransactionSentTimeDict[] =
    "hns.wallet.last_transaction_sent_time_dict";
const char kHnsWalletNftDiscoveryEnabled[] =
    "hns.wallet.nft_discovery_enabled";
const char kHnsWalletLastDiscoveredAssetsAt[] =
    "hns.wallet.last_discovered_assets_at";
const char kHnsWalletTransactionsChainIdMigrated[] =
    "hns.wallet.transactions.chain_id_migrated";
const char kHnsWalletSolanaTransactionsV0SupportMigrated[] =
    "hns.wallet.solana_transactions.v0_support_migrated";
const char kHnsWalletTransactionsFromPrefsToDBMigrated[] =
    "hns.wallet.transactions.from_prefs_to_db_migrated";
const char kHnsWalletCustomNetworksFantomMainnetMigrated[] =
    "hns.wallet.custom_networks.fantom_mainnet_migrated";

// DEPRECATED
const char kShowWalletTestNetworksDeprecated[] =
    "hns.wallet.show_wallet_test_networks";
const char kHnsWalletWeb3ProviderDeprecated[] = "hns.wallet.web3_provider";
const char kDefaultWalletDeprecated[] = "hns.wallet.default_wallet";
const char kHnsWalletCustomNetworksDeprecated[] =
    "hns.wallet.wallet_custom_networks";
const char kHnsWalletCurrentChainId[] =
    "hns.wallet.wallet_current_chain_id";
const char kHnsWalletUserAssetsDeprecated[] = "hns.wallet.user_assets";
const char kHnsWalletUserAssetsAddPreloadingNetworksMigratedDeprecated[] =
    "hns.wallet.user.assets.add_preloading_networks_migrated";
const char kHnsWalletUserAssetsAddPreloadingNetworksMigratedDeprecated2[] =
    "hns.wallet.user.assets.add_preloading_networks_migrated_2";
const char kPinnedNFTAssets[] = "hns.wallet.user_pin_data";
const char kAutoPinEnabled[] = "hns.wallet.auto_pin_enabled";
const char kHnsWalletSelectedCoinDeprecated[] = "hns.wallet.selected_coin";
