/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/common/hns_vpn_utils.h"

#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/hns_vpn/common/pref_names.h"
#include "hns/components/skus/browser/skus_utils.h"
#include "hns/components/skus/common/features.h"
#include "components/prefs/pref_service.h"
#include "components/prefs/testing_pref_service.h"
#include "components/sync_preferences/testing_pref_service_syncable.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(HnsVPNUtilsUnitTest, MigrateAndMerge) {
  sync_preferences::TestingPrefServiceSyncable profile_pref_service;
  hns_vpn::RegisterProfilePrefs(profile_pref_service.registry());
  TestingPrefServiceSimple local_state_pref_service;
  hns_vpn::RegisterLocalStatePrefs(local_state_pref_service.registry());
  EXPECT_FALSE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNLocalStateMigrated));
  auto vpn_settings = base::JSONReader::Read(R"(
        {
            "device_region_name": "eu-de",
            "env": "development",
            "region_list":
            [
                {
                    "continent": "oceania",
                    "country-iso-code": "AU",
                    "name": "au-au",
                    "name-pretty": "Australia"
                }
            ]
        })");
  profile_pref_service.Set(hns_vpn::prefs::kHnsVPNRootPref, *vpn_settings);
  auto p3a_settings = base::JSONReader::Read(R"(
          {
            "days_in_month_used":
            [
                {
                    "day": 1663448400.0,
                    "value": 1.0
                }
            ],
            "first_use_time": "13307922000000000",
            "last_use_time": "13307922000000000"
          })");
  local_state_pref_service.Set(hns_vpn::prefs::kHnsVPNRootPref,
                               *p3a_settings);
  hns_vpn::MigrateVPNSettings(&profile_pref_service,
                                &local_state_pref_service);

  EXPECT_FALSE(
      profile_pref_service.HasPrefPath(hns_vpn::prefs::kHnsVPNRootPref));
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNRootPref));
  base::Value result = vpn_settings->Clone();
  auto& result_dict = result.GetDict();
  result_dict.Merge(p3a_settings->GetDict().Clone());
  EXPECT_EQ(
      local_state_pref_service.GetDict(hns_vpn::prefs::kHnsVPNRootPref),
      result);
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNLocalStateMigrated));
}

TEST(HnsVPNUtilsUnitTest, Migrate) {
  sync_preferences::TestingPrefServiceSyncable profile_pref_service;
  hns_vpn::RegisterProfilePrefs(profile_pref_service.registry());
  TestingPrefServiceSimple local_state_pref_service;
  hns_vpn::RegisterLocalStatePrefs(local_state_pref_service.registry());
  EXPECT_FALSE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNLocalStateMigrated));

  auto vpn_settings = base::JSONReader::Read(R"(
        {
            "show_button": true,
            "device_region_name": "eu-de",
            "env": "development",
            "region_list":
            [
                {
                    "continent": "oceania",
                    "country-iso-code": "AU",
                    "name": "au-au",
                    "name-pretty": "Australia"
                }
            ]
        })");
  profile_pref_service.Set(hns_vpn::prefs::kHnsVPNRootPref, *vpn_settings);
  hns_vpn::MigrateVPNSettings(&profile_pref_service,
                                &local_state_pref_service);
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNRootPref));
  vpn_settings->GetIfDict()->Remove("show_button");
  EXPECT_EQ(
      local_state_pref_service.GetDict(hns_vpn::prefs::kHnsVPNRootPref),
      *vpn_settings);
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNLocalStateMigrated));
}

TEST(HnsVPNUtilsUnitTest, NoMigration) {
  sync_preferences::TestingPrefServiceSyncable profile_pref_service;
  hns_vpn::RegisterProfilePrefs(profile_pref_service.registry());
  TestingPrefServiceSimple local_state_pref_service;
  hns_vpn::RegisterLocalStatePrefs(local_state_pref_service.registry());
  EXPECT_FALSE(
      profile_pref_service.HasPrefPath(hns_vpn::prefs::kHnsVPNRootPref));

  auto p3a_settings = base::JSONReader::Read(R"(
          {
            "days_in_month_used":
            [
                {
                    "day": 1663448400.0,
                    "value": 1.0
                }
            ],
            "first_use_time": "13307922000000000",
            "last_use_time": "13307922000000000"
          })");
  local_state_pref_service.Set(hns_vpn::prefs::kHnsVPNRootPref,
                               *p3a_settings);
  hns_vpn::MigrateVPNSettings(&profile_pref_service,
                                &local_state_pref_service);

  EXPECT_FALSE(
      profile_pref_service.HasPrefPath(hns_vpn::prefs::kHnsVPNRootPref));
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNRootPref));
  EXPECT_EQ(
      local_state_pref_service.GetDict(hns_vpn::prefs::kHnsVPNRootPref),
      *p3a_settings);
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNLocalStateMigrated));
}

