/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/decentralized_dns_network_delegate_helper.h"

#include <memory>

#include "base/functional/callback_helpers.h"
#include "base/run_loop.h"
#include "base/test/scoped_feature_list.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/browser/net/url_context.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/browser/json_rpc_service_test_utils.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/eth_abi_utils.h"
#include "hns/components/hns_wallet/common/hex_utils.h"
#include "hns/components/decentralized_dns/core/constants.h"
#include "hns/components/decentralized_dns/core/pref_names.h"
#include "hns/components/decentralized_dns/core/utils.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "chrome/test/base/testing_profile.h"
#include "components/prefs/testing_pref_service.h"
#include "content/public/test/browser_task_environment.h"
#include "net/base/net_errors.h"
#include "services/data_decoder/public/cpp/test_support/in_process_data_decoder.h"
#include "services/network/public/cpp/weak_wrapper_shared_url_loader_factory.h"
#include "services/network/test/test_url_loader_factory.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "url/gurl.h"

using hns::ResponseCallback;

namespace decentralized_dns {

class DecentralizedDnsNetworkDelegateHelperTest : public testing::Test {
 public:
  DecentralizedDnsNetworkDelegateHelperTest()
      : local_state_(std::make_unique<ScopedTestingLocalState>(
            TestingBrowserProcess::GetGlobal())) {}

  ~DecentralizedDnsNetworkDelegateHelperTest() override = default;

  void SetUp() override {
    profile_ = std::make_unique<TestingProfile>();

    shared_url_loader_factory_ =
        base::MakeRefCounted<network::WeakWrapperSharedURLLoaderFactory>(
            &test_url_loader_factory_);
    json_rpc_service_ =
        hns_wallet::JsonRpcServiceFactory::GetServiceForContext(
            browser_context());
    json_rpc_service_->SetAPIRequestHelperForTesting(
        shared_url_loader_factory_);
  }

  void TearDown() override {
    profile_.reset();
    local_state_.reset();
  }

  content::BrowserContext* browser_context() { return profile_.get(); }
  TestingProfile* profile() { return profile_.get(); }
  PrefService* local_state() { return local_state_->Get(); }
  network::TestURLLoaderFactory& test_url_loader_factory() {
    return test_url_loader_factory_;
  }

