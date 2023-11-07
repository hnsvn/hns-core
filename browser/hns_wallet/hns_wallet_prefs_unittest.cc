/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/hns_wallet_prefs.h"

#include <memory>
#include <utility>

#include "base/strings/string_util.h"
#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/features.h"
#include "hns/components/hns_wallet/common/test_utils.h"
#include "chrome/browser/prefs/browser_prefs.h"
#include "chrome/test/base/testing_profile.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/scoped_user_pref_update.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

using testing::ElementsAreArray;
using testing::SizeIs;

namespace hns_wallet {

class HnsWalletPrefsUnitTest : public testing::Test {
 public:
  HnsWalletPrefsUnitTest()
      : task_environment_(base::test::TaskEnvironment::TimeSource::MOCK_TIME) {}
  ~HnsWalletPrefsUnitTest() override = default;

 protected:
  void SetUp() override {
    TestingProfile::Builder builder;
    auto prefs =
        std::make_unique<sync_preferences::TestingPrefServiceSyncable>();
    // RegisterProfilePrefsForMigration(prefs->registry());
    RegisterUserProfilePrefs(prefs->registry());
    builder.SetPrefService(std::move(prefs));
    profile_ = builder.Build();
  }

  PrefService* GetPrefs() { return profile_->GetPrefs(); }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;
};

TEST_F(HnsWalletPrefsUnitTest, MigrateObsoleteProfilePrefsWeb3Provider) {
  // AskDeprecated changes to HnsWalletPreferExtension
  GetPrefs()->SetInteger(kHnsWalletWeb3ProviderDeprecated,
                         static_cast<int>(mojom::DefaultWallet::AskDeprecated));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // HnsWallet changes to HnsWalletPreferExtension
  GetPrefs()->SetInteger(kHnsWalletWeb3ProviderDeprecated,
                         static_cast<int>(mojom::DefaultWallet::HnsWallet));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // HnsWalletPreferExtension remains HnsWalletPreferExtension
  GetPrefs()->SetInteger(
      kHnsWalletWeb3ProviderDeprecated,
      static_cast<int>(mojom::DefaultWallet::HnsWalletPreferExtension));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // Ask changes to HnsWalletPreferExtension
  GetPrefs()->SetInteger(kHnsWalletWeb3ProviderDeprecated,
                         static_cast<int>(mojom::DefaultWallet::AskDeprecated));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // CryptoWallets changes to HnsWalletPreferExtension
  GetPrefs()->SetInteger(kHnsWalletWeb3ProviderDeprecated,
                         static_cast<int>(mojom::DefaultWallet::CryptoWallets));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // None remains None
  GetPrefs()->SetInteger(kHnsWalletWeb3ProviderDeprecated,
                         static_cast<int>(mojom::DefaultWallet::None));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::None,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));
}

TEST_F(HnsWalletPrefsUnitTest,
       MigrateObsoleteProfilePrefsDefaultWalletDeprecated) {
  // AskDeprecated changes to HnsWalletPreferExtension
  GetPrefs()->SetInteger(kDefaultWalletDeprecated,
                         static_cast<int>(mojom::DefaultWallet::AskDeprecated));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // HnsWallet changes to HnsWalletPreferExtension
  GetPrefs()->SetInteger(kDefaultWalletDeprecated,
                         static_cast<int>(mojom::DefaultWallet::HnsWallet));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // HnsWalletPreferExtension remains HnsWalletPreferExtension
  GetPrefs()->SetInteger(
      kDefaultWalletDeprecated,
      static_cast<int>(mojom::DefaultWallet::HnsWalletPreferExtension));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // Ask changes to HnsWalletPreferExtension
  GetPrefs()->SetInteger(kDefaultWalletDeprecated,
                         static_cast<int>(mojom::DefaultWallet::AskDeprecated));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // CryptoWallets changes to HnsWalletPreferExtension
  GetPrefs()->SetInteger(kDefaultWalletDeprecated,
                         static_cast<int>(mojom::DefaultWallet::CryptoWallets));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::HnsWalletPreferExtension,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));

  // None remains None
  GetPrefs()->SetInteger(kDefaultWalletDeprecated,
                         static_cast<int>(mojom::DefaultWallet::None));
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_EQ(mojom::DefaultWallet::None,
            static_cast<mojom::DefaultWallet>(
                GetPrefs()->GetInteger(kDefaultEthereumWallet)));
}

