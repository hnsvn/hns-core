/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_browser_features.h"
#include "hns/browser/url_sanitizer/url_sanitizer_service_factory.h"
#include "hns/components/url_sanitizer/browser/url_sanitizer_service.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "chrome/browser/ui/views/omnibox/omnibox_view_views.h"
#include "chrome/browser/ui/views/toolbar/toolbar_view.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"
#include "ui/base/clipboard/clipboard.h"
#include "ui/strings/grit/ui_strings.h"
#include "ui/views/controls/textfield/textfield.h"
#include "url/gurl.h"

class HnsOmniboxViewViewsTest : public InProcessBrowserTest {
 public:
  LocationBarView* location_bar() {
    auto* browser_view = BrowserView::GetBrowserViewForBrowser(browser());
    return browser_view->toolbar()->location_bar();
  }
  OmniboxViewViews* omnibox_view() { return location_bar()->omnibox_view(); }
};

class HnsOmniboxViewViewsEnabledFeatureTest
    : public HnsOmniboxViewViewsTest {
 public:
  HnsOmniboxViewViewsEnabledFeatureTest() {
    features_.InitWithFeatureState(features::kHnsCopyCleanLinkByDefault,
                                   true);
  }

 private:
  base::test::ScopedFeatureList features_;
};

class HnsOmniboxViewViewsDisabledFeatureTest
    : public HnsOmniboxViewViewsTest {
 public:
  HnsOmniboxViewViewsDisabledFeatureTest() {
    features_.InitWithFeatureState(features::kHnsCopyCleanLinkByDefault,
                                   false);
  }

 private:
  base::test::ScopedFeatureList features_;
};

// Load hns url and check copied url also has hns scheme.
IN_PROC_BROWSER_TEST_F(HnsOmniboxViewViewsTest,
                       CopyInternalURLToClipboardTest) {
  const std::string test_url("hns://version/");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), GURL(test_url)));

  omnibox_view()->SelectAll(true);
  omnibox_view()->ExecuteCommand(views::Textfield::kCopy, 0);
  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
  std::string text_from_clipboard;
  clipboard->ReadAsciiText(ui::ClipboardBuffer::kCopyPaste,
                           /* data_dst = */ nullptr,
                           &text_from_clipboard);
  EXPECT_EQ(test_url, text_from_clipboard);

#if BUILDFLAG(IS_LINUX)
  clipboard->ReadAsciiText(ui::ClipboardBuffer::kSelection,
                           /* data_dst = */ nullptr,
                           &text_from_clipboard);
  EXPECT_EQ(test_url, text_from_clipboard);
#endif
}

IN_PROC_BROWSER_TEST_F(HnsOmniboxViewViewsTest, CopyCleanURLToClipboardTest) {
  hns::URLSanitizerServiceFactory::GetForBrowserContext(browser()->profile())
      ->Initialize(R"([
    { "include": [ "*://*/*"], "params": ["utm_content"] }
  ])");
  const std::string test_url(
      "https://dev-pages.hnssoftware.com/clean-urls/"
      "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&&;b&"
      "d&utm_content=removethis&e=&f=g&=end");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), GURL(test_url)));

  omnibox_view()->SelectAll(true);
  omnibox_view()->ExecuteCommand(IDC_COPY_CLEAN_LINK, 0);
  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
  std::string text_from_clipboard;
  clipboard->ReadAsciiText(ui::ClipboardBuffer::kCopyPaste,
                           /* data_dst = */ nullptr, &text_from_clipboard);
  EXPECT_EQ(text_from_clipboard,
            "https://dev-pages.hnssoftware.com/clean-urls/"
            "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&&;b&d&"
            "e=&f=g&=end");
}

IN_PROC_BROWSER_TEST_F(HnsOmniboxViewViewsTest, CopyURLToClipboardTest) {
  hns::URLSanitizerServiceFactory::GetForBrowserContext(browser()->profile())
      ->Initialize(R"([
    { "include": [ "*://*/*"], "params": ["utm_content"] }
  ])");
  const std::string test_url(
      "https://dev-pages.hnssoftware.com/clean-urls/"
      "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&"
      "d&utm_content=removethis&e=&f=g&=end");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), GURL(test_url)));

  omnibox_view()->SelectAll(true);
  omnibox_view()->ExecuteCommand(views::Textfield::kCopy, 0);
  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
  std::string text_from_clipboard;
  clipboard->ReadAsciiText(ui::ClipboardBuffer::kCopyPaste,
                           /* data_dst = */ nullptr, &text_from_clipboard);
  EXPECT_EQ(text_from_clipboard,
            "https://dev-pages.hnssoftware.com/clean-urls/"
            "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&d&"
            "utm_content=removethis&e=&f=g&=end");
}