TEST(HnsVPNUtilsUnitTest, AlreadyMigrated) {
  sync_preferences::TestingPrefServiceSyncable profile_pref_service;
  hns_vpn::RegisterProfilePrefs(profile_pref_service.registry());
  TestingPrefServiceSimple local_state_pref_service;
  hns_vpn::RegisterLocalStatePrefs(local_state_pref_service.registry());
  local_state_pref_service.SetBoolean(
      hns_vpn::prefs::kHnsVPNLocalStateMigrated, true);

  auto vpn_settings = base::JSONReader::Read(R"(
        {
            "show_button": true,
            "device_region_name": "eu-de",
            "env": "development",
            "region_list":
            [
                {
                    "continent": "oceania",
                    "country-iso-code": "AU",
                    "name": "au-au",
                    "name-pretty": "Australia"
                }
            ]
        })");
  profile_pref_service.Set(hns_vpn::prefs::kHnsVPNRootPref, *vpn_settings);
  auto p3a_settings = base::JSONReader::Read(R"(
          {
            "days_in_month_used":
            [
                {
                    "day": 1663448400.0,
                    "value": 1.0
                }
            ],
            "first_use_time": "13307922000000000",
            "last_use_time": "13307922000000000"
          })");
  local_state_pref_service.Set(hns_vpn::prefs::kHnsVPNRootPref,
                               *p3a_settings);
  hns_vpn::MigrateVPNSettings(&profile_pref_service,
                                &local_state_pref_service);
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNRootPref));
  EXPECT_EQ(
      local_state_pref_service.GetDict(hns_vpn::prefs::kHnsVPNRootPref),
      *p3a_settings);
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      hns_vpn::prefs::kHnsVPNLocalStateMigrated));
}

TEST(HnsVPNUtilsUnitTest, VPNPaymentsEnv) {
  EXPECT_EQ("production",
            hns_vpn::GetHnsVPNPaymentsEnv(skus::kEnvProduction));
  EXPECT_EQ("staging", hns_vpn::GetHnsVPNPaymentsEnv(skus::kEnvStaging));
  EXPECT_EQ("development",
            hns_vpn::GetHnsVPNPaymentsEnv(skus::kEnvDevelopment));
}

TEST(HnsVPNUtilsUnitTest, IsHnsVPNEnabled) {
  sync_preferences::TestingPrefServiceSyncable profile_pref_service;
  hns_vpn::RegisterProfilePrefs(profile_pref_service.registry());
  base::test::ScopedFeatureList scoped_feature_list;
  scoped_feature_list.InitWithFeatures(
      {hns_vpn::features::kHnsVPN, skus::features::kSkusFeature}, {});

  EXPECT_TRUE(hns_vpn::IsHnsVPNFeatureEnabled());
  profile_pref_service.SetManagedPref(
      hns_vpn::prefs::kManagedHnsVPNDisabled, base::Value(false));
  EXPECT_TRUE(hns_vpn::IsHnsVPNEnabled(&profile_pref_service));
  profile_pref_service.SetManagedPref(
      hns_vpn::prefs::kManagedHnsVPNDisabled, base::Value(true));
  EXPECT_FALSE(hns_vpn::IsHnsVPNEnabled(&profile_pref_service));
}

TEST(HnsVPNUtilsUnitTest, FeatureTest) {
#if !BUILDFLAG(IS_LINUX)
  EXPECT_TRUE(hns_vpn::IsHnsVPNFeatureEnabled());
#else
  EXPECT_FALSE(hns_vpn::IsHnsVPNFeatureEnabled());
#endif
}
