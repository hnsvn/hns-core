/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_sync/hns_sync_prefs.h"

#include <memory>

#include "base/base64.h"
#include "base/logging.h"
#include "base/test/gtest_util.h"
#include "base/test/task_environment.h"
#include "build/build_config.h"
#include "components/os_crypt/sync/os_crypt_mocker.h"
#include "components/prefs/testing_pref_service.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace hns_sync {

namespace {

const char kValidSyncCode[] =
    "fringe digital begin feed equal output proof cheap "
    "exotic ill sure question trial squirrel glove celery "
    "awkward push jelly logic broccoli almost grocery drift";

}  // namespace

class HnsSyncPrefsTest : public testing::Test {
 protected:
  HnsSyncPrefsTest() {
    hns_sync::Prefs::RegisterProfilePrefs(pref_service_.registry());
    hns_sync_prefs_ = std::make_unique<hns_sync::Prefs>(&pref_service_);
  }

  hns_sync::Prefs* hns_sync_prefs() { return hns_sync_prefs_.get(); }

  PrefService* pref_service() { return &pref_service_; }

  base::test::SingleThreadTaskEnvironment task_environment_;
  TestingPrefServiceSimple pref_service_;
  std::unique_ptr<hns_sync::Prefs> hns_sync_prefs_;
};

#if BUILDFLAG(IS_APPLE)

// On macOS expected to see decryption failure when reading seed on
// locked keyring
TEST_F(HnsSyncPrefsTest, ValidPassphraseKeyringLocked) {
  OSCryptMocker::SetUp();

  hns_sync_prefs()->SetSeed(kValidSyncCode);

  bool failed_to_decrypt = false;
  OSCryptMocker::SetBackendLocked(true);
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), "");
  EXPECT_TRUE(failed_to_decrypt);

  OSCryptMocker::TearDown();
}

#endif  // BUILDFLAG(IS_APPLE)

TEST_F(HnsSyncPrefsTest, FailedToDecryptHnsSeedValue) {
  OSCryptMocker::SetUp();

  // Empty seed is expected as valid when sync is not turned on
  bool failed_to_decrypt = false;
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), "");
  EXPECT_FALSE(failed_to_decrypt);

  // Valid code does not set failed_to_decrypt
  hns_sync_prefs()->SetSeed(kValidSyncCode);
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), kValidSyncCode);
  EXPECT_FALSE(failed_to_decrypt);

  // Wrong base64-encoded seed must set failed_to_decrypt to true
  const char kWrongBase64String[] = "AA%BB";
  std::string base64_decoded;
  EXPECT_FALSE(base::Base64Decode(kWrongBase64String, &base64_decoded));
  pref_service()->SetString(hns_sync::Prefs::GetSeedPath(),
                            kWrongBase64String);
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), "");
  EXPECT_TRUE(failed_to_decrypt);

  // Valid base64 string but not valid encrypted string must set
  // failed_to_decrypt to true. Note: "v10" prefix is important to make
  // DecryptString fail. Also the remaining string must be 12 or more bytes.
  std::string valid_base64_string;
  base::Base64Encode("v10_AABBCCDDEEFF", &valid_base64_string);
  pref_service()->SetString(hns_sync::Prefs::GetSeedPath(),
                            valid_base64_string);
  EXPECT_EQ(hns_sync_prefs()->GetSeed(&failed_to_decrypt), "");
  EXPECT_TRUE(failed_to_decrypt);

  OSCryptMocker::TearDown();
}

using HnsSyncPrefsDeathTest = HnsSyncPrefsTest;

// Some tests are failing for Windows x86 CI,
// See https://github.com/hnsvn/hns-browser/issues/22767
#if BUILDFLAG(IS_WIN) && defined(ARCH_CPU_X86)
#define MAYBE_GetSeedOutNullptrCHECK DISABLED_GetSeedOutNullptrCHECK
#else
#define MAYBE_GetSeedOutNullptrCHECK GetSeedOutNullptrCHECK
#endif
TEST_F(HnsSyncPrefsDeathTest, MAYBE_GetSeedOutNullptrCHECK) {
  EXPECT_CHECK_DEATH(hns_sync_prefs()->GetSeed(nullptr));
}

}  // namespace hns_sync
