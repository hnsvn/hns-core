/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/core/wallet/wallet_util.h"

#include "base/test/task_environment.h"
#include "hns/components/hns_rewards/core/global_constants.h"
#include "hns/components/hns_rewards/core/mojom_structs.h"
#include "hns/components/hns_rewards/core/rewards_engine_impl_mock.h"
#include "hns/components/hns_rewards/core/state/state_keys.h"
#include "hns/components/hns_rewards/core/test/test_rewards_engine_client.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

// npm run test -- hns_unit_tests --filter=*WalletUtilTest*

using testing::_;
using testing::Test;
using testing::Values;
using testing::WithParamInterface;

namespace hns_rewards::internal::wallet {

mojom::ExternalWalletPtr ExternalWalletPtrFromJSON(std::string wallet_string,
                                                   std::string wallet_type);

class WalletUtilTest : public Test {};

TEST_F(WalletUtilTest, InvalidJSON) {
  EXPECT_FALSE(ExternalWalletPtrFromJSON("", "uphold"));
}

TEST_F(WalletUtilTest, ExternalWalletPtrFromJSON) {
  const char data[] =
      "{\n"
      "  \"token\": \"sI5rKiy6ijzbbJgE2MMFzAbTc6udYYXEi3wzS9iknP6n\",\n"
      "  \"address\": \"6a752063-8958-44d5-b5db-71543f18567d\",\n"
      "  \"one_time_string\": \"eda4c873eac72e1ecc30e77b25bb623b8b5bf99f\",\n"
      "  \"status\": 2,\n"
      "  \"user_name\": \"random_user\",\n"
      "  \"account_url\": \"https://random.domain/account\","
      "  \"login_url\": \"https://random.domain/login\","
      "  \"fees\": {\"hns\": 5.00}"
      "}\n";

  mojom::ExternalWalletPtr wallet = ExternalWalletPtrFromJSON(data, "uphold");
  EXPECT_EQ(wallet->token, "sI5rKiy6ijzbbJgE2MMFzAbTc6udYYXEi3wzS9iknP6n");
  EXPECT_EQ(wallet->address, "6a752063-8958-44d5-b5db-71543f18567d");
  EXPECT_EQ(wallet->one_time_string,
            "eda4c873eac72e1ecc30e77b25bb623b8b5bf99f");
  EXPECT_EQ(wallet->status, mojom::WalletStatus::kConnected);
  EXPECT_EQ(wallet->user_name, "random_user");
  EXPECT_EQ(wallet->account_url, "https://random.domain/account");
  EXPECT_EQ(wallet->login_url, "https://random.domain/login");
  EXPECT_NE(wallet->fees.find("hns"), wallet->fees.end());
  EXPECT_EQ(wallet->fees["hns"], 5.00);
}

using TransitionWalletCreateParamType =
    std::tuple<std::string,          // test name suffix
               mojom::WalletStatus,  // "to" WalletStatus
               bool,                 // wallet already exists
               bool                  // expected outcome
               >;

class TransitionWalletCreate
    : public Test,
      public WithParamInterface<TransitionWalletCreateParamType> {
 protected:
  base::test::TaskEnvironment task_environment_;
  MockRewardsEngineImpl mock_engine_impl_;
};

TEST_P(TransitionWalletCreate, Paths) {
  const auto& [ignore, to, wallet_already_exists, expected] = GetParam();

  EXPECT_CALL(*mock_engine_impl_.mock_client(),
              GetStringState(state::kWalletUphold, _))
      .Times(1)
      .WillOnce([&](const std::string&, auto callback) {
        std::move(callback).Run(wallet_already_exists
                                    ? FakeEncryption::Base64EncryptString("{}")
                                    : "");
      });

  ON_CALL(*mock_engine_impl_.mock_client(), RunDBTransaction(_, _))
      .WillByDefault([](mojom::DBTransactionPtr transaction, auto callback) {
        std::move(callback).Run(db_error_response->Clone());
      });

  const auto wallet =
      TransitionWallet(mock_engine_impl_, constant::kWalletUphold, to);
  EXPECT_EQ(static_cast<bool>(wallet), expected);

  if (wallet) {
    EXPECT_EQ(wallet->type, constant::kWalletUphold);
    EXPECT_FALSE(wallet->one_time_string.empty());
    EXPECT_FALSE(wallet->code_verifier.empty());
    EXPECT_EQ(wallet->status, to);
    EXPECT_FALSE(wallet->account_url.empty());
    EXPECT_TRUE(wallet->activity_url.empty());
    EXPECT_FALSE(wallet->login_url.empty());

    EXPECT_TRUE(wallet->token.empty());
    EXPECT_TRUE(wallet->address.empty());
  }

  task_environment_.RunUntilIdle();
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
  WalletUtilTest,
  TransitionWalletCreate,
  Values(
    TransitionWalletCreateParamType{
      "wallet_already_exists",
      mojom::WalletStatus::kNotConnected,
      true,
      false
    },
    TransitionWalletCreateParamType{
      "attempting_to_create_wallet_as_kConnected",
      mojom::WalletStatus::kConnected,
      false,
      false
    },
    TransitionWalletCreateParamType{
      "attempting_to_create_wallet_as_kLoggedOut",
      mojom::WalletStatus::kLoggedOut,
      false,
      false
    },
    TransitionWalletCreateParamType{
      "create_success",
      mojom::WalletStatus::kNotConnected,
      false,
      true
    }
  ),
  [](const auto& info) {
    return std::get<0>(info.param);
  }
);
// clang-format on

using TransitionWalletTransitionParamType =
    std::tuple<std::string,                                // test name suffix
               std::shared_ptr<mojom::ExternalWalletPtr>,  // from wallet
               mojom::WalletStatus,                        // "to" WalletStatus
               bool                                        // expected outcome
               >;

class TransitionWalletTransition
    : public Test,
      public WithParamInterface<TransitionWalletTransitionParamType> {
 protected:
  base::test::TaskEnvironment task_environment_;
  MockRewardsEngineImpl mock_engine_impl_;
};

TEST_P(TransitionWalletTransition, Paths) {
  const auto& [ignore, from_wallet, to, expected] = GetParam();

  ON_CALL(*mock_engine_impl_.mock_client(), RunDBTransaction(_, _))
      .WillByDefault([](mojom::DBTransactionPtr transaction, auto callback) {
        std::move(callback).Run(db_error_response->Clone());
      });

  const auto to_wallet =
      TransitionWallet(mock_engine_impl_, std::move(*from_wallet), to);
  EXPECT_EQ(static_cast<bool>(to_wallet), expected);

  if (to_wallet) {
    EXPECT_EQ(to_wallet->type, constant::kWalletUphold);
    EXPECT_EQ(to_wallet->status, to);
    EXPECT_FALSE(to_wallet->account_url.empty());
    EXPECT_FALSE(to_wallet->login_url.empty());

    if (to == mojom::WalletStatus::kConnected) {
      EXPECT_FALSE(to_wallet->activity_url.empty());
    } else {
      EXPECT_TRUE(to == mojom::WalletStatus::kNotConnected ||
                  to == mojom::WalletStatus::kLoggedOut);

      EXPECT_TRUE(to_wallet->activity_url.empty());

      EXPECT_FALSE(to_wallet->one_time_string.empty());
      EXPECT_FALSE(to_wallet->code_verifier.empty());

      EXPECT_TRUE(to_wallet->token.empty());
      EXPECT_TRUE(to_wallet->address.empty());
    }
  }

  task_environment_.RunUntilIdle();
}

// clang-format off
INSTANTIATE_TEST_SUITE_P(
  WalletUtilTest,
  TransitionWalletTransition,
  Values(
    TransitionWalletTransitionParamType{
      "kNotConnected__kNotConnected",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->status = mojom::WalletStatus::kNotConnected;
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kNotConnected,
      false
    },
    TransitionWalletTransitionParamType{
      "kNotConnected__kLoggedOut",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->status = mojom::WalletStatus::kNotConnected;
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kLoggedOut,
      false
    },
    TransitionWalletTransitionParamType{
      "kNotConnected__kConnected_no_token",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->status = mojom::WalletStatus::kNotConnected;
        wallet->address = "address";
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kConnected,
      false
    },
    TransitionWalletTransitionParamType{
      "kNotConnected__kConnected_no_address",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->status = mojom::WalletStatus::kNotConnected;
        wallet->token = "token";
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kConnected,
      false
    },
    TransitionWalletTransitionParamType{
      "kNotConnected__kConnected",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->type = constant::kWalletUphold;
        wallet->status = mojom::WalletStatus::kNotConnected;
        wallet->address = "address";
        wallet->token = "token";
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kConnected,
      true
    },
    TransitionWalletTransitionParamType{
      "kLoggedOut__kNotConnected",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->type = constant::kWalletUphold;
        wallet->status = mojom::WalletStatus::kLoggedOut;
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kNotConnected,
      true
    },
    TransitionWalletTransitionParamType{
      "kLoggedOut__kLoggedOut",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->status = mojom::WalletStatus::kLoggedOut;
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kLoggedOut,
      false
    },
    TransitionWalletTransitionParamType{
      "kLoggedOut__kConnected_no_token",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->status = mojom::WalletStatus::kLoggedOut;
        wallet->address = "address";
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kConnected,
      false
    },
    TransitionWalletTransitionParamType{
      "kLoggedOut__kConnected_no_address",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->status = mojom::WalletStatus::kLoggedOut;
        wallet->token = "token";
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kConnected,
      false
    },
    TransitionWalletTransitionParamType{
      "kLoggedOut__kConnected",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->type = constant::kWalletUphold;
        wallet->status = mojom::WalletStatus::kLoggedOut;
        wallet->address = "address";
        wallet->token = "token";
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kConnected,
      true
    },
    TransitionWalletTransitionParamType{
      "kConnected__kNotConnected",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->type = constant::kWalletUphold;
        wallet->status = mojom::WalletStatus::kConnected;
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kNotConnected,
      true
    },
    TransitionWalletTransitionParamType{
      "kConnected__kLoggedOut",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->type = constant::kWalletUphold;
        wallet->status = mojom::WalletStatus::kConnected;
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kLoggedOut,
      true
    },
    TransitionWalletTransitionParamType{
      "kConnected__kConnected",
      []{
        auto wallet = mojom::ExternalWallet::New();
        wallet->status = mojom::WalletStatus::kConnected;
        return std::make_shared<mojom::ExternalWalletPtr>(std::move(wallet));
      }(),
      mojom::WalletStatus::kConnected,
      false
    }
  ),
  [](const auto& info) {
    return (std::get<3>(info.param) ? "" : "in") +
           std::string("valid_transition_") + std::get<0>(info.param);
  }
);
// clang-format on

}  // namespace hns_rewards::internal::wallet
