/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "hns/browser/search_engines/search_engine_tracker.h"

#include "base/test/metrics/histogram_tester.h"
#include "hns/browser/profiles/profile_util.h"
#include "hns/browser/ui/browser_commands.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/search_engines/hns_prepopulated_engines.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "chrome/browser/search_engines/template_url_service_factory.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/search_test_utils.h"
#include "components/country_codes/country_codes.h"
#include "components/keyed_service/content/browser_context_dependency_manager.h"
#include "components/search_engines/template_url_prepopulate_data.h"
#include "content/public/test/browser_test.h"

class SearchEngineProviderP3ATest : public InProcessBrowserTest {
 public:
  SearchEngineProviderP3ATest() {
    histogram_tester_ = std::make_unique<base::HistogramTester>();

    // Override the default region. Defaults vary and this
    // ties the expected test results to a specific region
    // so everyone sees the same behaviour.
    //
    // May also help with unstable test results in ci.
    create_services_subscription_ =
        BrowserContextDependencyManager::GetInstance()
            ->RegisterCreateServicesCallbackForTesting(
                base::BindRepeating(&OverrideCountryID, "US"));
  }

 private:
  static void OverrideCountryID(const std::string& country_id,
                                content::BrowserContext* context) {
    const int32_t id = country_codes::CountryCharsToCountryID(country_id.at(0),
                                                              country_id.at(1));
    Profile::FromBrowserContext(context)->GetPrefs()->SetInteger(
        country_codes::kCountryIDAtInstall, id);
  }

 protected:
  std::unique_ptr<base::HistogramTester> histogram_tester_;
  base::CallbackListSubscription create_services_subscription_;
};

IN_PROC_BROWSER_TEST_F(SearchEngineProviderP3ATest, DefaultSearchEngineP3A) {
  // Check that the metric is reported on startup.
  histogram_tester_->ExpectUniqueSample(kDefaultSearchEngineMetric,
                                        SearchEngineP3A::kHns, 1);

  auto* service =
      TemplateURLServiceFactory::GetForProfile(browser()->profile());
  search_test_utils::WaitForTemplateURLServiceToLoad(service);

  // Check that changing the default engine triggers emitting of a new value.
  auto ddg_data = TemplateURLPrepopulateData::GetPrepopulatedEngine(
      browser()->profile()->GetPrefs(),
      TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO);
  TemplateURL ddg_url(*ddg_data);

  service->SetUserSelectedDefaultSearchProvider(&ddg_url);
  histogram_tester_->ExpectBucketCount(kDefaultSearchEngineMetric,
                                       SearchEngineP3A::kDuckDuckGo, 1);

  // Check switching back to original engine.
  auto hns_data = TemplateURLPrepopulateData::GetPrepopulatedEngine(
      browser()->profile()->GetPrefs(),
      TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_HNS);
  TemplateURL hns_url(*hns_data);
  service->SetUserSelectedDefaultSearchProvider(&hns_url);
  histogram_tester_->ExpectBucketCount(kDefaultSearchEngineMetric,
                                       SearchEngineP3A::kHns, 2);

  // Check that incognito or TOR profiles do not emit the metric.
  CreateIncognitoBrowser();
#if BUILDFLAG(ENABLE_TOR)
  hns::NewOffTheRecordWindowTor(browser());
#endif

  histogram_tester_->ExpectTotalCount(kDefaultSearchEngineMetric, 3);
}

IN_PROC_BROWSER_TEST_F(SearchEngineProviderP3ATest, SwitchSearchEngineP3A) {
  // Check that the metric is reported on startup.
  // For some reason we can record kNoSwitch twice, even though
  // kDefaultSearchEngineMetric is only updated once at this point.
  auto start_count = histogram_tester_->GetBucketCount(
      kSwitchSearchEngineMetric, SearchEngineSwitchP3A::kNoSwitch);
  EXPECT_GT(start_count, 0);

  // Load service for switching the default search engine.
  auto* service =
      TemplateURLServiceFactory::GetForProfile(browser()->profile());
  search_test_utils::WaitForTemplateURLServiceToLoad(service);

  // Check that changing the default engine triggers emission of a new value.
  auto ddg_data = TemplateURLPrepopulateData::GetPrepopulatedEngine(
      browser()->profile()->GetPrefs(),
      TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_DUCKDUCKGO);
  TemplateURL ddg_url(*ddg_data);

  service->SetUserSelectedDefaultSearchProvider(&ddg_url);
  // This assumes Hns Search is the default!
  histogram_tester_->ExpectBucketCount(kSwitchSearchEngineMetric,
                                       SearchEngineSwitchP3A::kHnsToDDG, 1);

  // Check additional changes.
  auto hns_data = TemplateURLPrepopulateData::GetPrepopulatedEngine(
      browser()->profile()->GetPrefs(),
      TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_HNS);
  TemplateURL hns_url(*hns_data);

  service->SetUserSelectedDefaultSearchProvider(&hns_url);
  histogram_tester_->ExpectBucketCount(kSwitchSearchEngineMetric,
                                       SearchEngineSwitchP3A::kDDGToHns, 1);

  // Check additional changes.
  auto bing_data = TemplateURLPrepopulateData::GetPrepopulatedEngine(
      browser()->profile()->GetPrefs(),
      TemplateURLPrepopulateData::PREPOPULATED_ENGINE_ID_BING);
  TemplateURL bing_url(*bing_data);

  service->SetUserSelectedDefaultSearchProvider(&bing_url);
  histogram_tester_->ExpectBucketCount(kSwitchSearchEngineMetric,
                                       SearchEngineSwitchP3A::kHnsToOther, 1);

  // Check switching back to original engine.
  service->SetUserSelectedDefaultSearchProvider(&hns_url);
  histogram_tester_->ExpectBucketCount(kSwitchSearchEngineMetric,
                                       SearchEngineSwitchP3A::kOtherToHns, 1);

  // Check that incognito or TOR profiles do not emit the metric.
  histogram_tester_->ExpectTotalCount(kSwitchSearchEngineMetric, 8);
  CreateIncognitoBrowser();
#if BUILDFLAG(ENABLE_TOR)
  hns::NewOffTheRecordWindowTor(browser());
#endif

  histogram_tester_->ExpectTotalCount(kSwitchSearchEngineMetric, 8);
}

IN_PROC_BROWSER_TEST_F(SearchEngineProviderP3ATest, WebDiscoveryEnabledP3A) {
  histogram_tester_->ExpectBucketCount(kWebDiscoveryEnabledMetric, 0, 1);

  PrefService* prefs = browser()->profile()->GetPrefs();
  prefs->SetBoolean(kWebDiscoveryEnabled, true);

  histogram_tester_->ExpectBucketCount(kWebDiscoveryEnabledMetric, 1, 1);

  prefs->SetBoolean(kWebDiscoveryEnabled, false);
  histogram_tester_->ExpectBucketCount(kWebDiscoveryEnabledMetric, 0, 2);
}
