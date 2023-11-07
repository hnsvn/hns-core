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
#include "components/prefs/pref_service.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"

using hns_shields::ControlType;

const char kEmbeddedTestServerDirectory[] = "webaudio";
const char kTitleScript[] = "document.title;";

class HnsWebAudioFarblingBrowserTest : public InProcessBrowserTest {
 public:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();

    content_client_ = std::make_unique<ChromeContentClient>();
    content::SetContentClient(content_client_.get());
    browser_content_client_ = std::make_unique<HnsContentBrowserClient>();
    content::SetBrowserClientForTesting(browser_content_client_.get());

    host_resolver()->AddRule("*", "127.0.0.1");
    content::SetupCrossSiteRedirector(embedded_test_server());

    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    test_data_dir = test_data_dir.AppendASCII(kEmbeddedTestServerDirectory);
    embedded_test_server()->ServeFilesFromDirectory(test_data_dir);

    ASSERT_TRUE(embedded_test_server()->Start());

    top_level_page_url_ = embedded_test_server()->GetURL("a.com", "/");
    farbling_url_ = embedded_test_server()->GetURL("a.com", "/farbling.html");
    copy_from_channel_url_ =
        embedded_test_server()->GetURL("a.com", "/copyFromChannel.html");
  }

  void TearDown() override {
    browser_content_client_.reset();
    content_client_.reset();
  }

  const GURL& copy_from_channel_url() { return copy_from_channel_url_; }

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
  GURL copy_from_channel_url_;
  GURL farbling_url_;
  std::unique_ptr<ChromeContentClient> content_client_;
  std::unique_ptr<HnsContentBrowserClient> browser_content_client_;
};

// Tests for crash in copyFromChannel as reported in
// https://github.com/hnsvn/hns-browser/issues/9552
// No crash indicates a successful test.
IN_PROC_BROWSER_TEST_F(HnsWebAudioFarblingBrowserTest,
                       CopyFromChannelNoCrash) {
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), copy_from_channel_url()));
}

// Tests results of farbling known values
IN_PROC_BROWSER_TEST_F(HnsWebAudioFarblingBrowserTest, FarbleWebAudio) {
  // Farbling level: maximum
  // web audio: pseudo-random data with no relation to underlying audio channel
  BlockFingerprinting();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), farbling_url()));
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript), "405");
  // second time, same as the first (tests that the PRNG properly resets itself
  // at the beginning of each calculation)
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), farbling_url()));
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript), "405");

  // Farbling level: balanced (default)
  // web audio: farbled audio data
  SetFingerprintingDefault();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), farbling_url()));
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript), "7968");

  // Farbling level: off
  // web audio: original audio data
  AllowFingerprinting();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), farbling_url()));
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript), "8000");
}
