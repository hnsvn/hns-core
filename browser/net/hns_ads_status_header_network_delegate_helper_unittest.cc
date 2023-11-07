/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <memory>
#include <string>
#include <utility>

#include "base/test/scoped_feature_list.h"
#include "hns/browser/net/hns_ads_status_header_network_delegate_helper.h"
#include "hns/browser/net/url_context.h"
#include "hns/components/hns_rewards/browser/rewards_service.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/l10n/common/test/scoped_default_locale.h"
#include "chrome/browser/prefs/browser_prefs.h"
#include "chrome/test/base/testing_profile.h"
#include "components/prefs/pref_registry_simple.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/testing_pref_service.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "content/public/test/browser_task_environment.h"
#include "net/base/net_errors.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/blink/public/mojom/loader/resource_load_info.mojom-shared.h"
#include "url/gurl.h"

#if BUILDFLAG(IS_ANDROID)
#include "hns/components/hns_rewards/common/features.h"
#endif  // BUILDFLAG(IS_ANDROID)

using testing::Return;

namespace {

constexpr char kHnsSearchRequestUrl[] =
    "https://search.hns.com/search?q=qwerty";
constexpr char kHnsSearchImageRequestUrl[] =
    "https://search.hns.com/img.png";
constexpr char kNonHnsSearchRequestUrl[] =
    "https://hns.com/search?q=qwerty";
constexpr char kHnsSearchTabUrl[] = "https://search.hns.com";
constexpr char kNonHnsSearchTabUrl[] = "https://hns.com";

}  // namespace

class AdsStatusHeaderDelegateHelperTest : public testing::Test {
 protected:
  void SetUp() override {
    scoped_feature_list_.InitWithFeatures(
        {
#if BUILDFLAG(IS_ANDROID)
          hns_rewards::features::kHnsRewards
#endif  // BUILDFLAG(IS_ANDROID)
        },
        {});

    TestingProfile::Builder builder;
    auto prefs =
        std::make_unique<sync_preferences::TestingPrefServiceSyncable>();
    hns_rewards::RewardsService::RegisterProfilePrefs(prefs->registry());
    RegisterUserProfilePrefs(prefs->registry());
    builder.SetPrefService(std::move(prefs));
    profile_ = builder.Build();
  }

  hns_l10n::test::ScopedDefaultLocale scoped_locale_{"en_US"};
  content::BrowserTaskEnvironment task_environment_;
  base::test::ScopedFeatureList scoped_feature_list_;
  std::unique_ptr<TestingProfile> profile_;
};

TEST_F(AdsStatusHeaderDelegateHelperTest, HnsSearchTabAdsEnabled) {
  // pref_service_.SetBoolean(hns_rewards::prefs::kEnabled, true);
  profile_->GetPrefs()->SetBoolean(hns_rewards::prefs::kEnabled, true);

  auto request_info = std::make_shared<hns::HnsRequestInfo>();
  request_info->browser_context = profile_.get();
  request_info->tab_origin = GURL(kHnsSearchTabUrl);

  {
    request_info->request_url = GURL(kHnsSearchTabUrl);
    request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

    net::HttpRequestHeaders headers;
    const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
        &headers, hns::ResponseCallback(), request_info);
    EXPECT_EQ(rc, net::OK);

    std::string ads_status_header;
    EXPECT_TRUE(headers.GetHeader(hns::kAdsStatusHeader, &ads_status_header));
    EXPECT_EQ(ads_status_header, hns::kAdsEnabledStatusValue);
  }

  {
    request_info->request_url = GURL(kHnsSearchTabUrl);
    request_info->resource_type = blink::mojom::ResourceType::kXhr;

    net::HttpRequestHeaders headers;
    const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
        &headers, hns::ResponseCallback(), request_info);
    EXPECT_EQ(rc, net::OK);

    std::string ads_status_header;
    EXPECT_TRUE(headers.GetHeader(hns::kAdsStatusHeader, &ads_status_header));
    EXPECT_EQ(ads_status_header, hns::kAdsEnabledStatusValue);
  }

  {
    request_info->request_url = GURL(kHnsSearchImageRequestUrl);
    request_info->resource_type = blink::mojom::ResourceType::kImage;

    net::HttpRequestHeaders headers;
    const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
        &headers, hns::ResponseCallback(), request_info);
    EXPECT_EQ(rc, net::OK);

    std::string ads_status_header;
    EXPECT_TRUE(headers.GetHeader(hns::kAdsStatusHeader, &ads_status_header));
    EXPECT_EQ(ads_status_header, hns::kAdsEnabledStatusValue);
  }

  {
    request_info->tab_origin = GURL();
    request_info->initiator_url = GURL(kHnsSearchTabUrl);
    request_info->request_url = GURL(kHnsSearchTabUrl);
    request_info->resource_type = blink::mojom::ResourceType::kXhr;

    net::HttpRequestHeaders headers;
    const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
        &headers, hns::ResponseCallback(), request_info);
    EXPECT_EQ(rc, net::OK);

    std::string ads_status_header;
    EXPECT_TRUE(headers.GetHeader(hns::kAdsStatusHeader, &ads_status_header));
    EXPECT_EQ(ads_status_header, hns::kAdsEnabledStatusValue);
  }
}