 private:
  content::BrowserTaskEnvironment task_environment_;
  std::unique_ptr<TestingProfile> profile_;
  std::unique_ptr<ScopedTestingLocalState> local_state_;
  network::TestURLLoaderFactory test_url_loader_factory_;
  scoped_refptr<network::SharedURLLoaderFactory> shared_url_loader_factory_;
  raw_ptr<hns_wallet::JsonRpcService> json_rpc_service_ = nullptr;
  data_decoder::test::InProcessDataDecoder in_process_data_decoder_;
};

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       DecentralizedDnsPreRedirectWork) {
  GURL url("http://hns.crypto");
  auto hns_request_info = std::make_shared<hns::HnsRequestInfo>(url);
  hns_request_info->browser_context = profile();

  // No redirect if resolve method is not set to Ethereum.
  EXPECT_FALSE(IsUnstoppableDomainsResolveMethodEnabled(local_state()));
  int rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
      base::DoNothing(), hns_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  local_state()->SetInteger(kUnstoppableDomainsResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ENABLED));
  EXPECT_TRUE(IsUnstoppableDomainsResolveMethodEnabled(local_state()));

  // No redirect for OTR context.
  hns_request_info->browser_context =
      profile()->GetPrimaryOTRProfile(/*create_if_needed=*/true);
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          hns_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());
  hns_request_info->browser_context = profile();

  // TLD is not .crypto
  hns_request_info->request_url = GURL("http://test.com");
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          hns_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());
  hns_request_info->request_url = url;

  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          hns_request_info);
  EXPECT_EQ(rc, net::ERR_IO_PENDING);

  // No redirect if ENS resolve method is not set to Ethereum.
  EXPECT_FALSE(IsENSResolveMethodEnabled(local_state()));
  hns_request_info->request_url = GURL("http://hns.eth");
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          hns_request_info);
  EXPECT_EQ(rc, net::OK);
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  local_state()->SetInteger(kENSResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ENABLED));
  EXPECT_TRUE(IsENSResolveMethodEnabled(local_state()));
  hns_request_info->request_url = GURL("http://hns.eth");
  rc = OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(base::DoNothing(),
                                                          hns_request_info);
  EXPECT_EQ(rc, net::ERR_IO_PENDING);
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       DecentralizedDnsPreRedirectTLDs) {
  local_state()->SetInteger(kUnstoppableDomainsResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ENABLED));
  struct TestCase {
    const char* url;
    bool is_valid;
  } test_cases[] = {
      {"https://hns.crypto", true},
      {"https://hns.x", true},
      {"https://hns.coin", false},
      {"https://hns.nft", true},
      {"https://hns.dao", true},
      {"https://hns.wallet", true},
      {"https://hns.888", false},
      {"https://hns.blockchain", true},
      {"https://hns.bitcoin", true},
      {"https://hns.zil", true},
      {"https://hns", false},
      {"https://hns.com", false},
      {"", false},
  };

  for (const auto& test_case : test_cases) {
    auto hns_request_info =
        std::make_shared<hns::HnsRequestInfo>(GURL(test_case.url));
    hns_request_info->browser_context = profile();
    EXPECT_EQ(test_case.is_valid ? net::ERR_IO_PENDING : net::OK,
              OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                  base::DoNothing(), hns_request_info));
  }
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       UnstoppableDomainsRedirectWork) {
  local_state()->SetInteger(kUnstoppableDomainsResolveMethod,
                            static_cast<int>(ResolveMethodTypes::ENABLED));

  GURL url("http://hns.crypto");
  auto hns_request_info = std::make_shared<hns::HnsRequestInfo>(url);
  hns_request_info->browser_context = profile();

  auto polygon_spec = hns_wallet::GetUnstoppableDomainsRpcUrl(
                          hns_wallet::mojom::kPolygonMainnetChainId)
                          .spec();
  auto eth_spec = hns_wallet::GetUnstoppableDomainsRpcUrl(
                      hns_wallet::mojom::kMainnetChainId)
                      .spec();

  // No redirect for failed requests.
  EXPECT_EQ(net::ERR_IO_PENDING,
            OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                base::DoNothing(), hns_request_info));
  test_url_loader_factory().SimulateResponseForPendingRequest(
      polygon_spec,
      hns_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://hns.com"}),
      net::HTTP_REQUEST_TIMEOUT);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      eth_spec,
      hns_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://hns.com"}),
      net::HTTP_REQUEST_TIMEOUT);
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  // Polygon result.
  EXPECT_EQ(net::ERR_IO_PENDING,
            OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                base::DoNothing(), hns_request_info));
  test_url_loader_factory().SimulateResponseForPendingRequest(
      polygon_spec,
      hns_wallet::MakeJsonRpcStringArrayResponse(
          {"", "", "", "", "", "https://hns.com"}),
      net::HTTP_OK);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      eth_spec,
      hns_wallet::MakeJsonRpcStringArrayResponse(
          {"hash", "", "", "", "", ""}),
      net::HTTP_OK);
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(hns_request_info->new_url_spec, "https://hns.com/");

  // Eth result.
  EXPECT_EQ(net::ERR_IO_PENDING,
            OnBeforeURLRequest_DecentralizedDnsPreRedirectWork(
                base::DoNothing(), hns_request_info));
  test_url_loader_factory().SimulateResponseForPendingRequest(
      polygon_spec,
      hns_wallet::MakeJsonRpcStringArrayResponse({"", "", "", "", "", ""}),
      net::HTTP_OK);
  test_url_loader_factory().SimulateResponseForPendingRequest(
      eth_spec,
      hns_wallet::MakeJsonRpcStringArrayResponse(
          {"hash", "", "", "", "", ""}),
      net::HTTP_OK);
  base::RunLoop().RunUntilIdle();
  EXPECT_EQ(hns_request_info->new_url_spec, "ipfs://hash");
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest, EnsRedirectWork) {
  GURL url("http://brantly.eth");
  auto hns_request_info = std::make_shared<hns::HnsRequestInfo>(url);

  // No redirect for failed requests.
  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), hns_request_info, {}, false,
      hns_wallet::mojom::ProviderError::kInternalError, "todo");
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), hns_request_info, {}, false,
      hns_wallet::mojom::ProviderError::kSuccess, "");
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  // No redirect for invalid content hash.
  std::string content_hash_encoded_string =
      "0x0000000000000000000000000000000000000000000000000000000000000020000000"
      "000000000000000000000000000000000000000000000000000000002655010170122023"
      "e0160eec32d7875c19c5ac7c03bc1f306dc260080d621454bc5f631e7310a70000000000"
      "000000000000000000000000000000000000000000";

  auto content_hash = *hns_wallet::eth_abi::ExtractBytesFromTuple(
      *hns_wallet::PrefixedHexStringToBytes(content_hash_encoded_string), 0);
  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), hns_request_info, content_hash, false,
      hns_wallet::mojom::ProviderError::kSuccess, "");
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  // Redirect for valid content hash.
  content_hash_encoded_string =
      "0x0000000000000000000000000000000000000000000000000000000000000020000000"
      "0000000000000000000000000000000000000000000000000000000026e3010170122023"
      "e0160eec32d7875c19c5ac7c03bc1f306dc260080d621454bc5f631e7310a70000000000"
      "000000000000000000000000000000000000000000";

  content_hash = *hns_wallet::eth_abi::ExtractBytesFromTuple(
      *hns_wallet::PrefixedHexStringToBytes(content_hash_encoded_string), 0);
  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), hns_request_info, content_hash, false,
      hns_wallet::mojom::ProviderError::kSuccess, "");
  EXPECT_EQ(hns_request_info->new_url_spec,
            "ipfs://"
            "bafybeibd4ala53bs26dvygofvr6ahpa7gbw4eyaibvrbivf4l5rr44yqu4");

  EXPECT_FALSE(hns_request_info->pending_error.has_value());
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest,
       EnsRedirect_OffchainLookupRequired) {
  GURL url("http://brantly.eth");
  auto hns_request_info = std::make_shared<hns::HnsRequestInfo>(url);

  // Offchain lookup required.
  OnBeforeURLRequest_EnsRedirectWork(
      base::DoNothing(), hns_request_info, {}, true,
      hns_wallet::mojom::ProviderError::kSuccess, "");
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());
  EXPECT_EQ(hns_request_info->pending_error,
            net::ERR_ENS_OFFCHAIN_LOOKUP_NOT_SELECTED);
}