TEST_F(HnsWalletPrefsUnitTest,
       MigrateObsoleteProfilePrefsHnsWalletEthereumTransactionsCoinType) {
  GetPrefs()->SetBoolean(kHnsWalletTransactionsChainIdMigrated, true);
  // Migration when kHnsWalletTransactions is default value (empty dict).
  ASSERT_FALSE(
      GetPrefs()->GetBoolean(kHnsWalletEthereumTransactionsCoinTypeMigrated));
  auto* pref = GetPrefs()->FindPreference(kHnsWalletTransactions);
  ASSERT_TRUE(pref && pref->IsDefaultValue());
  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_TRUE(pref && pref->IsDefaultValue());
  EXPECT_TRUE(
      GetPrefs()->GetBoolean(kHnsWalletEthereumTransactionsCoinTypeMigrated));

  // Migration with existing transactions.
  GetPrefs()->ClearPref(kHnsWalletEthereumTransactionsCoinTypeMigrated);
  GetPrefs()->ClearPref(kHnsWalletTransactions);
  GetPrefs()->SetBoolean(kHnsWalletTransactionsChainIdMigrated, true);
  ASSERT_FALSE(
      GetPrefs()->GetBoolean(kHnsWalletEthereumTransactionsCoinTypeMigrated));
  base::Value::Dict tx1;
  tx1.Set("id", "0x1");
  tx1.Set("status", 1);

  base::Value::Dict tx2;
  tx2.Set("id", "0x2");
  tx2.Set("status", 2);

  {
    ScopedDictPrefUpdate update(GetPrefs(), kHnsWalletTransactions);
    update->SetByDottedPath("mainnet.meta1", tx1.Clone());
    update->SetByDottedPath("mainnet.meta2", tx2.Clone());
    update->SetByDottedPath("ropsten.meta3", tx1.Clone());
  }

  MigrateObsoleteProfilePrefs(GetPrefs());
  const auto& dict = GetPrefs()->GetDict(kHnsWalletTransactions);
  const auto* tx1_value = dict.FindDictByDottedPath("ethereum.mainnet.meta1");
  const auto* tx2_value = dict.FindDictByDottedPath("ethereum.mainnet.meta2");
  const auto* tx3_value = dict.FindDictByDottedPath("ethereum.ropsten.meta3");
  ASSERT_TRUE(tx1_value && tx2_value && tx3_value);
  EXPECT_EQ(*tx1_value, tx1);
  EXPECT_EQ(*tx2_value, tx2);
  EXPECT_EQ(*tx3_value, tx1);
  EXPECT_EQ(dict.size(), 1u);
  EXPECT_EQ(dict.FindDict("ethereum")->size(), 2u);
  EXPECT_EQ(dict.FindDictByDottedPath("ethereum.mainnet")->size(), 2u);
  EXPECT_EQ(dict.FindDictByDottedPath("ethereum.ropsten")->size(), 1u);
  EXPECT_TRUE(
      GetPrefs()->GetBoolean(kHnsWalletEthereumTransactionsCoinTypeMigrated));

  // Migration when kHnsWalletTransactions is an empty dict.
  GetPrefs()->ClearPref(kHnsWalletEthereumTransactionsCoinTypeMigrated);
  GetPrefs()->ClearPref(kHnsWalletTransactions);
  {
    ScopedDictPrefUpdate update(GetPrefs(), kHnsWalletTransactions);
    update->SetByDottedPath("mainnet.meta1", tx1.Clone());
    update->Remove("mainnet");
  }
  EXPECT_TRUE(pref && !pref->IsDefaultValue());
  EXPECT_TRUE(GetPrefs()->GetDict(kHnsWalletTransactions).empty());

  MigrateObsoleteProfilePrefs(GetPrefs());
  EXPECT_TRUE(pref && pref->IsDefaultValue());
  EXPECT_TRUE(
      GetPrefs()->GetBoolean(kHnsWalletEthereumTransactionsCoinTypeMigrated));
}

