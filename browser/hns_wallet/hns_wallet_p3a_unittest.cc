/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/hns_wallet_p3a.h"
#include "base/memory/raw_ptr.h"
#include "base/test/metrics/histogram_tester.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/features.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile_manager.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns_wallet {

class HnsWalletP3AUnitTest : public testing::Test {
 public:
  HnsWalletP3AUnitTest()
      : task_environment_(base::test::TaskEnvironment::TimeSource::MOCK_TIME) {
    histogram_tester_ = std::make_unique<base::HistogramTester>();
  }

  void SetUp() override {
    TestingProfile::Builder builder;
    local_state_ = std::make_unique<ScopedTestingLocalState>(
        TestingBrowserProcess::GetGlobal());
    profile_ = builder.Build();
    keyring_service_ =
        KeyringServiceFactory::GetServiceForContext(profile_.get());
    wallet_service_ =
        hns_wallet::HnsWalletServiceFactory::GetServiceForContext(
            profile_.get());
    wallet_p3a_ = wallet_service_->GetHnsWalletP3A();
  }
  void WaitForResponse() { task_environment_.RunUntilIdle(); }

  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<ScopedTestingLocalState> local_state_;
  std::unique_ptr<TestingProfile> profile_;
  std::unique_ptr<base::HistogramTester> histogram_tester_;
  raw_ptr<KeyringService> keyring_service_;
  raw_ptr<HnsWalletService> wallet_service_;
  raw_ptr<HnsWalletP3A> wallet_p3a_;
};

TEST_F(HnsWalletP3AUnitTest, KeyringCreated) {
  base::test::ScopedFeatureList feature_list;
  base::FieldTrialParams parameters;
  parameters[features::kCreateDefaultSolanaAccount.name] = "false";

  std::vector<base::test::FeatureRefAndParams> enabled_features;
  enabled_features.emplace_back(
      hns_wallet::features::kHnsWalletSolanaFeature, parameters);

  feature_list.InitWithFeaturesAndParameters(enabled_features, {});

  histogram_tester_->ExpectBucketCount(kKeyringCreatedHistogramName, 0, 1);
  keyring_service_->CreateWallet("testing123", base::DoNothing());
  WaitForResponse();
  histogram_tester_->ExpectBucketCount(kKeyringCreatedHistogramName, 1, 1);
}

TEST_F(HnsWalletP3AUnitTest, ReportOnboardingAction) {
  histogram_tester_->ExpectTotalCount(kOnboardingConversionHistogramName, 0);

  wallet_p3a_->ReportOnboardingAction(mojom::OnboardingAction::Shown);
  // should not record immediately, should delay
  histogram_tester_->ExpectTotalCount(kOnboardingConversionHistogramName, 0);
  task_environment_.FastForwardBy(base::Seconds(110));

  // report new action before 120 seconds deadline, should postpone timer
  wallet_p3a_->ReportOnboardingAction(
      mojom::OnboardingAction::LegalAndPassword);
  histogram_tester_->ExpectTotalCount(kOnboardingConversionHistogramName, 0);
  task_environment_.FastForwardBy(base::Seconds(120));
  histogram_tester_->ExpectUniqueSample(kOnboardingConversionHistogramName, 1,
                                        1);

  // report new action after 120 seconds deadline, should record
  // immediately to correct histogram value
  wallet_p3a_->ReportOnboardingAction(mojom::OnboardingAction::RecoverySetup);
  histogram_tester_->ExpectTotalCount(kOnboardingConversionHistogramName, 2);
  histogram_tester_->ExpectBucketCount(kOnboardingConversionHistogramName, 2,
                                       1);
}

TEST_F(HnsWalletP3AUnitTest, ReportOnboardingActionRestore) {
  histogram_tester_->ExpectTotalCount(kOnboardingConversionHistogramName, 0);

  wallet_p3a_->ReportOnboardingAction(mojom::OnboardingAction::Shown);
  histogram_tester_->ExpectTotalCount(kOnboardingConversionHistogramName, 0);
  task_environment_.FastForwardBy(base::Seconds(50));

  wallet_p3a_->ReportOnboardingAction(mojom::OnboardingAction::StartRestore);
  histogram_tester_->ExpectTotalCount(kOnboardingConversionHistogramName, 0);
  task_environment_.FastForwardBy(base::Seconds(120));
  // should not monitor the wallet restore flow
  histogram_tester_->ExpectTotalCount(kOnboardingConversionHistogramName, 0);
}

