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
#include "url/gurl.h"

using hns_shields::ControlType;

const char kEnumerateDevicesScript[] =
    "navigator.mediaDevices.enumerateDevices()"
    ".then(function(devices) {"
    "  var devicekinds = '';"
    "  devices.forEach(function(device) {"
    "    devicekinds = devicekinds + device.kind + '|';"
    "  });"
    "  return devicekinds;"
    "})";

class HnsEnumerateDevicesFarblingBrowserTest : public InProcessBrowserTest {
 public:
  HnsEnumerateDevicesFarblingBrowserTest()
      : https_server_(net::EmbeddedTestServer::TYPE_HTTPS) {
    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    https_server_.SetSSLConfig(net::EmbeddedTestServer::CERT_TEST_NAMES);
    https_server_.ServeFilesFromDirectory(test_data_dir);
    EXPECT_TRUE(https_server_.Start());
    top_level_page_url_ = https_server_.GetURL("b.test", "/");
    farbling_url_ = https_server_.GetURL("b.test", "/simple.html");
  }

  HnsEnumerateDevicesFarblingBrowserTest(
      const HnsEnumerateDevicesFarblingBrowserTest&) = delete;
  HnsEnumerateDevicesFarblingBrowserTest& operator=(
      const HnsEnumerateDevicesFarblingBrowserTest&) = delete;

  ~HnsEnumerateDevicesFarblingBrowserTest() override = default;

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

  const GURL& farbling_url() { return farbling_url_; }

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
  GURL farbling_url_;
  std::unique_ptr<ChromeContentClient> content_client_;
  std::unique_ptr<HnsContentBrowserClient> browser_content_client_;
};

// Tests results of farbling known values
IN_PROC_BROWSER_TEST_F(HnsEnumerateDevicesFarblingBrowserTest,
                       FarbleEnumerateDevices) {
  // Farbling level: off
  // get real navigator.mediaDevices.enumerateDevices array
  AllowFingerprinting();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), farbling_url()));
  std::string real_value =
      content::EvalJs(contents(), kEnumerateDevicesScript).ExtractString();
  ASSERT_NE(real_value, "");

  // Farbling level: balanced (default)
  // navigator.mediaDevices.enumerateDevices array is shuffled
  // pseudo-randomly based on domain+session key
  SetFingerprintingDefault();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), farbling_url()));
  std::string balanced_value =
      content::EvalJs(contents(), kEnumerateDevicesScript).ExtractString();
  EXPECT_NE(balanced_value, real_value);

  // Farbling level: maximum
  // same as farbling level: balanced
  BlockFingerprinting();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), farbling_url()));
  std::string maximum_value =
      content::EvalJs(contents(), kEnumerateDevicesScript).ExtractString();
  EXPECT_EQ(balanced_value, maximum_value);
}