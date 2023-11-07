/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/browser/connection/wireguard/credentials/hns_vpn_wireguard_profile_credentials.h"

#include "testing/gtest/include/gtest/gtest.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_vpn {

class HnsWireguardProfileCredentialsUnitTest : public testing::Test {
 public:
  HnsWireguardProfileCredentialsUnitTest() = default;
  ~HnsWireguardProfileCredentialsUnitTest() override = default;
};

TEST_F(HnsWireguardProfileCredentialsUnitTest, Parse) {
  wireguard::WireguardProfileCredentials test(
      "server_public_key", "client_private_key", "mapped_ip4_address",
      "client_id", "api_auth_token");

  EXPECT_EQ(test.server_public_key, "server_public_key");
  EXPECT_EQ(test.client_private_key, "client_private_key");
  EXPECT_EQ(test.mapped_ip4_address, "mapped_ip4_address");
  EXPECT_EQ(test.client_id, "client_id");
  EXPECT_EQ(test.api_auth_token, "api_auth_token");
  EXPECT_TRUE(test.IsValid());
  EXPECT_EQ(
      *wireguard::WireguardProfileCredentials::FromString(*test.ToString()),
      test);

  EXPECT_EQ(*wireguard::WireguardProfileCredentials::FromServerResponse(
                R"({
    "api-auth-token":"api_auth_token",
    "client-id":"client_id",
    "mapped-ipv4-address":"mapped_ip4_address",
    "server-public-key":"server_public_key"
  })",
                "client_private_key"),
            test);

  // Broken json cases
  EXPECT_FALSE(
      wireguard::WireguardProfileCredentials::FromString(std::string()));
  EXPECT_FALSE(wireguard::WireguardProfileCredentials::FromString("{}"));
  EXPECT_FALSE(wireguard::WireguardProfileCredentials::FromString("{,}"));

  // Missed one of items.
  EXPECT_FALSE(wireguard::WireguardProfileCredentials::FromString(R"({
    "api-auth-token":"api_auth_token",
    "client-id":"client_id",
    "mapped-ipv4-address":"mapped_ip4_address",
    "server-public-key":"server_public_key"
  })"));
  EXPECT_FALSE(wireguard::WireguardProfileCredentials::FromString(R"({
    "client-id":"client_id",
    "mapped-ipv4-address":"mapped_ip4_address",
    "server-public-key":"server_public_key",
    "client_private_key": "client_private_key"
  })"));
  EXPECT_FALSE(wireguard::WireguardProfileCredentials::FromString(R"({
    "api-auth-token":"api_auth_token",
    "mapped-ipv4-address":"mapped_ip4_address",
    "server-public-key":"server_public_key",
    "client_private_key": "client_private_key"
  })"));
  EXPECT_FALSE(wireguard::WireguardProfileCredentials::FromString(R"({
    "api-auth-token":"api_auth_token",
    "client-id":"client_id",
    "server-public-key":"server_public_key",
    "client_private_key": "client_private_key"
  })"));
  EXPECT_FALSE(wireguard::WireguardProfileCredentials::FromString(R"({
    "api-auth-token":"api_auth_token",
    "client-id":"client_id",
    "mapped-ipv4-address":"mapped_ip4_address",
    "client_private_key": "client_private_key"
  })"));

  // Do not serialize invalid state
  test.client_private_key = "";
  EXPECT_FALSE(test.IsValid());
  EXPECT_FALSE(test.ToString());
}

}  // namespace hns_vpn
