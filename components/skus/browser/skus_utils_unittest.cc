/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/skus/browser/skus_utils.h"

#include "base/json/json_reader.h"
#include "hns/components/skus/browser/pref_names.h"
#include "components/prefs/testing_pref_service.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace skus {

TEST(SkusUtilsUnittest, GetDefaultEnvironment) {
#if defined(OFFICIAL_BUILD)
  EXPECT_EQ(GetDefaultEnvironment(), kEnvProduction);
#else
  EXPECT_EQ(GetDefaultEnvironment(), kEnvDevelopment);
#endif
}

TEST(SkusUtilsUnittest, GetEnvironmentForDomain) {
  EXPECT_EQ(GetEnvironmentForDomain("account.hns.com"), kEnvProduction);
  EXPECT_EQ(GetEnvironmentForDomain("vpn.hns.com"), kEnvProduction);

  EXPECT_EQ(GetEnvironmentForDomain("vpn.hnssoftware.com"), kEnvStaging);
  EXPECT_EQ(GetEnvironmentForDomain("account.hnssoftware.com"), kEnvStaging);

  EXPECT_EQ(GetEnvironmentForDomain("vpn.hns.software"), kEnvDevelopment);
  EXPECT_EQ(GetEnvironmentForDomain("account.hns.software"), kEnvDevelopment);
}

TEST(SkusUtilsUnittest, GetDomain) {
  EXPECT_EQ(GetEnvironmentForDomain(GetDomain("vpn", kEnvDevelopment)),
            kEnvDevelopment);
  EXPECT_EQ(GetEnvironmentForDomain(GetDomain("talk", kEnvDevelopment)),
            kEnvDevelopment);

  EXPECT_EQ(GetEnvironmentForDomain(GetDomain("vpn", kEnvStaging)),
            kEnvStaging);
  EXPECT_EQ(GetEnvironmentForDomain(GetDomain("talk", kEnvStaging)),
            kEnvStaging);

  EXPECT_EQ(GetEnvironmentForDomain(GetDomain("vpn", kEnvProduction)),
            kEnvProduction);
  EXPECT_EQ(GetEnvironmentForDomain(GetDomain("talk", kEnvProduction)),
            kEnvProduction);
}

TEST(SkusUtilsUnittest, Migrate) {
  TestingPrefServiceSimple profile_pref_service;
  skus::RegisterProfilePrefsForMigration(profile_pref_service.registry());
  TestingPrefServiceSimple local_state_pref_service;
  skus::RegisterLocalStatePrefs(local_state_pref_service.registry());
  EXPECT_FALSE(local_state_pref_service.HasPrefPath(
      skus::prefs::kSkusStateMigratedToLocalState));
  EXPECT_FALSE(local_state_pref_service.HasPrefPath(skus::prefs::kSkusState));
  auto skus_settings = base::JSONReader::Read(R"({
    "skus":
      {
          "state":
          {
              "skus:development": "{}"
          }
      }
    })");
  profile_pref_service.Set(skus::prefs::kSkusState, *skus_settings);
  skus::MigrateSkusSettings(&profile_pref_service, &local_state_pref_service);

  EXPECT_FALSE(profile_pref_service.HasPrefPath(skus::prefs::kSkusState));
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(skus::prefs::kSkusState));
  EXPECT_EQ(local_state_pref_service.GetDict(skus::prefs::kSkusState),
            *skus_settings);
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      skus::prefs::kSkusStateMigratedToLocalState));
}

TEST(SkusUtilsUnittest, AlreadyMigrated) {
  TestingPrefServiceSimple profile_pref_service;
  skus::RegisterProfilePrefsForMigration(profile_pref_service.registry());
  TestingPrefServiceSimple local_state_pref_service;
  skus::RegisterLocalStatePrefs(local_state_pref_service.registry());
  local_state_pref_service.SetBoolean(
      skus::prefs::kSkusStateMigratedToLocalState, true);
  auto existing_skus = base::JSONReader::Read(R"({
      "skus": {
            "state":{
              "migrated_to_local_state": true
            }
        }
      })");
  local_state_pref_service.Set(skus::prefs::kSkusState, *existing_skus);
  auto skus_settings = base::JSONReader::Read(R"({
    "skus":
      {
          "state":
          {
              "development": "{}"
          }
      }
    })");
  profile_pref_service.Set(skus::prefs::kSkusState, *skus_settings);
  skus::MigrateSkusSettings(&profile_pref_service, &local_state_pref_service);

  EXPECT_TRUE(local_state_pref_service.HasPrefPath(skus::prefs::kSkusState));
  EXPECT_EQ(local_state_pref_service.GetDict(skus::prefs::kSkusState),
            *existing_skus);
  EXPECT_TRUE(local_state_pref_service.HasPrefPath(
      skus::prefs::kSkusStateMigratedToLocalState));
}

}  // namespace skus