TEST_F(HnsWalletP3AUnitTest, TransactionSent) {
  histogram_tester_->ExpectTotalCount(kEthTransactionSentHistogramName, 0);
  histogram_tester_->ExpectTotalCount(kSolTransactionSentHistogramName, 0);
  histogram_tester_->ExpectTotalCount(kFilTransactionSentHistogramName, 0);

  HnsWalletP3A* wallet_p3a = wallet_service_->GetHnsWalletP3A();

  wallet_p3a->ReportTransactionSent(mojom::CoinType::ETH, true);
  histogram_tester_->ExpectUniqueSample(kEthTransactionSentHistogramName, 1, 1);

  wallet_p3a->ReportTransactionSent(mojom::CoinType::SOL, true);
  histogram_tester_->ExpectUniqueSample(kSolTransactionSentHistogramName, 1, 1);

  wallet_p3a->ReportTransactionSent(mojom::CoinType::FIL, true);
  histogram_tester_->ExpectUniqueSample(kFilTransactionSentHistogramName, 1, 1);

  task_environment_.FastForwardBy(base::Days(4));
  histogram_tester_->ExpectUniqueSample(kEthTransactionSentHistogramName, 1, 5);
  histogram_tester_->ExpectUniqueSample(kSolTransactionSentHistogramName, 1, 5);
  histogram_tester_->ExpectUniqueSample(kFilTransactionSentHistogramName, 1, 5);

  task_environment_.FastForwardBy(base::Days(3));
  histogram_tester_->ExpectBucketCount(kEthTransactionSentHistogramName, 0, 1);
  histogram_tester_->ExpectBucketCount(kSolTransactionSentHistogramName, 0, 1);
  histogram_tester_->ExpectBucketCount(kFilTransactionSentHistogramName, 0, 1);
}

TEST_F(HnsWalletP3AUnitTest, ActiveAccounts) {
  wallet_p3a_->RecordActiveWalletCount(0, mojom::CoinType::ETH);
  wallet_p3a_->RecordActiveWalletCount(0, mojom::CoinType::FIL);
  wallet_p3a_->RecordActiveWalletCount(0, mojom::CoinType::SOL);

  // Should not record zero to histogram if user never had an active account
  histogram_tester_->ExpectTotalCount(kEthActiveAccountHistogramName, 0);
  histogram_tester_->ExpectTotalCount(kFilActiveAccountHistogramName, 0);
  histogram_tester_->ExpectTotalCount(kSolActiveAccountHistogramName, 0);

  wallet_p3a_->RecordActiveWalletCount(3, mojom::CoinType::ETH);
  wallet_p3a_->RecordActiveWalletCount(9, mojom::CoinType::FIL);
  wallet_p3a_->RecordActiveWalletCount(7, mojom::CoinType::SOL);

  histogram_tester_->ExpectBucketCount(kEthActiveAccountHistogramName, 3, 1);
  histogram_tester_->ExpectBucketCount(kFilActiveAccountHistogramName, 5, 1);
  histogram_tester_->ExpectBucketCount(kSolActiveAccountHistogramName, 4, 1);

  wallet_p3a_->RecordActiveWalletCount(0, mojom::CoinType::ETH);
  wallet_p3a_->RecordActiveWalletCount(1, mojom::CoinType::FIL);
  wallet_p3a_->RecordActiveWalletCount(2, mojom::CoinType::SOL);

  histogram_tester_->ExpectBucketCount(kEthActiveAccountHistogramName, 0, 1);
  histogram_tester_->ExpectBucketCount(kFilActiveAccountHistogramName, 1, 1);
  histogram_tester_->ExpectBucketCount(kSolActiveAccountHistogramName, 2, 1);
}

TEST_F(HnsWalletP3AUnitTest, NewUserBalance) {
  // record first usage
  wallet_p3a_->ReportUsage(true);

  task_environment_.FastForwardBy(base::Days(3));
  wallet_p3a_->ReportUsage(true);

  histogram_tester_->ExpectTotalCount(kNewUserBalanceHistogramName, 0);
  wallet_p3a_->RecordActiveWalletCount(1, mojom::CoinType::ETH);
  histogram_tester_->ExpectUniqueSample(kNewUserBalanceHistogramName, 1, 1);

  task_environment_.FastForwardBy(base::Days(2));
  wallet_p3a_->RecordActiveWalletCount(1, mojom::CoinType::ETH);
  // Should not record because we already recorded
  histogram_tester_->ExpectUniqueSample(kNewUserBalanceHistogramName, 1, 1);
}

