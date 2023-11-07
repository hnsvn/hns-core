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
#include "hns/components/hns_shields/browser/hns_shields_util.h"
#include "hns/components/constants/hns_paths.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/extensions/extension_browsertest.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/common/chrome_content_client.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/permissions/permission_request.h"
#include "components/prefs/pref_service.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "net/dns/mock_host_resolver.h"

using hns_shields::ControlType;

const char kEmbeddedTestServerDirectory[] = "canvas";
const char kTitleScript[] = "document.title;";
const char kExpectedImageDataHashFarblingBalanced[] = "204";
const char kExpectedImageDataHashFarblingOff[] = "0";
const char kExpectedImageDataHashFarblingMaximum[] = "204";

class HnsOffscreenCanvasFarblingBrowserTest : public InProcessBrowserTest {
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
  }

  void TearDown() override {
    browser_content_client_.reset();
    content_client_.reset();
  }

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
  std::unique_ptr<ChromeContentClient> content_client_;
  std::unique_ptr<HnsContentBrowserClient> browser_content_client_;
};

IN_PROC_BROWSER_TEST_F(HnsOffscreenCanvasFarblingBrowserTest,
                       MustNotTimeout) {
  GURL url =
      embedded_test_server()->GetURL("a.com", "/offscreen-farbling.html");
  AllowFingerprinting();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  // NavigateToURL will return before our Worker has a chance
  // to run its code to completion, so we block here until document.title
  // changes. This will happen relatively quickly if things are going well
  // inside the Worker. If the browser crashes while executing the Worker
  // code (which is what this test is really testing), then this will never
  // unblock and the entire browser test will eventually time out. Timing
  // out indicates a fatal error.
  while (content::EvalJs(contents(), kTitleScript).ExtractString() == "") {
  }
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript), "pass");

  BlockFingerprinting();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  while (content::EvalJs(contents(), kTitleScript).ExtractString() == "") {
  }
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript), "pass");

  SetFingerprintingDefault();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  while (content::EvalJs(contents(), kTitleScript).ExtractString() == "") {
  }
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript), "pass");
}

IN_PROC_BROWSER_TEST_F(HnsOffscreenCanvasFarblingBrowserTest,
                       FarbleGetImageData) {
  GURL url = embedded_test_server()->GetURL(
      "a.com", "/offscreen-getimagedata-farbling.html");

  AllowFingerprinting();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  // wait for worker thread to complete
  while (content::EvalJs(contents(), kTitleScript).ExtractString() == "") {
  }
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript),
            kExpectedImageDataHashFarblingOff);

  BlockFingerprinting();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  while (content::EvalJs(contents(), kTitleScript).ExtractString() == "") {
  }
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript),
            kExpectedImageDataHashFarblingMaximum);

  SetFingerprintingDefault();
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  while (content::EvalJs(contents(), kTitleScript).ExtractString() == "") {
  }
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript),
            kExpectedImageDataHashFarblingBalanced);

  // Turn off shields to test that the worker content settings agent
  // properly respects shields setting separately from fingerprinting
  // setting.
  hns_shields::SetHnsShieldsEnabled(content_settings(), false, url);
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  while (content::EvalJs(contents(), kTitleScript).ExtractString() == "") {
  }
  EXPECT_EQ(content::EvalJs(contents(), kTitleScript),
            kExpectedImageDataHashFarblingOff);
}
