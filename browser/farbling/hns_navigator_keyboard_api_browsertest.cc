/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/containers/contains.h"
#include "base/path_service.h"
#include "hns/browser/hns_content_browser_client.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "hns/components/constants/hns_paths.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/chrome_content_client.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"
#include "url/gurl.h"

using hns_shields::ControlType;

namespace {

constexpr char kGetLayoutMapScript[] =
    R"(navigator.keyboard.getLayoutMap instanceof Function)";

}  // namespace

class HnsNavigatorKeyboardAPIBrowserTest : public InProcessBrowserTest {
 public:
  HnsNavigatorKeyboardAPIBrowserTest()
      : https_server_(net::EmbeddedTestServer::TYPE_HTTPS) {
    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    https_server_.SetSSLConfig(net::EmbeddedTestServer::CERT_TEST_NAMES);
    https_server_.ServeFilesFromDirectory(test_data_dir);
    EXPECT_TRUE(https_server_.Start());
    top_level_page_url_ = https_server_.GetURL("a.test", "/");
    test_url_ = https_server_.GetURL("a.test", "/simple.html");
  }

  HnsNavigatorKeyboardAPIBrowserTest(
      const HnsNavigatorKeyboardAPIBrowserTest&) = delete;
  HnsNavigatorKeyboardAPIBrowserTest& operator=(
      const HnsNavigatorKeyboardAPIBrowserTest&) = delete;

  ~HnsNavigatorKeyboardAPIBrowserTest() override = default;

  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();

    content_client_ = std::make_unique<ChromeContentClient>();
    content::SetContentClient(content_client_.get());
    browser_content_client_ = std::make_unique<HnsContentBrowserClient>();
    content::SetBrowserClientForTesting(browser_content_client_.get());

    host_resolver()->AddRule("*", "127.0.0.1");
  }

  void TearDown() override {
    browser_content_client_.reset();
    content_client_.reset();
  }

 protected:
  net::EmbeddedTestServer https_server_;

  const GURL& test_url() { return test_url_; }

  HostContentSettingsMap* content_settings() {
    return HostContentSettingsMapFactory::GetForProfile(browser()->profile());
  }

  void AllowFingerprinting() {
    hns_shields::SetFingerprintingControlType(
        content_settings(), ControlType::ALLOW, top_level_page_url_);
  }

  void BlockFingerprinting() {
    hns_shields::SetFingerprintingControlType(
        content_settings(), ControlType::BLOCK, top_level_page_url_);
  }

  void SetFingerprintingDefault() {
    hns_shields::SetFingerprintingControlType(
        content_settings(), ControlType::DEFAULT, top_level_page_url_);
  }

  content::WebContents* contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

 private:
  GURL top_level_page_url_;
  GURL test_url_;
  std::unique_ptr<ChromeContentClient> content_client_;
  std::unique_ptr<HnsContentBrowserClient> browser_content_client_;
};

IN_PROC_BROWSER_TEST_F(HnsNavigatorKeyboardAPIBrowserTest,
                       TestKeyboardAPIAvilability) {
  // Fingerprinting level: off
  // get real navigator.keyboard.getLayoutMap key
  AllowFingerprinting();
  EXPECT_TRUE(ui_test_utils::NavigateToURL(browser(), test_url()));
  EXPECT_EQ(true, EvalJs(contents(), kGetLayoutMapScript));

  // Fingerprinting level: standard (default)
  // navigator.keyboard will be null
  SetFingerprintingDefault();
  EXPECT_TRUE(ui_test_utils::NavigateToURL(browser(), test_url()));
  auto result_standard = EvalJs(contents(), kGetLayoutMapScript);
  EXPECT_TRUE(base::Contains(
      result_standard.error,
      "Cannot read properties of null (reading 'getLayoutMap')"));

  // Fingerprinting level: blocked (same as standard for this test)
  BlockFingerprinting();
  EXPECT_TRUE(ui_test_utils::NavigateToURL(browser(), test_url()));
  auto result_blocked = EvalJs(contents(), kGetLayoutMapScript);
  EXPECT_TRUE(base::Contains(
      result_blocked.error,
      "Cannot read properties of null (reading 'getLayoutMap')"));
}