TEST_F(DecentralizedDnsNetworkDelegateHelperTest, SnsRedirectWork) {
  GURL url("http://test.sol");
  auto hns_request_info = std::make_shared<hns::HnsRequestInfo>(url);

  // No redirect for failed requests.
  OnBeforeURLRequest_SnsRedirectWork(
      base::DoNothing(), hns_request_info, {},
      hns_wallet::mojom::SolanaProviderError::kInternalError, "todo");
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  OnBeforeURLRequest_SnsRedirectWork(
      base::DoNothing(), hns_request_info, {},
      hns_wallet::mojom::SolanaProviderError::kSuccess, "");
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  // No redirect for invalid url.
  OnBeforeURLRequest_SnsRedirectWork(
      base::DoNothing(), hns_request_info, GURL("invalid"),
      hns_wallet::mojom::SolanaProviderError::kSuccess, "");
  EXPECT_TRUE(hns_request_info->new_url_spec.empty());

  // Redirect for valid url.
  OnBeforeURLRequest_SnsRedirectWork(
      base::DoNothing(), hns_request_info, GURL("https://hns.com"),
      hns_wallet::mojom::SolanaProviderError::kSuccess, "");
  EXPECT_EQ(hns_request_info->new_url_spec, GURL("https://hns.com"));

  EXPECT_FALSE(hns_request_info->pending_error.has_value());
}

}  // namespace decentralized_dns
