/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <memory>

#include "base/path_service.h"
#include "base/strings/stringprintf.h"
#include "base/test/thread_test_helper.h"
#include "hns/browser/hns_content_browser_client.h"
#include "hns/browser/extensions/hns_base_local_data_files_browsertest.h"
#include "hns/components/hns_component_updater/browser/local_data_files_service.h"
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "hns/components/constants/hns_paths.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/extensions/extension_browsertest.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/chrome_content_client.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/network_session_configurator/common/network_switches.h"
#include "components/permissions/permission_request.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"
#include "third_party/blink/public/common/device_memory/approximated_device_memory.h"

using hns_shields::ControlType;

const char kDeviceMemoryScript[] = "navigator.deviceMemory * 1024";

class HnsDeviceMemoryFarblingBrowserTest : public InProcessBrowserTest {
 public:
  HnsDeviceMemoryFarblingBrowserTest()
      : https_server_(net::EmbeddedTestServer::TYPE_HTTPS) {
    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    https_server_.SetSSLConfig(net::EmbeddedTestServer::CERT_TEST_NAMES);
    https_server_.ServeFilesFromDirectory(test_data_dir);
    EXPECT_TRUE(https_server_.Start());
  }

  HnsDeviceMemoryFarblingBrowserTest(
      const HnsDeviceMemoryFarblingBrowserTest&) = delete;
  HnsDeviceMemoryFarblingBrowserTest& operator=(
      const HnsDeviceMemoryFarblingBrowserTest&) = delete;

  ~HnsDeviceMemoryFarblingBrowserTest() override = default;

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
  base::test::ScopedFeatureList feature_list_;
  net::EmbeddedTestServer https_server_;

  HostContentSettingsMap* content_settings() {
    return HostContentSettingsMapFactory::GetForProfile(browser()->profile());
  }

  void AllowFingerprinting(std::string domain) {
    hns_shields::SetFingerprintingControlType(
        content_settings(), ControlType::ALLOW,
        https_server_.GetURL(domain, "/"));
  }

  void BlockFingerprinting(std::string domain) {
    hns_shields::SetFingerprintingControlType(
        content_settings(), ControlType::BLOCK,
        https_server_.GetURL(domain, "/"));
  }

  void SetFingerprintingDefault(std::string domain) {
    hns_shields::SetFingerprintingControlType(
        content_settings(), ControlType::DEFAULT,
        https_server_.GetURL(domain, "/"));
  }

  content::WebContents* contents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

 private:
  std::unique_ptr<ChromeContentClient> content_client_;
  std::unique_ptr<HnsContentBrowserClient> browser_content_client_;
};

// Tests results of farbling known values
IN_PROC_BROWSER_TEST_F(HnsDeviceMemoryFarblingBrowserTest,
                       FarbleDeviceMemory) {
  std::string domain1 = "b.test";
  std::string domain2 = "d.test";
  GURL url1 = https_server_.GetURL(domain1, "/simple.html");
  GURL url2 = https_server_.GetURL(domain2, "/simple.html");
  // set memory to 10GB
  blink::ApproximatedDeviceMemory::SetPhysicalMemoryMBForTesting(1024 * 10);
  int true_value =
      blink::ApproximatedDeviceMemory::GetApproximatedDeviceMemory() * 1024;
  EXPECT_EQ(true_value, 8192);
  // Farbling level: off
  AllowFingerprinting(domain1);
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url1));
  EXPECT_EQ(true_value, EvalJs(contents(), kDeviceMemoryScript));
  AllowFingerprinting(domain2);
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url2));
  EXPECT_EQ(true_value, EvalJs(contents(), kDeviceMemoryScript));

  // Farbling level: default
  SetFingerprintingDefault(domain1);
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url1));
  EXPECT_EQ(2048, EvalJs(contents(), kDeviceMemoryScript));
  SetFingerprintingDefault(domain2);
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url2));
  EXPECT_EQ(4096, EvalJs(contents(), kDeviceMemoryScript));

  // Farbling level: maximum
  BlockFingerprinting(domain1);
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url1));
  EXPECT_EQ(512, EvalJs(contents(), kDeviceMemoryScript));
  AllowFingerprinting(domain2);
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url2));
  EXPECT_EQ(8192, EvalJs(contents(), kDeviceMemoryScript));
}
