/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/path_service.h"
#include "base/test/bind.h"
#include "base/test/scoped_feature_list.h"
#include "base/test/thread_test_helper.h"
#include "hns/browser/hns_wallet/json_rpc_service_factory.h"
#include "hns/browser/hns_wallet/keyring_service_factory.h"
#include "hns/components/hns_wallet/browser/hns_wallet_constants.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/json_rpc_service.h"
#include "hns/components/hns_wallet/browser/keyring_service.h"
#include "hns/components/hns_wallet/browser/permission_utils.h"
#include "hns/components/hns_wallet/common/common_utils.h"
#include "hns/components/hns_wallet/common/features.h"
#include "hns/components/constants/hns_paths.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/permissions/contexts/hns_wallet_permission_context.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "components/network_session_configurator/common/network_switches.h"
#include "content/public/browser/browser_task_traits.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "content/public/test/content_mock_cert_verifier.h"
#include "net/dns/mock_host_resolver.h"
#include "url/gurl.h"
#include "url/origin.h"

namespace {

const char kEmbeddedTestServerDirectory[] = "hns-wallet";

std::string CheckForEventScript(const std::string& event_var) {
  return base::StringPrintf(R"(
      new Promise(resolve => {
        const timer = setInterval(function () {
          if (%s) {
            clearInterval(timer);
            resolve(true);
          }
        }, 100);
      });
    )",
                            event_var.c_str());
}

}  // namespace

namespace hns_wallet {

class HnsWalletEventEmitterTest : public InProcessBrowserTest {
 public:
  HnsWalletEventEmitterTest() {
    feature_list_.InitAndEnableFeature(
        hns_wallet::features::kNativeHnsWalletFeature);
  }

  void SetUpInProcessBrowserTestFixture() override {
    InProcessBrowserTest::SetUpInProcessBrowserTestFixture();
    mock_cert_verifier_.SetUpInProcessBrowserTestFixture();
  }

  void TearDownInProcessBrowserTestFixture() override {
    InProcessBrowserTest::TearDownInProcessBrowserTestFixture();
    mock_cert_verifier_.TearDownInProcessBrowserTestFixture();
  }

  void SetUpOnMainThread() override {
    hns_wallet::SetDefaultEthereumWallet(
        browser()->profile()->GetPrefs(),
        hns_wallet::mojom::DefaultWallet::HnsWallet);
    InProcessBrowserTest::SetUpOnMainThread();
    mock_cert_verifier_.mock_cert_verifier()->set_default_result(net::OK);
    host_resolver()->AddRule("*", "127.0.0.1");

    https_server_ = std::make_unique<net::EmbeddedTestServer>(
        net::test_server::EmbeddedTestServer::TYPE_HTTPS);
    https_server_->SetSSLConfig(net::EmbeddedTestServer::CERT_OK);

    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    test_data_dir = test_data_dir.AppendASCII(kEmbeddedTestServerDirectory);
    https_server_->ServeFilesFromDirectory(test_data_dir);

    keyring_service_ =
        KeyringServiceFactory::GetServiceForContext(browser()->profile());

    ASSERT_TRUE(https_server_->Start());
  }

  void SetUpCommandLine(base::CommandLine* command_line) override {
    InProcessBrowserTest::SetUpCommandLine(command_line);
    mock_cert_verifier_.SetUpCommandLine(command_line);
  }

  net::EmbeddedTestServer* https_server() { return https_server_.get(); }

  mojo::Remote<hns_wallet::mojom::JsonRpcService> GetJsonRpcService() {
    if (!json_rpc_service_) {
      auto pending =
          hns_wallet::JsonRpcServiceFactory::GetInstance()->GetForContext(
              browser()->profile());
      json_rpc_service_.Bind(std::move(pending));
    }
    return std::move(json_rpc_service_);
  }

  HostContentSettingsMap* host_content_settings_map() {
    return HostContentSettingsMapFactory::GetForProfile(browser()->profile());
  }

