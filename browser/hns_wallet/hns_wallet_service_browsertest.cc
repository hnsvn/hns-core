/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/memory/raw_ptr.h"
#include "base/path_service.h"
#include "base/test/scoped_feature_list.h"
#include "hns/browser/hns_wallet/hns_wallet_service_factory.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service.h"
#include "hns/components/hns_wallet/browser/hns_wallet_service_observer_base.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/hns_wallet/common/features.h"
#include "hns/components/constants/hns_paths.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "net/dns/mock_host_resolver.h"
#include "url/origin.h"

namespace hns_wallet {

namespace {

void OnGetActiveOrigin(bool* callback_called,
                       mojom::OriginInfoPtr expected_active_origin,
                       mojom::OriginInfoPtr active_origin) {
  EXPECT_EQ(expected_active_origin, active_origin);
  *callback_called = true;
}

}  // namespace

class TestHnsWalletServiceObserver
    : public hns_wallet::HnsWalletServiceObserverBase {
 public:
  TestHnsWalletServiceObserver() = default;

  void OnActiveOriginChanged(mojom::OriginInfoPtr origin_info) override {
    active_origin_info_ = origin_info->Clone();
  }

  const mojom::OriginInfoPtr& active_origin_info() const {
    return active_origin_info_;
  }

  mojo::PendingRemote<hns_wallet::mojom::HnsWalletServiceObserver>
  GetReceiver() {
    return observer_receiver_.BindNewPipeAndPassRemote();
  }

  void Reset() { active_origin_info_ = {}; }

 private:
  mojom::OriginInfoPtr active_origin_info_;
  mojo::Receiver<hns_wallet::mojom::HnsWalletServiceObserver>
      observer_receiver_{this};
};

class HnsWalletServiceTest : public InProcessBrowserTest {
 public:
  HnsWalletServiceTest()
      : https_server_(net::EmbeddedTestServer::TYPE_HTTPS) {
    feature_list_.InitAndEnableFeature(
        hns_wallet::features::kNativeHnsWalletFeature);

    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    https_server_.SetSSLConfig(net::EmbeddedTestServer::CERT_TEST_NAMES);
    https_server_.ServeFilesFromDirectory(test_data_dir);
    EXPECT_TRUE(https_server_.Start());
  }

  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();
    host_resolver()->AddRule("*", "127.0.0.1");
    wallet_service_ = hns_wallet::HnsWalletServiceFactory::GetInstance()
                          ->GetServiceForContext(browser()->profile());
  }

  HnsWalletService* wallet_service() { return wallet_service_; }
  const net::EmbeddedTestServer* https_server() const { return &https_server_; }

 private:
  raw_ptr<HnsWalletService> wallet_service_ = nullptr;
  net::EmbeddedTestServer https_server_;
  base::test::ScopedFeatureList feature_list_;
};

IN_PROC_BROWSER_TEST_F(HnsWalletServiceTest, ActiveOrigin) {
  GURL url = https_server()->GetURL("a.test", "/simple.html");
  auto expected_origin_info = MakeOriginInfo(url::Origin::Create(url));
  TestHnsWalletServiceObserver observer;
  wallet_service()->AddObserver(observer.GetReceiver());
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));

  bool callback_called = false;
  wallet_service()->GetActiveOrigin(base::BindOnce(
      &OnGetActiveOrigin, &callback_called, expected_origin_info->Clone()));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_called);
  EXPECT_EQ(observer.active_origin_info(), expected_origin_info);

  url = https_server()->GetURL("b.test", "/simple.html");
  expected_origin_info = MakeOriginInfo(url::Origin::Create(url));
  callback_called = false;
  observer.Reset();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  wallet_service()->GetActiveOrigin(base::BindOnce(
      &OnGetActiveOrigin, &callback_called, expected_origin_info->Clone()));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_called);
  EXPECT_EQ(observer.active_origin_info(), expected_origin_info);

  url = https_server()->GetURL("c.test", "/simple.html");
  expected_origin_info = MakeOriginInfo(url::Origin::Create(url));
  observer.Reset();
  ui_test_utils::NavigateToURLWithDisposition(
      browser(), url, WindowOpenDisposition::NEW_FOREGROUND_TAB,
      ui_test_utils::BROWSER_TEST_WAIT_FOR_LOAD_STOP);
  wallet_service()->GetActiveOrigin(base::BindOnce(
      &OnGetActiveOrigin, &callback_called, expected_origin_info->Clone()));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_called);
  EXPECT_EQ(observer.active_origin_info(), expected_origin_info);

  url = https_server()->GetURL("d.test", "/simple.html");
  expected_origin_info = MakeOriginInfo(url::Origin::Create(url));
  observer.Reset();
  ui_test_utils::NavigateToURLWithDisposition(
      browser(), url, WindowOpenDisposition::NEW_WINDOW,
      ui_test_utils::BROWSER_TEST_WAIT_FOR_LOAD_STOP);
  wallet_service()->GetActiveOrigin(base::BindOnce(
      &OnGetActiveOrigin, &callback_called, expected_origin_info->Clone()));
  base::RunLoop().RunUntilIdle();
  EXPECT_TRUE(callback_called);
  EXPECT_EQ(observer.active_origin_info(), expected_origin_info);
}

}  // namespace hns_wallet
