/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <map>

#include "base/files/scoped_temp_dir.h"
#include "base/memory/raw_ptr.h"
#include "base/test/scoped_feature_list.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/components/hns_rewards/browser/rewards_service_impl.h"
#include "hns/components/hns_rewards/browser/rewards_service_observer.h"
#include "hns/components/hns_rewards/browser/test_util.h"
#include "hns/components/hns_rewards/common/features.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/hns_rewards/common/rewards_flags.h"
#include "hns/components/hns_rewards/core/global_constants.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"
#include "hns/components/greaselion/browser/buildflags/buildflags.h"
#include "hns/components/l10n/common/test/scoped_default_locale.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/test/base/testing_browser_process.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=RewardsServiceJPTest.*

namespace hns_rewards {

using ::testing::NiceMock;
using ::testing::Return;

class RewardsServiceJPTest : public testing::Test {
 public:
  RewardsServiceJPTest() = default;
  ~RewardsServiceJPTest() override = default;

 protected:
  void SetUp() override {
    ASSERT_TRUE(temp_dir_.CreateUniqueTempDir());
    scoped_default_locale_ =
        std::make_unique<hns_l10n::test::ScopedDefaultLocale>("ja_JP");
    profile_ = CreateHnsRewardsProfile(temp_dir_.GetPath());
    ASSERT_TRUE(profile_);
#if BUILDFLAG(ENABLE_GREASELION)
    auto* rewards_ = new RewardsServiceImpl(profile(), nullptr);
#else
    auto* rewards_ = new RewardsServiceImpl(profile());
#endif
    RewardsServiceFactory::SetServiceForTesting(std::move(rewards_));
    rewards_service_ = static_cast<RewardsServiceImpl*>(
        RewardsServiceFactory::GetForProfile(profile()));
    ASSERT_TRUE(RewardsServiceFactory::GetInstance());
    ASSERT_TRUE(rewards_service());

    profile()->GetPrefs()->SetString(prefs::kDeclaredGeo, "JP");
  }

  void TearDown() override {
    TestingBrowserProcess::GetGlobal()->SetLocalState(nullptr);
    delete rewards_service_;
    profile_.reset();
  }

  Profile* profile() { return profile_.get(); }
  RewardsServiceImpl* rewards_service() { return rewards_service_; }

#if BUILDFLAG(ENABLE_GEMINI_WALLET)
  void EnableGemini() {
    feature_list_.InitAndEnableFeature(features::kGeminiFeature);
  }

  void DisableGemini() {
    feature_list_.InitAndDisableFeature(features::kGeminiFeature);
  }
#endif

 protected:
  base::test::ScopedFeatureList feature_list_;

 private:
  // Need this as a very first member to run tests in UI thread
  // When this is set, class should not install any other MessageLoops, like
  // base::test::ScopedTaskEnvironment
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<Profile> profile_;
  raw_ptr<RewardsServiceImpl> rewards_service_ = nullptr;
  base::ScopedTempDir temp_dir_;
  std::unique_ptr<hns_l10n::test::ScopedDefaultLocale> scoped_default_locale_;
};

#if BUILDFLAG(ENABLE_GEMINI_WALLET)
TEST_F(RewardsServiceJPTest, GetExternalWallet) {
  DisableGemini();
  EXPECT_EQ(rewards_service()->GetExternalWalletType(),
            internal::constant::kWalletBitflyer);
}

TEST_F(RewardsServiceJPTest, GetExternalWalletMultipleCustodians) {
  EnableGemini();
  EXPECT_EQ(rewards_service()->GetExternalWalletType(),
            internal::constant::kWalletBitflyer);
  profile()->GetPrefs()->SetString(prefs::kExternalWalletType,
                                   internal::constant::kWalletUphold);
  EXPECT_EQ(rewards_service()->GetExternalWalletType(),
            internal::constant::kWalletBitflyer);
  profile()->GetPrefs()->SetString(prefs::kExternalWalletType,
                                   internal::constant::kWalletGemini);
  EXPECT_EQ(rewards_service()->GetExternalWalletType(),
            internal::constant::kWalletBitflyer);
}
#endif

}  // namespace hns_rewards