TEST_F(HnsWalletPrefsUnitTest, MigrateShowTestNetworksToggle) {
  EXPECT_FALSE(GetPrefs()->HasPrefPath(kShowWalletTestNetworksDeprecated));

  // Hiding non-test network which should not be touched by migration.
  AddHiddenNetwork(GetPrefs(), mojom::CoinType::FIL, "0x123");

  // Test networks are hidden by default.
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::ETH),
              ElementsAreArray({mojom::kGoerliChainId, mojom::kSepoliaChainId,
                                mojom::kLocalhostChainId,
                                mojom::kFilecoinEthereumTestnetChainId}));
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::FIL),
              ElementsAreArray({mojom::kFilecoinTestnet,
                                mojom::kLocalhostChainId, "0x123"}));
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::SOL),
              ElementsAreArray({mojom::kSolanaDevnet, mojom::kSolanaTestnet,
                                mojom::kLocalhostChainId}));

  MigrateObsoleteProfilePrefs(GetPrefs());
  // Still same when nothing to migrate.
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::ETH),
              ElementsAreArray({mojom::kGoerliChainId, mojom::kSepoliaChainId,
                                mojom::kLocalhostChainId,
                                mojom::kFilecoinEthereumTestnetChainId}));
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::FIL),
              ElementsAreArray({mojom::kFilecoinTestnet,
                                mojom::kLocalhostChainId, "0x123"}));
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::SOL),
              ElementsAreArray({mojom::kSolanaDevnet, mojom::kSolanaTestnet,
                                mojom::kLocalhostChainId}));

  GetPrefs()->SetBoolean(kShowWalletTestNetworksDeprecated, false);

  MigrateObsoleteProfilePrefs(GetPrefs());
  // Still same when test networks toggle was explicitly off.
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::ETH),
              ElementsAreArray({mojom::kGoerliChainId, mojom::kSepoliaChainId,
                                mojom::kLocalhostChainId,
                                mojom::kFilecoinEthereumTestnetChainId}));
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::FIL),
              ElementsAreArray({mojom::kFilecoinTestnet,
                                mojom::kLocalhostChainId, "0x123"}));
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::SOL),
              ElementsAreArray({mojom::kSolanaDevnet, mojom::kSolanaTestnet,
                                mojom::kLocalhostChainId}));
  EXPECT_FALSE(GetPrefs()->HasPrefPath(kShowWalletTestNetworksDeprecated));

  GetPrefs()->SetBoolean(kShowWalletTestNetworksDeprecated, true);

  MigrateObsoleteProfilePrefs(GetPrefs());
  // Test networks are removed from hidden list after successfull migration.
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::ETH), SizeIs(0));
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::FIL),
              ElementsAreArray({"0x123"}));
  EXPECT_THAT(GetHiddenNetworks(GetPrefs(), mojom::CoinType::SOL), SizeIs(0));
  EXPECT_FALSE(GetPrefs()->HasPrefPath(kShowWalletTestNetworksDeprecated));
}

TEST_F(HnsWalletPrefsUnitTest, MigrateAddChainIdToTransactionInfo) {
  GetPrefs()->SetBoolean(kHnsWalletEthereumTransactionsCoinTypeMigrated,
                         true);
  EXPECT_FALSE(
      GetPrefs()->HasPrefPath(kHnsWalletTransactionsChainIdMigrated));
  EXPECT_FALSE(GetPrefs()->GetBoolean(kHnsWalletTransactionsChainIdMigrated));

  base::Value::Dict txs;
  const char ethTxId[] = "b1e8dda1";
  const auto ethPath = base::JoinString({"mainnet", ethTxId}, ".");
  base::Value::Dict tx1;
  tx1.SetByDottedPath(base::JoinString({ethPath, "id"}, "."), ethTxId);

  const char solTxId[] = "887e878f";
  const auto solPath = base::JoinString({"devnet", solTxId}, ".");
  base::Value::Dict tx2;
  tx2.SetByDottedPath(base::JoinString({solPath, "id"}, "."), solTxId);

  const char filTxId[] = "197ea1e5";
  const auto filPath = base::JoinString({"testnet", filTxId}, ".");
  base::Value::Dict tx3;
  tx3.SetByDottedPath(base::JoinString({filPath, "id"}, "."), filTxId);

  {
    ScopedDictPrefUpdate update(GetPrefs(), kHnsWalletTransactions);
    update->SetByDottedPath("ethereum", std::move(tx1));
    update->SetByDottedPath("solana", std::move(tx2));
    update->SetByDottedPath("filecoin", std::move(tx3));
  }
  MigrateObsoleteProfilePrefs(GetPrefs());

  EXPECT_TRUE(GetPrefs()->HasPrefPath(kHnsWalletTransactionsChainIdMigrated));
  EXPECT_TRUE(GetPrefs()->GetBoolean(kHnsWalletTransactionsChainIdMigrated));

  const base::Value::Dict* transactions =
      &GetPrefs()->GetDict(kHnsWalletTransactions);

  const std::string* ethChainId = transactions->FindStringByDottedPath(
      base::JoinString({"ethereum", ethPath, "chain_id"}, "."));

  EXPECT_TRUE(ethChainId != nullptr);
  EXPECT_EQ(*ethChainId, "0x1");

  const std::string* solChainId = transactions->FindStringByDottedPath(
      base::JoinString({"solana", solPath, "chain_id"}, "."));

  EXPECT_FALSE(solChainId == nullptr);
  EXPECT_EQ(*solChainId, "0x67");

  const std::string* filChainId = transactions->FindStringByDottedPath(
      base::JoinString({"filecoin", filPath, "chain_id"}, "."));

  EXPECT_FALSE(filChainId == nullptr);
  EXPECT_EQ(*filChainId, "t");
}

}  // namespace hns_wallet
