/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>
#include <tuple>

#include "hns/components/hns_shields/browser/hns_farbling_service.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/abseil-cpp/absl/random/random.h"
#include "url/gurl.h"

namespace {
const uint64_t kTestSessionToken = 123456789;
const uint64_t kTestIncognitoSessionToken = 234567890;
const uint64_t kAnotherTestSessionToken = 45678;
const uint64_t kAnotherTestIncognitoSessionToken = 56789;
}  // namespace

class HnsFarblingServiceTest : public testing::Test {
 public:
  HnsFarblingServiceTest() = default;
  HnsFarblingServiceTest(const HnsFarblingServiceTest&) = delete;
  HnsFarblingServiceTest& operator=(const HnsFarblingServiceTest&) = delete;
  ~HnsFarblingServiceTest() override = default;

  void SetUp() override {
    farbling_service_ = std::make_unique<hns::HnsFarblingService>();
    farbling_service_->set_session_tokens_for_testing(
        kTestSessionToken, kTestIncognitoSessionToken);
  }

  hns::HnsFarblingService* farbling_service() {
    return farbling_service_.get();
  }

 private:
  std::unique_ptr<hns::HnsFarblingService> farbling_service_;
};

TEST_F(HnsFarblingServiceTest, SessionTokens) {
  EXPECT_EQ(farbling_service()->session_token(false /* is_off_the_record */),
            kTestSessionToken);
  EXPECT_EQ(farbling_service()->session_token(true /* is_off_the_record */),
            kTestIncognitoSessionToken);
}

TEST_F(HnsFarblingServiceTest, PRNGKnownValues) {
  const std::array<std::tuple<GURL, bool, uint64_t>, 4> test_cases = {
      std::make_tuple<>(GURL("http://a.com"), false, 16188622623906601575UL),
      std::make_tuple<>(GURL("http://a.com"), true, 8942885125771927068UL),
      std::make_tuple<>(GURL("http://b.com"), false, 10059331952077172763UL),
      std::make_tuple<>(GURL("http://b.com"), true, 6702825749149599294UL),
  };
  for (const auto& c : test_cases) {
    hns::FarblingPRNG prng;
    ASSERT_TRUE(farbling_service()->MakePseudoRandomGeneratorForURL(
        std::get<0>(c), std::get<1>(c), &prng));
    EXPECT_EQ(prng(), std::get<2>(c));
  }
}

TEST_F(HnsFarblingServiceTest, PRNGKnownValuesDifferentSeeds) {
  farbling_service()->set_session_tokens_for_testing(
      kAnotherTestSessionToken, kAnotherTestIncognitoSessionToken);
  const std::array<std::tuple<GURL, bool, uint64_t>, 4> test_cases = {
      std::make_tuple<>(GURL("http://a.com"), false, 6565599272117158152UL),
      std::make_tuple<>(GURL("http://a.com"), true, 18152828989207203999UL),
      std::make_tuple<>(GURL("http://b.com"), false, 10499595974068024348UL),
      std::make_tuple<>(GURL("http://b.com"), true, 7485037097853289552UL),
  };
  for (const auto& c : test_cases) {
    hns::FarblingPRNG prng;
    ASSERT_TRUE(farbling_service()->MakePseudoRandomGeneratorForURL(
        std::get<0>(c), std::get<1>(c), &prng));
    EXPECT_EQ(prng(), std::get<2>(c));
  }
}

TEST_F(HnsFarblingServiceTest, InvalidDomains) {
  const std::array<GURL, 8> test_cases = {
      GURL("about:blank"),
      GURL("hns://settings"),
      GURL("chrome://version"),
      GURL("gopher://hns.com"),
      GURL("file:///etc/passwd"),
      GURL("javascript:alert(1)"),
      GURL("data:text/plain;base64,"),
      GURL(""),
  };
  for (const auto& url : test_cases) {
    hns::FarblingPRNG prng;
    EXPECT_FALSE(
        farbling_service()->MakePseudoRandomGeneratorForURL(url, false, &prng));
    EXPECT_FALSE(
        farbling_service()->MakePseudoRandomGeneratorForURL(url, true, &prng));
  }
}
