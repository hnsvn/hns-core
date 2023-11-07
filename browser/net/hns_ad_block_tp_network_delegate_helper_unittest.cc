/* Copyright (c) 2019 The Hns Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/net/hns_ad_block_tp_network_delegate_helper.h"

#include <memory>
#include <string>
#include <utility>

#include "base/memory/raw_ptr.h"
#include "base/path_service.h"
#include "base/task/single_thread_task_runner.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/net/url_context.h"
#include "hns/components/hns_component_updater/browser/hns_component.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/hns_shields/browser/ad_block_subscription_download_manager.h"
#include "hns/components/hns_shields/browser/test_filters_provider.h"
#include "hns/test/base/testing_hns_browser_process.h"
#include "chrome/browser/net/system_network_context_manager.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/test/base/scoped_testing_local_state.h"
#include "chrome/test/base/testing_browser_process.h"
#include "content/public/test/browser_task_environment.h"
#include "net/base/net_errors.h"
#include "net/dns/mock_host_resolver.h"
#include "net/log/net_log.h"
#include "services/network/host_resolver.h"
#include "testing/gtest/include/gtest/gtest.h"

using hns::ResponseCallback;
using hns_component_updater::HnsComponent;
using hns_shields::TestFiltersProvider;

namespace {

// Note: extract a common impl if needed for other tests, do not copy.

// TODO(iefremov): This is only needed to provide a task runner to the adblock
// service. We can drop this stub once the service doesn't need an
// "external" runner.
class TestingHnsComponentUpdaterDelegate : public HnsComponent::Delegate {
 public:
  explicit TestingHnsComponentUpdaterDelegate(PrefService* local_state)
      : local_state_(local_state) {}
  ~TestingHnsComponentUpdaterDelegate() override = default;

  TestingHnsComponentUpdaterDelegate(TestingHnsComponentUpdaterDelegate&) =
      delete;
  TestingHnsComponentUpdaterDelegate& operator=(
      TestingHnsComponentUpdaterDelegate&) = delete;

  using ComponentObserver = update_client::UpdateClient::Observer;

  // hns_component_updater::HnsComponent::Delegate implementation
  void Register(const std::string& component_name,
                const std::string& component_base64_public_key,
                base::OnceClosure registered_callback,
                HnsComponent::ReadyCallback ready_callback) override {}
  bool Unregister(const std::string& component_id) override { return true; }
  void OnDemandUpdate(const std::string& component_id) override {}

  void AddObserver(ComponentObserver* observer) override {}
  void RemoveObserver(ComponentObserver* observer) override {}

  scoped_refptr<base::SequencedTaskRunner> GetTaskRunner() override {
    return base::SingleThreadTaskRunner::GetCurrentDefault();
  }

  const std::string& locale() const override { return locale_; }
  PrefService* local_state() override { return local_state_; }

 private:
  raw_ptr<PrefService> local_state_ = nullptr;
  const std::string locale_ = "en";
};

}  // namespace

void FakeAdBlockSubscriptionDownloadManagerGetter(
    base::OnceCallback<
        void(hns_shields::AdBlockSubscriptionDownloadManager*)>) {
  // no-op, subscription services are not currently used in unit tests
}

class HnsAdBlockTPNetworkDelegateHelperTest : public testing::Test {
 protected:
  void SetUp() override {
    local_state_ = std::make_unique<ScopedTestingLocalState>(
        TestingBrowserProcess::GetGlobal());

    hns_component_updater_delegate_ =
        std::make_unique<TestingHnsComponentUpdaterDelegate>(
            local_state_->Get());

    base::FilePath user_data_dir;
    DCHECK(base::PathService::Get(chrome::DIR_USER_DATA, &user_data_dir));
    auto adblock_service = std::make_unique<hns_shields::AdBlockService>(
        hns_component_updater_delegate_->local_state(),
        hns_component_updater_delegate_->locale(), nullptr,
        hns_component_updater_delegate_->GetTaskRunner(),
        base::BindOnce(&FakeAdBlockSubscriptionDownloadManagerGetter),
        user_data_dir);

    TestingHnsBrowserProcess::GetGlobal()->SetAdBlockService(
        std::move(adblock_service));

    host_resolver_ = std::make_unique<net::MockHostResolver>();
    resolver_wrapper_ = std::make_unique<network::HostResolver>(
        host_resolver_.get(), net::NetLog::Get());
    hns::SetAdblockCnameHostResolverForTesting(resolver_wrapper_.get());

    stub_resolver_config_reader_ =
        std::make_unique<StubResolverConfigReader>(local_state_->Get());
    SystemNetworkContextManager::set_stub_resolver_config_reader_for_testing(
        stub_resolver_config_reader_.get());
  }

  void TearDown() override {
    // The AdBlockBaseService destructor must be called before the task runner
    // is destroyed.
    TestingHnsBrowserProcess::DeleteInstance();
  }

  void ResetAdblockInstance(std::string rules, std::string resources) {
    filters_provider_ = std::make_unique<TestFiltersProvider>(rules, resources);
    g_hns_browser_process->ad_block_service()->UseSourceProvidersForTest(
        filters_provider_.get(), filters_provider_.get());
  }

  // Returns true if the request handler deferred control back to the calling
  // thread before completion, or true if it completed instantly.
  bool CheckRequest(std::shared_ptr<hns::HnsRequestInfo> request_info) {
    // `request_identifier` must be nonzero, or else nothing will be tested.
    request_info->request_identifier = 1;

    int rc =
        OnBeforeURLRequest_AdBlockTPPreWork(base::DoNothing(), request_info);
    EXPECT_TRUE(rc == net::OK || rc == net::ERR_IO_PENDING);
    task_environment_.RunUntilIdle();

    return rc == net::ERR_IO_PENDING;
  }

  std::unique_ptr<ScopedTestingLocalState> local_state_;

  std::unique_ptr<TestingHnsComponentUpdaterDelegate>
      hns_component_updater_delegate_;

  content::BrowserTaskEnvironment task_environment_;

  std::unique_ptr<net::MockHostResolver> host_resolver_;

  std::unique_ptr<StubResolverConfigReader> stub_resolver_config_reader_;

  std::unique_ptr<TestFiltersProvider> filters_provider_;

 private:
  std::unique_ptr<network::HostResolver> resolver_wrapper_;
};

TEST_F(HnsAdBlockTPNetworkDelegateHelperTest, NoInitiatorURL) {
  const GURL url("https://bradhatesprimes.hns.com/composite_numbers_ftw");
  auto request_info = std::make_shared<hns::HnsRequestInfo>(url);
  request_info->resource_type = blink::mojom::ResourceType::kScript;

  EXPECT_FALSE(CheckRequest(request_info));
  EXPECT_EQ(request_info->blocked_by, hns::kNotBlocked);
  EXPECT_TRUE(request_info->new_url_spec.empty());
}

TEST_F(HnsAdBlockTPNetworkDelegateHelperTest, EmptyRequestURL) {
  auto request_info = std::make_shared<hns::HnsRequestInfo>(GURL());
  request_info->initiator_url = GURL("https://example.com");
  request_info->resource_type = blink::mojom::ResourceType::kScript;

  EXPECT_FALSE(CheckRequest(request_info));
  EXPECT_EQ(request_info->blocked_by, hns::kNotBlocked);
  EXPECT_TRUE(request_info->new_url_spec.empty());
}

TEST_F(HnsAdBlockTPNetworkDelegateHelperTest, DevToolURL) {
  const GURL url("devtools://devtools/");
  auto request_info = std::make_shared<hns::HnsRequestInfo>(url);
  request_info->initiator_url =
      GURL("devtools://devtools/bundled/root/root.js");
  request_info->resource_type = blink::mojom::ResourceType::kScript;

  EXPECT_FALSE(CheckRequest(request_info));
  EXPECT_EQ(request_info->blocked_by, hns::kNotBlocked);
  EXPECT_TRUE(request_info->new_url_spec.empty());
}

TEST_F(HnsAdBlockTPNetworkDelegateHelperTest, RequestDataURL) {
  const GURL url(
      "data:image/gif;base64,R0lGODlhAQABAIAAAP///"
      "wAAACH5BAEAAAAALAAAAAABAAEAAAICRAEAOw==");
  auto request_info = std::make_shared<hns::HnsRequestInfo>(url);
  request_info->initiator_url = GURL("https://example.com");
  request_info->resource_type = blink::mojom::ResourceType::kImage;

  EXPECT_FALSE(CheckRequest(request_info));
  EXPECT_EQ(request_info->blocked_by, hns::kNotBlocked);
  EXPECT_TRUE(request_info->new_url_spec.empty());
}

TEST_F(HnsAdBlockTPNetworkDelegateHelperTest, SimpleBlocking) {
  ResetAdblockInstance("||hns.com/test.txt", "");

  const GURL url("https://hns.com/test.txt");
  auto request_info = std::make_shared<hns::HnsRequestInfo>(url);
  request_info->request_identifier = 1;
  request_info->resource_type = blink::mojom::ResourceType::kScript;
  request_info->initiator_url = GURL("https://hnssoftware.com");

  EXPECT_TRUE(CheckRequest(request_info));
  EXPECT_EQ(request_info->blocked_by, hns::kAdBlocked);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  // It's unclear whether or not this is a Tor request, so no DNS queries are
  // made (`browser_context` is `nullptr`).
  EXPECT_EQ(0ULL, host_resolver_->num_resolve());
}

TEST_F(HnsAdBlockTPNetworkDelegateHelperTest, Default1pException) {
  ResetAdblockInstance("||hns.com/test.txt", "");

  const GURL url("https://hns.com/test.txt");
  auto request_info = std::make_shared<hns::HnsRequestInfo>(url);
  request_info->request_identifier = 1;
  request_info->resource_type = blink::mojom::ResourceType::kScript;
  request_info->initiator_url = GURL("https://hns.com");

  EXPECT_TRUE(CheckRequest(request_info));
  EXPECT_EQ(request_info->blocked_by, hns::kNotBlocked);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  EXPECT_EQ(0ULL, host_resolver_->num_resolve());
}

TEST_F(HnsAdBlockTPNetworkDelegateHelperTest, AggressiveNo1pException) {
  ResetAdblockInstance("||hns.com/test.txt", "");

  const GURL url("https://hns.com/test.txt");
  auto request_info = std::make_shared<hns::HnsRequestInfo>(url);
  request_info->request_identifier = 1;
  request_info->resource_type = blink::mojom::ResourceType::kScript;
  request_info->initiator_url = GURL("https://hns.com");
  request_info->aggressive_blocking = true;

  EXPECT_TRUE(CheckRequest(request_info));
  EXPECT_EQ(request_info->blocked_by, hns::kAdBlocked);
  EXPECT_TRUE(request_info->new_url_spec.empty());
  // It's unclear whether or not this is a Tor request, so no DNS queries are
  // made (`browser_context` is `nullptr`).
  EXPECT_EQ(0ULL, host_resolver_->num_resolve());
}