  content::WebContents* web_contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  url::Origin GetLastCommitedOrigin() {
    return url::Origin::Create(web_contents()->GetLastCommittedURL());
  }

  void RestoreWallet() {
    const char mnemonic[] =
        "drip caution abandon festival order clown oven regular absorb "
        "evidence crew where";
    base::RunLoop run_loop;
    keyring_service_->RestoreWallet(
        mnemonic, "hns123", false,
        base::BindLambdaForTesting([&](bool success) {
          ASSERT_TRUE(success);
          run_loop.Quit();
        }));
    run_loop.Run();
  }

  void GetAddress(std::string* valid_address) {
    ASSERT_NE(valid_address, nullptr);

    base::RunLoop run_loop;
    keyring_service_->GetKeyringInfo(
        hns_wallet::mojom::kDefaultKeyringId,
        base::BindLambdaForTesting(
            [&](hns_wallet::mojom::KeyringInfoPtr keyring_info) {
              *valid_address = "";
              if (keyring_info->account_infos.size() > 0) {
                *valid_address = keyring_info->account_infos[0]->address;
              }
              run_loop.Quit();
            }));
    run_loop.Run();
  }

  void SetSelectedAccount(const std::string& address) {
    base::RunLoop run_loop;
    keyring_service_->SetSelectedAccount(
        MakeAccountId(mojom::CoinType::ETH,
                      hns_wallet::mojom::kDefaultKeyringId,
                      mojom::AccountKind::kDerived, address),
        base::BindLambdaForTesting([&](bool success) {
          ASSERT_TRUE(success);
          run_loop.Quit();
        }));
    run_loop.Run();
  }

 private:
  content::ContentMockCertVerifier mock_cert_verifier_;
  mojo::Remote<hns_wallet::mojom::JsonRpcService> json_rpc_service_;
  raw_ptr<KeyringService> keyring_service_ = nullptr;
  std::unique_ptr<net::EmbeddedTestServer> https_server_;
  base::test::ScopedFeatureList feature_list_;
};

IN_PROC_BROWSER_TEST_F(HnsWalletEventEmitterTest, CheckForAConnectEvent) {
  GURL url =
      https_server()->GetURL("a.com", "/hns_wallet_event_emitter.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();

  auto result_first =
      EvalJs(contents, CheckForEventScript("received_connect_event"));
  EXPECT_EQ(base::Value(true), result_first.value);
}

IN_PROC_BROWSER_TEST_F(HnsWalletEventEmitterTest,
                       CheckForAChainChangedEvent) {
  GURL url =
      https_server()->GetURL("a.com", "/hns_wallet_event_emitter.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  auto service = GetJsonRpcService();
  service->SetNetwork(hns_wallet::mojom::kGoerliChainId,
                      hns_wallet::mojom::CoinType::ETH, absl::nullopt,
                      base::DoNothing());

  auto result_first =
      EvalJs(contents, CheckForEventScript("received_chain_changed_event"));
  EXPECT_EQ(base::Value(true), result_first.value);
}

IN_PROC_BROWSER_TEST_F(HnsWalletEventEmitterTest,
                       CheckForAnAccountChangedEvent) {
  RestoreWallet();
  GURL url =
      https_server()->GetURL("a.com", "/hns_wallet_event_emitter.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  std::string address;
  GetAddress(&address);

  url::Origin sub_request_origin;
  ASSERT_TRUE(hns_wallet::GetSubRequestOrigin(
      permissions::RequestType::kHnsEthereum, GetLastCommitedOrigin(),
      address, &sub_request_origin));
  host_content_settings_map()->SetContentSettingDefaultScope(
      sub_request_origin.GetURL(), GetLastCommitedOrigin().GetURL(),
      ContentSettingsType::HNS_ETHEREUM,
      ContentSetting::CONTENT_SETTING_ALLOW);
  SetSelectedAccount(address);

  auto result_first =
      EvalJs(contents, CheckForEventScript("received_account_changed_event"));
  EXPECT_EQ(base::Value(true), result_first.value);
}

}  // namespace hns_wallet
