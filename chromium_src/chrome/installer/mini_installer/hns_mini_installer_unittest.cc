// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/installer/mini_installer/mini_installer.h"

#include "testing/gtest/include/gtest/gtest.h"

namespace mini_installer {

class HnsMiniInstallerTest: public testing::Test {
 public:
  HnsMiniInstallerTest() {
  }
  ~HnsMiniInstallerTest() override {}
};


TEST_F(HnsMiniInstallerTest, HasNoReferralCode) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup.exe", &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasStandardReferralCode) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"HnsBrowserSetup-FOO123.exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(HnsMiniInstallerTest, HasStandardReferralCodeWithLowercase) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"HnsBrowserSetup-foo123.exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(HnsMiniInstallerTest, HasStandardReferralCodeWithPath) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"c:/foo/bar/HnsBrowserSetup-FOO123.exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(HnsMiniInstallerTest,
                HasStandardReferralCodeWithDeduplicatingSuffix) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"c:/foo/bar/HnsBrowserSetup-FOO123 (1).exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(HnsMiniInstallerTest,
                HasStandardReferralCodeWithDeduplicatingSuffixNoSpaces) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"c:/foo/bar/HnsBrowserSetup-FOO123(1).exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(HnsMiniInstallerTest,
                HasStandardReferralCodeWithDeduplicatingSuffixExtraSpaces) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(
                          L"c:/foo/bar/HnsBrowserSetup-FOO123   (1).exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"FOO123");
}

TEST_F(HnsMiniInstallerTest, HasInvalidStandardReferralCodeReversed) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup-123FOO.exe",
                          &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasInvalidStandardReferralCodeNoDigits) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup-FOO.exe", &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasInvalidStandardReferralCodeNoLetters) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup-123.exe", &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasInvalidStandardReferralCodeTooManyDigits) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup-FOO1234.exe",
                          &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasInvalidStandardReferralCodeTooFewDigits) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup-FOO12.exe",
                          &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasInvalidStandardReferralCodeTooManyLetters) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup-FOOO123.exe",
                          &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasInvalidStandardReferralCodeTooFewLetters) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup-FO123.exe",
                          &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasExtendedReferralCode) {
  ReferralCodeString referral_code;
  EXPECT_TRUE(ParseReferralCode(L"HnsBrowserSetup-extended-code.exe",
                          &referral_code));
  EXPECT_STREQ(referral_code.get(), L"extended-code");
}

TEST_F(HnsMiniInstallerTest,
                HasInvalidExtendedReferralCodeNonAlphabeticCharacters) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(
                          L"HnsBrowserSetup-invalid-extended-c0de.exe",
                          &referral_code));
}

TEST_F(HnsMiniInstallerTest, HasInvalidExtendedReferralCodeTooFewWords) {
  ReferralCodeString referral_code;
  EXPECT_FALSE(ParseReferralCode(L"HnsBrowserSetup-invalidextendedcode.exe",
                          &referral_code));
}

}  // namespace mini_installer
