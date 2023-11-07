/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/scoped_observation.h"
#include "hns/browser/hns_news/hns_news_controller_factory.h"
#include "hns/browser/hns_rewards/rewards_service_factory.h"
#include "hns/browser/perf/hns_perf_switches.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/browser/hns_news_controller.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_rewards/browser/rewards_service_observer.h"
#include "hns/components/speedreader/common/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"

#if BUILDFLAG(ENABLE_SPEEDREADER)
#include "hns/browser/speedreader/speedreader_service_factory.h"
#include "hns/components/speedreader/speedreader_service.h"
#endif

namespace {
class TestRewardsServiceObserver
    : public hns_rewards::RewardsServiceObserver {
 public:
  void WaitForServiceInitialized(hns_rewards::RewardsService* service) {
    if (service->IsInitialized()) {
      return;
    }

    observer_.Observe(service);
    run_loop_.Run();
  }

 private:
  void OnRewardsInitialized(hns_rewards::RewardsService*) override {
    run_loop_.Quit();
  }

  base::RunLoop run_loop_;
  base::ScopedObservation<hns_rewards::RewardsService,
                          hns_rewards::RewardsServiceObserver>
      observer_{this};
};
}  // namespace

class HnsSpeedFeatureProcessorBrowserTest : public InProcessBrowserTest {
 protected:
  void SetUpCommandLine(base::CommandLine* command_line) override {
    InProcessBrowserTest::SetUpCommandLine(command_line);
    command_line->AppendSwitch(
        perf::switches::kEnableHnsFeaturesForPerfTesting);
  }

#if BUILDFLAG(ENABLE_SPEEDREADER)
  bool SpeedreaderIsEnabled() {
    auto* speedreader_service =
        speedreader::SpeedreaderServiceFactory::GetForBrowserContext(
            browser()->profile());
    return speedreader_service->IsEnabledForAllSites();
  }
#endif  // BUILDFLAG(ENABLE_SPEEDREADER)

  bool HnsNewsAreEnabled() {
    return hns_news::GetIsEnabled(browser()->profile()->GetPrefs());
  }

  bool HasOptedInToNotificationAds() {
    return browser()->profile()->GetPrefs()->GetBoolean(
        hns_ads::prefs::kOptedInToNotificationAds);
  }

  void WaitForRewardsServiceInitialized() {
    auto* rewards_service = hns_rewards::RewardsServiceFactory::GetForProfile(
        browser()->profile());
    TestRewardsServiceObserver observer;
    observer.WaitForServiceInitialized(rewards_service);
  }
};

IN_PROC_BROWSER_TEST_F(HnsSpeedFeatureProcessorBrowserTest, PRE_Default) {
  WaitForRewardsServiceInitialized();
}

IN_PROC_BROWSER_TEST_F(HnsSpeedFeatureProcessorBrowserTest, Default) {
#if BUILDFLAG(ENABLE_SPEEDREADER)
  EXPECT_TRUE(SpeedreaderIsEnabled());
#endif
  EXPECT_TRUE(HasOptedInToNotificationAds());
  EXPECT_TRUE(HnsNewsAreEnabled());
  WaitForRewardsServiceInitialized();
}