TEST_F(HnsWalletP3AUnitTest, NewUserBalancePastDeadline) {
  // record first usage
  wallet_p3a_->ReportUsage(true);

  task_environment_.FastForwardBy(base::Days(8));
  wallet_p3a_->ReportUsage(true);

  histogram_tester_->ExpectTotalCount(kNewUserBalanceHistogramName, 0);
  wallet_p3a_->RecordActiveWalletCount(1, mojom::CoinType::ETH);

  // Should not record new value since we are past the deadline
  histogram_tester_->ExpectTotalCount(kNewUserBalanceHistogramName, 0);
}

TEST_F(HnsWalletP3AUnitTest, JSProviders) {
  auto test_func = [&](mojom::CoinType coin_type, const char* histogram_name) {
    histogram_tester_->ExpectTotalCount(histogram_name, 0);

    wallet_p3a_->ReportJSProvider(mojom::JSProviderType::None, coin_type,
                                  /*allow_provider_override*/ true);
    histogram_tester_->ExpectUniqueSample(histogram_name, 0, 1);

    keyring_service_->CreateWallet("testing123", base::DoNothing());
    WaitForResponse();

    wallet_p3a_->ReportJSProvider(mojom::JSProviderType::None, coin_type,
                                  /*allow_provider_override*/ true);
    histogram_tester_->ExpectBucketCount(histogram_name, 1, 1);

    wallet_p3a_->ReportJSProvider(mojom::JSProviderType::Native, coin_type,
                                  /*allow_provider_override*/ true);
    histogram_tester_->ExpectBucketCount(histogram_name, 2, 1);

    wallet_p3a_->ReportJSProvider(mojom::JSProviderType::Native, coin_type,
                                  /*allow_provider_override*/ false);
    histogram_tester_->ExpectBucketCount(histogram_name, 3, 1);

    wallet_p3a_->ReportJSProvider(mojom::JSProviderType::ThirdParty, coin_type,
                                  /*allow_provider_override*/ true);
    histogram_tester_->ExpectBucketCount(histogram_name, 5, 1);

    keyring_service_->Reset();

    wallet_p3a_->ReportJSProvider(mojom::JSProviderType::ThirdParty, coin_type,
                                  /*allow_provider_override*/ true);
    histogram_tester_->ExpectBucketCount(histogram_name, 4, 1);

    keyring_service_->Reset();
    wallet_p3a_->ReportJSProvider(mojom::JSProviderType::Native, coin_type,
                                  /*allow_provider_override*/ true);
    histogram_tester_->ExpectBucketCount(histogram_name, 0, 2);
  };
  test_func(mojom::CoinType::ETH, kEthProviderHistogramName);
  test_func(mojom::CoinType::SOL, kSolProviderHistogramName);
}

TEST_F(HnsWalletP3AUnitTest, NFTGalleryViews) {
  histogram_tester_->ExpectTotalCount(kHnsWalletNFTCountHistogramName, 0);
  histogram_tester_->ExpectTotalCount(kHnsWalletNFTNewUserHistogramName, 0);

  wallet_p3a_->RecordNFTGalleryView(0);
  histogram_tester_->ExpectUniqueSample(kHnsWalletNFTCountHistogramName, 0,
                                        1);
  histogram_tester_->ExpectUniqueSample(kHnsWalletNFTNewUserHistogramName, 1,
                                        1);

  wallet_p3a_->RecordNFTGalleryView(6);
  histogram_tester_->ExpectBucketCount(kHnsWalletNFTCountHistogramName, 2, 1);
  // new user histogram should only be reported once, ever
  histogram_tester_->ExpectUniqueSample(kHnsWalletNFTNewUserHistogramName, 1,
                                        1);
}

TEST_F(HnsWalletP3AUnitTest, NFTDiscoveryEnabled) {
  histogram_tester_->ExpectTotalCount(
      kHnsWalletNFTDiscoveryEnabledHistogramName, 0);

  local_state_->Get()->SetTime(kHnsWalletLastUnlockTime, base::Time::Now());
  histogram_tester_->ExpectUniqueSample(
      kHnsWalletNFTDiscoveryEnabledHistogramName, 0, 1);

  profile_->GetPrefs()->SetBoolean(kHnsWalletNftDiscoveryEnabled, true);
  histogram_tester_->ExpectBucketCount(
      kHnsWalletNFTDiscoveryEnabledHistogramName, 1, 1);

  profile_->GetPrefs()->SetBoolean(kHnsWalletNftDiscoveryEnabled, false);
  histogram_tester_->ExpectBucketCount(
      kHnsWalletNFTDiscoveryEnabledHistogramName, 0, 2);
}

}  // namespace hns_wallet