IN_PROC_BROWSER_TEST_F(HnsOmniboxViewViewsEnabledFeatureTest,
                       CopyCleanedURLToClipboardByHotkey) {
  hns::URLSanitizerServiceFactory::GetForBrowserContext(browser()->profile())
      ->Initialize(R"([
    { "include": [ "*://*/*"], "params": ["utm_content"] }
  ])");
  const std::string test_url(
      "https://dev-pages.hnssoftware.com/clean-urls/"
      "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&&;b&"
      "d&utm_content=removethis&e=&f=g&=end");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), GURL(test_url)));

  omnibox_view()->SelectAll(true);

  auto* textfield = static_cast<views::Textfield*>(omnibox_view());
  textfield->AcceleratorPressed(
      ui::Accelerator(ui::VKEY_C, ui::EF_PLATFORM_ACCELERATOR));
  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
  std::string text_from_clipboard;
  clipboard->ReadAsciiText(ui::ClipboardBuffer::kCopyPaste,
                           /* data_dst = */ nullptr, &text_from_clipboard);
  EXPECT_EQ(text_from_clipboard,
            "https://dev-pages.hnssoftware.com/clean-urls/"
            "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&&;b&d&"
            "e=&f=g&=end");
}

IN_PROC_BROWSER_TEST_F(HnsOmniboxViewViewsTest, DoNotSanitizeInternalURLS) {
  const std::string test_url("hns://settings/?utm_ad=1");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), GURL(test_url)));
  hns::URLSanitizerServiceFactory::GetForBrowserContext(browser()->profile())
      ->Initialize(R"([
    { "include": [ "*://*/*"], "params": ["utm_ad"] }
  ])");
  base::RunLoop().RunUntilIdle();

  omnibox_view()->SelectAll(true);
  omnibox_view()->ExecuteCommand(views::Textfield::kCopy, 0);
  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
  std::string text_from_clipboard;
  clipboard->ReadAsciiText(ui::ClipboardBuffer::kCopyPaste,
                           /* data_dst = */ nullptr, &text_from_clipboard);
  EXPECT_EQ(text_from_clipboard, "hns://settings/?utm_ad=1");
}

IN_PROC_BROWSER_TEST_F(HnsOmniboxViewViewsDisabledFeatureTest,
                       CopyCleanedURLToClipboardByHotkey) {
  hns::URLSanitizerServiceFactory::GetForBrowserContext(browser()->profile())
      ->Initialize(R"([
    { "include": [ "*://*/*"], "params": ["utm_content"] }
  ])");
  const std::string test_url(
      "https://dev-pages.hnssoftware.com/clean-urls/"
      "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&&;b&"
      "d&utm_content=removethis&e=&f=g&=end");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), GURL(test_url)));

  omnibox_view()->SelectAll(true);

  auto* textfield = static_cast<views::Textfield*>(omnibox_view());
  textfield->AcceleratorPressed(
      ui::Accelerator(ui::VKEY_C, ui::EF_PLATFORM_ACCELERATOR));
  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
  std::string text_from_clipboard;
  clipboard->ReadAsciiText(ui::ClipboardBuffer::kCopyPaste,
                           /* data_dst = */ nullptr, &text_from_clipboard);
  EXPECT_EQ(text_from_clipboard,
            "https://dev-pages.hnssoftware.com/clean-urls/"
            "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&&;b&d&"
            "utm_content=removethis&e=&f=g&=end");
}

IN_PROC_BROWSER_TEST_F(HnsOmniboxViewViewsTest, CopyTextToClipboardByHotkey) {
  hns::URLSanitizerServiceFactory::GetForBrowserContext(browser()->profile())
      ->Initialize(R"([
    { "include": [ "*://*/*"], "params": ["utm_content"] }
  ])");
  const std::string test_text(
      "?hns_testing1=foo&hns_testing2=bar&hns_testing3=keep&&;b&"
      "d&utm_content=removethis&e=&f=g&=end");
  auto* textfield = static_cast<views::Textfield*>(omnibox_view());
  textfield->SetText(base::UTF8ToUTF16(test_text));

  omnibox_view()->SelectAll(true);

  textfield->AcceleratorPressed(
      ui::Accelerator(ui::VKEY_C, ui::EF_PLATFORM_ACCELERATOR));
  ui::Clipboard* clipboard = ui::Clipboard::GetForCurrentThread();
  std::string text_from_clipboard;
  clipboard->ReadAsciiText(ui::ClipboardBuffer::kCopyPaste,
                           /* data_dst = */ nullptr, &text_from_clipboard);
  EXPECT_EQ(text_from_clipboard, test_text);
}