TEST_F(AdsStatusHeaderDelegateHelperTest, NonHnsSearchTabAdsEnabled) {
  profile_->GetPrefs()->SetBoolean(hns_rewards::prefs::kEnabled, true);

  auto request_info =
      std::make_shared<hns::HnsRequestInfo>(GURL(kHnsSearchRequestUrl));
  request_info->browser_context = profile_.get();
  request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

  {
    request_info->tab_origin = GURL(kNonHnsSearchTabUrl);
    request_info->initiator_url = GURL();

    net::HttpRequestHeaders headers;
    const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
        &headers, hns::ResponseCallback(), request_info);
    EXPECT_EQ(rc, net::OK);

    EXPECT_FALSE(headers.HasHeader(hns::kAdsStatusHeader));
  }

  {
    request_info->tab_origin = GURL();
    request_info->initiator_url = GURL(kNonHnsSearchTabUrl);

    net::HttpRequestHeaders headers;
    const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
        &headers, hns::ResponseCallback(), request_info);
    EXPECT_EQ(rc, net::OK);

    EXPECT_FALSE(headers.HasHeader(hns::kAdsStatusHeader));
  }
}

TEST_F(AdsStatusHeaderDelegateHelperTest, NonHnsSearchRequestAdsEnabled) {
  profile_->GetPrefs()->SetBoolean(hns_rewards::prefs::kEnabled, true);

  auto request_info = std::make_shared<hns::HnsRequestInfo>(
      GURL(kNonHnsSearchRequestUrl));
  request_info->browser_context = profile_.get();
  request_info->tab_origin = GURL(kHnsSearchTabUrl);
  request_info->initiator_url = GURL(kHnsSearchTabUrl);
  request_info->resource_type = blink::mojom::ResourceType::kXhr;

  net::HttpRequestHeaders headers;
  const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
      &headers, hns::ResponseCallback(), request_info);
  EXPECT_EQ(rc, net::OK);

  EXPECT_FALSE(headers.HasHeader(hns::kAdsStatusHeader));
}

TEST_F(AdsStatusHeaderDelegateHelperTest, HnsSearchHostAdsDisabled) {
  profile_->GetPrefs()->SetBoolean(hns_rewards::prefs::kEnabled, false);

  auto request_info =
      std::make_shared<hns::HnsRequestInfo>(GURL(kHnsSearchRequestUrl));
  request_info->browser_context = profile_.get();
  request_info->tab_origin = GURL(kHnsSearchTabUrl);
  request_info->initiator_url = GURL(kHnsSearchTabUrl);

  {
    request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

    net::HttpRequestHeaders headers;
    const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
        &headers, hns::ResponseCallback(), request_info);
    EXPECT_EQ(rc, net::OK);

    EXPECT_FALSE(headers.HasHeader(hns::kAdsStatusHeader));
  }

  {
    request_info->resource_type = blink::mojom::ResourceType::kXhr;

    net::HttpRequestHeaders headers;
    const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
        &headers, hns::ResponseCallback(), request_info);
    EXPECT_EQ(rc, net::OK);

    EXPECT_FALSE(headers.HasHeader(hns::kAdsStatusHeader));
  }
}

TEST_F(AdsStatusHeaderDelegateHelperTest, HnsSearchHostIncognitoProfile) {
  TestingProfile* incognito_profile =
      TestingProfile::Builder().BuildIncognito(profile_.get());

  auto request_info =
      std::make_shared<hns::HnsRequestInfo>(GURL(kHnsSearchRequestUrl));
  request_info->browser_context = incognito_profile;
  request_info->tab_origin = GURL(kHnsSearchTabUrl);
  request_info->initiator_url = GURL(kHnsSearchTabUrl);
  request_info->resource_type = blink::mojom::ResourceType::kMainFrame;

  net::HttpRequestHeaders headers;
  const int rc = hns::OnBeforeStartTransaction_AdsStatusHeader(
      &headers, hns::ResponseCallback(), request_info);
  EXPECT_EQ(rc, net::OK);

  EXPECT_FALSE(headers.HasHeader(hns::kAdsStatusHeader));
}
