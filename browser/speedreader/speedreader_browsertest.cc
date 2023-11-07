/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/command_line.h"
#include "base/functional/bind.h"
#include "base/path_service.h"
#include "base/run_loop.h"
#include "base/strings/escape.h"
#include "base/strings/string_piece_forward.h"
#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "base/test/bind.h"
#include "base/test/scoped_feature_list.h"
#include "base/time/time.h"
#include "hns/app/hns_command_ids.h"
#include "hns/browser/speedreader/page_distiller.h"
#include "hns/browser/speedreader/speedreader_service_factory.h"
#include "hns/browser/speedreader/speedreader_tab_helper.h"
#include "hns/browser/ui/views/frame/hns_browser_view.h"
#include "hns/browser/ui/webui/speedreader/speedreader_toolbar_data_handler_impl.h"
#include "hns/components/ai_chat/common/buildflags/buildflags.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/constants/hns_paths.h"
#include "hns/components/speedreader/common/constants.h"
#include "hns/components/speedreader/common/features.h"
#include "hns/components/speedreader/common/speedreader.mojom.h"
#include "hns/components/speedreader/common/speedreader_toolbar.mojom.h"
#include "hns/components/speedreader/speedreader_service.h"
#include "hns/components/speedreader/speedreader_util.h"
#include "chrome/browser/profiles/keep_alive/profile_keep_alive_types.h"
#include "chrome/browser/profiles/keep_alive/scoped_profile_keep_alive.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "chrome/browser/ui/browser_commands.h"
#include "chrome/browser/ui/browser_list.h"
#include "chrome/browser/ui/side_panel/side_panel_ui.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/frame/toolbar_button_provider.h"
#include "chrome/browser/ui/views/page_action/page_action_icon_view.h"
#include "chrome/common/chrome_isolated_world_ids.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/dom_distiller/core/dom_distiller_switches.h"
#include "components/keep_alive_registry/keep_alive_types.h"
#include "components/keep_alive_registry/scoped_keep_alive.h"
#include "components/network_session_configurator/common/network_switches.h"
#include "content/public/browser/reload_type.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"
#include "content/public/test/test_navigation_observer.h"
#include "content/public/test/test_utils.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "net/dns/mock_host_resolver.h"
#include "net/test/embedded_test_server/embedded_test_server.h"
#include "net/test/embedded_test_server/http_request.h"
#include "net/test/embedded_test_server/http_response.h"

#if BUILDFLAG(ENABLE_AI_CHAT)
#include "hns/components/ai_chat/common/features.h"
#endif

const char kTestHost[] = "a.test";
const char kTestPageSimple[] = "/simple.html";
const char kTestPageReadable[] = "/articles/guardian.html";
const char kTestPageReadableOnUnreadablePath[] =
    "/speedreader/rewriter/pages/news_pages/abcnews.com/distilled.html";
const char kTestPageRedirect[] = "/articles/redirect_me.html";
const char kTestXml[] = "/article/rss.xml";

class SpeedReaderBrowserTest : public InProcessBrowserTest {
 public:
  SpeedReaderBrowserTest()
      : https_server_(net::EmbeddedTestServer::TYPE_HTTPS) {
#if BUILDFLAG(ENABLE_AI_CHAT)
    feature_list_.InitWithFeatures(
        {speedreader::kSpeedreaderFeature, ai_chat::features::kAIChat}, {});
#else
    feature_list_.InitAndEnableFeature(speedreader::kSpeedreaderFeature);
#endif
    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    https_server_.SetSSLConfig(net::EmbeddedTestServer::CERT_TEST_NAMES);

    auto redirector = [](const net::test_server::HttpRequest& request)
        -> std::unique_ptr<net::test_server::HttpResponse> {
      if (request.GetURL().path_piece() != kTestPageRedirect) {
        return nullptr;
      }
      const std::string dest =
          base::UnescapeBinaryURLComponent(request.GetURL().query_piece());

      auto http_response =
          std::make_unique<net::test_server::BasicHttpResponse>();
      http_response->set_code(net::HTTP_MOVED_PERMANENTLY);
      http_response->AddCustomHeader("Location", dest);
      http_response->AddCustomHeader("Access-Control-Allow-Origin", "*");
      http_response->set_content_type("text/html");
      return http_response;
    };

    https_server_.RegisterDefaultHandler(base::BindRepeating(redirector));
    https_server_.ServeFilesFromDirectory(test_data_dir);

    EXPECT_TRUE(https_server_.Start());
  }

  SpeedReaderBrowserTest(const SpeedReaderBrowserTest&) = delete;
  SpeedReaderBrowserTest& operator=(const SpeedReaderBrowserTest&) = delete;

  ~SpeedReaderBrowserTest() override = default;

  void SetUpOnMainThread() override {
    host_resolver()->AddRule("*", "127.0.0.1");
  }

  void TearDownOnMainThread() override { DisableSpeedreader(); }

  content::WebContents* ActiveWebContents() {
    return browser()->tab_strip_model()->GetActiveWebContents();
  }

  speedreader::SpeedreaderTabHelper* tab_helper() {
    return speedreader::SpeedreaderTabHelper::FromWebContents(
        ActiveWebContents());
  }

  speedreader::SpeedreaderService* speedreader_service() {
    return speedreader::SpeedreaderServiceFactory::GetForBrowserContext(
        browser()->profile());
  }

  void NonBlockingDelay(const base::TimeDelta& delay) {
    base::RunLoop run_loop(base::RunLoop::Type::kNestableTasksAllowed);
    base::SingleThreadTaskRunner::GetCurrentDefault()->PostDelayedTask(
        FROM_HERE, run_loop.QuitWhenIdleClosure(), delay);
    run_loop.Run();
  }

  PageActionIconView* GetReaderButton() {
    return BrowserView::GetBrowserViewForBrowser(browser())
        ->toolbar_button_provider()
        ->GetPageActionIconView(PageActionIconType::kReaderMode);
  }

  void WaitDistilled(speedreader::SpeedreaderTabHelper* th = nullptr) {
    if (!th) {
      th = tab_helper();
    }
    while (!speedreader::DistillStates::IsDistilled(th->PageDistillState())) {
      NonBlockingDelay(base::Milliseconds(10));
    }
    content::WaitForLoadStop(ActiveWebContents());
  }

  void WaitDistillable(speedreader::SpeedreaderTabHelper* th = nullptr) {
    if (!th) {
      th = tab_helper();
    }
    while (!speedreader::DistillStates::IsDistillable(th->PageDistillState())) {
      NonBlockingDelay(base::Milliseconds(10));
    }
    content::WaitForLoadStop(ActiveWebContents());
  }

  void WaitOriginal(speedreader::SpeedreaderTabHelper* th = nullptr) {
    if (!th) {
      th = tab_helper();
    }
    while (
        !speedreader::DistillStates::IsViewOriginal(th->PageDistillState())) {
      NonBlockingDelay(base::Milliseconds(10));
    }
    content::WaitForLoadStop(ActiveWebContents());
  }

  void ClickReaderButton() {
    const auto was_distilled = speedreader::DistillStates::IsDistilled(
        tab_helper()->PageDistillState());
    browser()->command_controller()->ExecuteCommand(
        IDC_SPEEDREADER_ICON_ONCLICK);
    if (!was_distilled) {
      WaitDistilled();
    } else {
      WaitDistillable();
    }
    content::WaitForLoadStop(ActiveWebContents());
  }

  void ToggleSpeedreader() {
    speedreader_service()->EnableForAllSites(
        !speedreader_service()->IsEnabledForAllSites());
  }

  void DisableSpeedreader() { speedreader_service()->EnableForAllSites(false); }

  void GoBack(Browser* browser) {
    content::TestNavigationObserver observer(ActiveWebContents());
    chrome::GoBack(browser, WindowOpenDisposition::CURRENT_TAB);
    observer.Wait();
  }

  void NavigateToPageSynchronously(
      base::StringPiece path,
      WindowOpenDisposition disposition =
          WindowOpenDisposition::NEW_FOREGROUND_TAB) {
    const GURL url = https_server_.GetURL(kTestHost, path);
    ASSERT_TRUE(ui_test_utils::NavigateToURLWithDisposition(
        browser(), url, disposition,
        ui_test_utils::BROWSER_TEST_WAIT_FOR_LOAD_STOP));
  }

 protected:
  base::test::ScopedFeatureList feature_list_;
  net::EmbeddedTestServer https_server_;
};

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, RestoreSpeedreaderPage) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageReadable);
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));

  Profile* profile = browser()->profile();

  ScopedKeepAlive test_keep_alive(KeepAliveOrigin::PANEL_VIEW,
                                  KeepAliveRestartOption::DISABLED);
  ScopedProfileKeepAlive test_profile_keep_alive(
      profile, ProfileKeepAliveOrigin::kBrowserWindow);
  CloseBrowserSynchronously(browser());

  EXPECT_EQ(0u, BrowserList::GetInstance()->size());
  chrome::OpenWindowWithRestoredTabs(profile);
  EXPECT_EQ(1u, BrowserList::GetInstance()->size());
  SelectFirstBrowser();
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, NavigationNostickTest) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageSimple);
  EXPECT_FALSE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
  NavigateToPageSynchronously(kTestPageReadable,
                              WindowOpenDisposition::CURRENT_TAB);
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));

  // Ensure distill state doesn't stick when we back-navigate from a readable
  // page to a non-readable one.
  GoBack(browser());
  EXPECT_FALSE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, DisableSiteWorks) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageReadable);
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
  speedreader_service()->EnableForSite(ActiveWebContents(), false);
  EXPECT_TRUE(WaitForLoadStop(ActiveWebContents()));
  EXPECT_FALSE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, SmokeTest) {
  // Solana web3.js console warning will interfere with console observer
  hns_wallet::SetDefaultSolanaWallet(
      browser()->profile()->GetPrefs(),
      hns_wallet::mojom::DefaultWallet::None);
  ToggleSpeedreader();

  content::WebContentsConsoleObserver console_observer(ActiveWebContents());
  console_observer.SetFilter(base::BindLambdaForTesting(
      [](const content::WebContentsConsoleObserver::Message& message) {
        return message.log_level != blink::mojom::ConsoleMessageLevel::kVerbose;
      }));
  NavigateToPageSynchronously(kTestPageReadable,
                              WindowOpenDisposition::CURRENT_TAB);

  const std::string kGetStyleLength =
      "document.getElementById('hns_speedreader_style').innerHTML.length";
  const std::string kGetFontsExists =
      "!!(document.getElementById('atkinson_hyperligible_font') && "
      "document.getElementById('open_dyslexic_font'))";

  const std::string kGetContentLength = "document.body.innerHTML.length";

  // Check that the document became much smaller and that non-empty speedreader
  // style is injected.
  EXPECT_LT(0, content::EvalJs(ActiveWebContents(), kGetStyleLength,
                               content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                               ISOLATED_WORLD_ID_HNS_INTERNAL)
                   .ExtractInt());
  EXPECT_TRUE(content::EvalJs(ActiveWebContents(), kGetFontsExists,
                              content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                              ISOLATED_WORLD_ID_HNS_INTERNAL)
                  .ExtractBool());
  EXPECT_GT(17750, content::EvalJs(ActiveWebContents(), kGetContentLength,
                                   content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                   ISOLATED_WORLD_ID_HNS_INTERNAL)
                       .ExtractInt());

  EXPECT_TRUE(console_observer.messages().empty());

  // Check that disabled speedreader doesn't affect the page.
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageReadable);
  EXPECT_LT(106000, content::EvalJs(ActiveWebContents(), kGetContentLength,
                                    content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                    ISOLATED_WORLD_ID_HNS_INTERNAL)
                        .ExtractInt());
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, Redirect) {
  ToggleSpeedreader();

  const auto redirect_url = https_server_.GetURL(
      kTestHost, "/speedreader/rewriter/jsonld_shortest_desc.html");
  NavigateToPageSynchronously(kTestPageRedirect + ("?" + redirect_url.spec()));

  const std::string kCheckNoStyle =
      "!document.getElementById('hns_speedreader_style')";

  EXPECT_TRUE(content::EvalJs(ActiveWebContents(), kCheckNoStyle,
                              content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                              ISOLATED_WORLD_ID_HNS_INTERNAL)
                  .ExtractBool());
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, ClickingOnReaderButton) {
  EXPECT_FALSE(speedreader_service()->IsEnabledForAllSites());

  NavigateToPageSynchronously(kTestPageReadable);
  EXPECT_TRUE(GetReaderButton()->GetVisible());

  EXPECT_FALSE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
  ClickReaderButton();
  EXPECT_TRUE(GetReaderButton()->GetVisible());
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
  EXPECT_TRUE(GetReaderButton()->GetVisible());

  ClickReaderButton();
  EXPECT_TRUE(GetReaderButton()->GetVisible());
  EXPECT_TRUE(speedreader::DistillStates::IsViewOriginal(
      tab_helper()->PageDistillState()));

  EXPECT_FALSE(speedreader_service()->IsEnabledForAllSites());
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, OnDemandReader) {
  EXPECT_FALSE(speedreader_service()->IsEnabledForAllSites());

  NavigateToPageSynchronously(kTestPageReadable);
  EXPECT_TRUE(GetReaderButton()->GetVisible());

  EXPECT_TRUE(speedreader::DistillStates::IsDistillable(
      tab_helper()->PageDistillState()));
  // Change content on the page.
  constexpr const char kChangeContent[] =
      R"js(
        document.querySelector('meta[property="og:title"]').content =
            'Title was changed by javascript'
      )js";
  EXPECT_TRUE(content::ExecJs(ActiveWebContents(), kChangeContent,
                              content::EXECUTE_SCRIPT_DEFAULT_OPTIONS));
  ClickReaderButton();

  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));

  // Check title on the distilled page.
  constexpr const char kCheckContent[] =
      R"js(
        !!document.getElementById('hns_speedreader_style') &&
        (document.title === 'Title was changed by javascript')
      )js";
  EXPECT_TRUE(content::EvalJs(ActiveWebContents(), kCheckContent,
                              content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                              ISOLATED_WORLD_ID_HNS_INTERNAL)
                  .ExtractBool());
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, EnableDisableSpeedreader) {
  EXPECT_FALSE(speedreader_service()->IsEnabledForAllSites());
  NavigateToPageSynchronously(kTestPageReadable);

  EXPECT_TRUE(GetReaderButton()->GetVisible());
  EXPECT_TRUE(speedreader::DistillStates::IsDistillable(
      tab_helper()->PageDistillState()));
  ToggleSpeedreader();
  WaitDistilled();
  EXPECT_TRUE(GetReaderButton()->GetVisible());
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
  DisableSpeedreader();
  WaitOriginal();
  EXPECT_TRUE(GetReaderButton()->GetVisible());
  EXPECT_TRUE(speedreader::DistillStates::IsDistillable(
      tab_helper()->PageDistillState()));
  EXPECT_TRUE(speedreader::DistillStates::IsViewOriginal(
      tab_helper()->PageDistillState()));

  ClickReaderButton();
  WaitDistilled();
  EXPECT_TRUE(GetReaderButton()->GetVisible());
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
  ToggleSpeedreader();
  WaitDistilled();
  EXPECT_TRUE(GetReaderButton()->GetVisible());
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
  DisableSpeedreader();
  WaitOriginal();
  EXPECT_TRUE(GetReaderButton()->GetVisible());
  EXPECT_TRUE(speedreader::DistillStates::IsDistillable(
      tab_helper()->PageDistillState()));
  EXPECT_TRUE(speedreader::DistillStates::IsViewOriginal(
      tab_helper()->PageDistillState()));
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, TogglingSiteSpeedreader) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageReadable);

  for (int i = 0; i < 2; ++i) {
    EXPECT_TRUE(WaitForLoadStop(ActiveWebContents()));
    EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
        tab_helper()->PageDistillState()));
    EXPECT_TRUE(GetReaderButton()->GetVisible());

    speedreader_service()->EnableForSite(ActiveWebContents(), false);
    EXPECT_TRUE(WaitForLoadStop(ActiveWebContents()));
    EXPECT_TRUE(speedreader::DistillStates::IsViewOriginal(
        tab_helper()->PageDistillState()));
    EXPECT_TRUE(GetReaderButton()->GetVisible());

    speedreader_service()->EnableForSite(ActiveWebContents(), true);
    EXPECT_TRUE(WaitForLoadStop(ActiveWebContents()));
  }
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, ReloadContent) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageReadable);
  auto* contents_1 = ActiveWebContents();
  NavigateToPageSynchronously(kTestPageReadable);
  auto* contents_2 = ActiveWebContents();

  auto* tab_helper_1 =
      speedreader::SpeedreaderTabHelper::FromWebContents(contents_1);
  auto* tab_helper_2 =
      speedreader::SpeedreaderTabHelper::FromWebContents(contents_2);

  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper_1->PageDistillState()));
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper_2->PageDistillState()));

  speedreader_service()->EnableForSite(tab_helper_1->web_contents(), false);
  content::WaitForLoadStop(contents_1);
  EXPECT_TRUE(speedreader::DistillStates::IsViewOriginal(
      tab_helper_1->PageDistillState()));
  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper_2->PageDistillState()));

  contents_2->GetController().Reload(content::ReloadType::NORMAL, false);
  content::WaitForLoadStop(contents_2);

  EXPECT_TRUE(speedreader::DistillStates::IsViewOriginal(
      tab_helper_1->PageDistillState()));
  EXPECT_TRUE(speedreader::DistillStates::IsViewOriginal(
      tab_helper_2->PageDistillState()));
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, ShowOriginalPage) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageReadable);
  auto* web_contents = ActiveWebContents();

  constexpr const char kCheckNoApiInMainWorld[] =
      R"js(
        document.speedreader === undefined
      )js";
  EXPECT_TRUE(content::EvalJs(web_contents, kCheckNoApiInMainWorld,
                              content::EXECUTE_SCRIPT_DEFAULT_OPTIONS)
                  .ExtractBool());

  constexpr const char kClickLinkAndGetTitle[] =
      R"js(
    (function() {
      // element id is hardcoded in extractor.rs
      const link =
        document.getElementById('c93e2206-2f31-4ddc-9828-2bb8e8ed940e');
      link.click();
      return link.innerText
    })();
  )js";

  EXPECT_EQ("View original",
            content::EvalJs(web_contents, kClickLinkAndGetTitle,
                            content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                            ISOLATED_WORLD_ID_HNS_INTERNAL)
                .ExtractString());
  content::WaitForLoadStop(web_contents);
  auto* tab_helper =
      speedreader::SpeedreaderTabHelper::FromWebContents(web_contents);
  EXPECT_TRUE(speedreader::DistillStates::IsDistillable(
      tab_helper->PageDistillState()));
  EXPECT_TRUE(speedreader_service()->IsEnabledForSite(web_contents));

  // Click on speedreader button
  ClickReaderButton();
  content::WaitForLoadStop(web_contents);
  EXPECT_TRUE(
      speedreader::DistillStates::IsDistilled(tab_helper->PageDistillState()));
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, ShowOriginalPageOnUnreadable) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageSimple);
  auto* web_contents = ActiveWebContents();

  constexpr const char kCheckNoElement[] =
      R"js(
        document.getElementById('c93e2206-2f31-4ddc-9828-2bb8e8ed940e') == null
      )js";

  EXPECT_TRUE(content::EvalJs(web_contents, kCheckNoElement,
                              content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                              ISOLATED_WORLD_ID_HNS_INTERNAL)
                  .ExtractBool());

  constexpr const char kCheckNoApi[] =
      R"js(
        document.speedreader === undefined
      )js";

  EXPECT_TRUE(content::EvalJs(web_contents, kCheckNoApi,
                              content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                              ISOLATED_WORLD_ID_HNS_INTERNAL)
                  .ExtractBool());
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, SetDataAttributes) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageReadable);
  auto* contents = ActiveWebContents();

  // Open second tab
  NavigateToPageSynchronously(kTestPageReadable);

  auto GetDataAttribute = [](const std::string& attr) {
    constexpr const char kGetDataAttribute[] =
        R"js(
          document.documentElement.getAttribute('$1')
        )js";
    return base::ReplaceStringPlaceholders(kGetDataAttribute, {attr}, nullptr);
  };

  EXPECT_EQ(speedreader::mojom::Theme::kNone,
            speedreader_service()->GetAppearanceSettings().theme);
  EXPECT_EQ(speedreader::mojom::FontFamily::kSans,
            speedreader_service()->GetAppearanceSettings().fontFamily);
  EXPECT_EQ(speedreader::mojom::FontSize::k100,
            speedreader_service()->GetAppearanceSettings().fontSize);

  EXPECT_EQ(nullptr, content::EvalJs(contents, GetDataAttribute("data-theme"),
                                     content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                     ISOLATED_WORLD_ID_HNS_INTERNAL));
  speedreader_service()->SetAppearanceSettings(
      speedreader::mojom::AppearanceSettings(
          speedreader::mojom::Theme::kDark, speedreader::mojom::FontSize::k130,
          speedreader::mojom::FontFamily::kDyslexic));

  auto EvalAttr = [&](content::WebContents* contents, const std::string& attr) {
    return content::EvalJs(contents, GetDataAttribute(attr),
                           content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                           ISOLATED_WORLD_ID_HNS_INTERNAL)
        .ExtractString();
  };

  EXPECT_EQ("dark", EvalAttr(contents, "data-theme"));
  EXPECT_EQ("dyslexic", EvalAttr(contents, "data-font-family"));
  EXPECT_EQ("130", EvalAttr(contents, "data-font-size"));

  // Same in the second tab
  EXPECT_EQ("dark", EvalAttr(ActiveWebContents(), "data-theme"));
  EXPECT_EQ("dyslexic", EvalAttr(ActiveWebContents(), "data-font-family"));
  EXPECT_EQ("130", EvalAttr(ActiveWebContents(), "data-font-size"));

  EXPECT_EQ(speedreader::mojom::Theme::kDark,
            speedreader_service()->GetAppearanceSettings().theme);
  EXPECT_EQ(speedreader::mojom::FontFamily::kDyslexic,
            speedreader_service()->GetAppearanceSettings().fontFamily);
  EXPECT_EQ(speedreader::mojom::FontSize::k130,
            speedreader_service()->GetAppearanceSettings().fontSize);

  // New page
  NavigateToPageSynchronously(kTestPageReadable);
  EXPECT_EQ("dark", EvalAttr(ActiveWebContents(), "data-theme"));
  EXPECT_EQ("dyslexic", EvalAttr(ActiveWebContents(), "data-font-family"));
  EXPECT_EQ("130", EvalAttr(ActiveWebContents(), "data-font-size"));
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, Toolbar) {
  auto GetDataAttribute = [](const std::string& attr) {
    constexpr const char kGetDataAttribute[] =
        R"js(
          document.documentElement.getAttribute('$1')
        )js";
    return base::ReplaceStringPlaceholders(kGetDataAttribute, {attr}, nullptr);
  };

  auto WaitAttr = [&](content::WebContents* contents, const std::string& attr,
                      const std::string& value) {
    for (;;) {
      NonBlockingDelay(base::Milliseconds(10));
      auto eval = content::EvalJs(contents, GetDataAttribute(attr),
                                  content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                  ISOLATED_WORLD_ID_HNS_INTERNAL);
      if (!eval.value.is_string() && value.empty()) {
        return true;
      }
      if (eval.ExtractString() == value) {
        return true;
      }
    }
  };

  auto WaitElement = [&](content::WebContents* contents,
                         const std::string& elem) {
    constexpr const char kWaitElement[] =
        R"js(
          (!!document.getElementById('$1'))
        )js";
    for (;;) {
      NonBlockingDelay(base::Milliseconds(10));
      if (content::EvalJs(
              contents,
              base::ReplaceStringPlaceholders(kWaitElement, {elem}, nullptr),
              content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
              ISOLATED_WORLD_ID_HNS_INTERNAL)
              .ExtractBool()) {
        break;
      }
    }
  };

  auto Click = [&](content::WebContents* contents, const std::string& id) {
    constexpr const char kClick[] =
        R"js(
          document.getElementById('$1').click()
        )js";
    ASSERT_TRUE(content::ExecJs(
        contents, base::ReplaceStringPlaceholders(kClick, {id}, nullptr)));
  };

  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestPageReadable);

  auto* page = ActiveWebContents();
  auto* toolbar_view = static_cast<HnsBrowserView*>(browser()->window())
                           ->reader_mode_toolbar_view_.get();
  auto* toolbar = toolbar_view->GetWebContentsForTesting();
  WaitElement(toolbar, "appearance");

#if BUILDFLAG(ENABLE_AI_CHAT)
  Click(toolbar, "ai");
  auto* side_panel = SidePanelUI::GetSidePanelUIForBrowser(browser());
  while (side_panel->GetCurrentEntryId() != SidePanelEntryId::kChatUI) {
    NonBlockingDelay(base::Milliseconds(10));
  }
  EXPECT_EQ(SidePanelEntryId::kChatUI, side_panel->GetCurrentEntryId());
  Click(toolbar, "ai");
  while (side_panel->GetCurrentEntryId().has_value()) {
    NonBlockingDelay(base::Milliseconds(10));
  }
  EXPECT_FALSE(side_panel->GetCurrentEntryId().has_value());
#endif

  Click(toolbar, "appearance");
  {  // change theme
    Click(toolbar, "theme-light");
    WaitAttr(page, "data-theme", "light");
    Click(toolbar, "theme-sepia");
    WaitAttr(page, "data-theme", "sepia");
    Click(toolbar, "theme-dark");
    WaitAttr(page, "data-theme", "dark");
    Click(toolbar, "theme-system");
    WaitAttr(page, "data-theme", "");
  }
  {  // change font
    Click(toolbar, "font-sans");
    WaitAttr(page, "data-font-family", "sans");
    Click(toolbar, "font-serif");
    WaitAttr(page, "data-font-family", "serif");
    Click(toolbar, "font-mono");
    WaitAttr(page, "data-font-family", "mono");
    Click(toolbar, "font-dyslexic");
    WaitAttr(page, "data-font-family", "dyslexic");
  }
  {  // change font size
    WaitAttr(page, "data-font-size", "100");
    Click(toolbar, "font-size-decrease");
    WaitAttr(page, "data-font-size", "90");
    Click(toolbar, "font-size-increase");
    WaitAttr(page, "data-font-size", "100");
    Click(toolbar, "font-size-increase");
    WaitAttr(page, "data-font-size", "110");
  }
  Click(toolbar, "appearance");

  Click(toolbar, "tune");
  {
    while (!tab_helper()->speedreader_bubble_view()) {
      NonBlockingDelay(base::Milliseconds(10));
    }
  }
  Click(toolbar, "tune");

  Click(toolbar, "close");
  {
    WaitOriginal();
    EXPECT_FALSE(toolbar_view->GetVisible());
  }
}

IN_PROC_BROWSER_TEST_F(SpeedReaderBrowserTest, RSS) {
  ToggleSpeedreader();
  NavigateToPageSynchronously(kTestXml);

  EXPECT_FALSE(GetReaderButton()->GetVisible());

  const std::string kNoStyleInjected =
      R"js(document.getElementById('hns_speedreader_style'))js";

  EXPECT_EQ(nullptr, content::EvalJs(ActiveWebContents(), kNoStyleInjected,
                                     content::EXECUTE_SCRIPT_DEFAULT_OPTIONS,
                                     ISOLATED_WORLD_ID_HNS_INTERNAL));
}

class SpeedReaderWithDistillationServiceBrowserTest
    : public SpeedReaderBrowserTest {
 public:
  void SetUpCommandLine(base::CommandLine* command_line) override {
    SpeedReaderBrowserTest::SetUpCommandLine(command_line);
    command_line->AppendSwitch(switches::kEnableDistillabilityService);
  }
};

IN_PROC_BROWSER_TEST_F(SpeedReaderWithDistillationServiceBrowserTest,
                       OnDemandReader) {
  EXPECT_FALSE(speedreader_service()->IsEnabledForAllSites());

  struct MockObserver : speedreader::PageDistiller::Observer {
    MOCK_METHOD(void,
                OnPageDistillStateChanged,
                (speedreader::PageDistiller::State),
                (override));
  };

  testing::NiceMock<MockObserver> observer;
  tab_helper()->speedreader::PageDistiller::AddObserver(&observer);

  base::RunLoop run_loop;
  ON_CALL(observer, OnPageDistillStateChanged(
                        speedreader::PageDistiller::State::kDistillable))
      .WillByDefault(
          testing::InvokeWithoutArgs(&run_loop, &base::RunLoop::Quit));

  NavigateToPageSynchronously(kTestPageReadableOnUnreadablePath,
                              WindowOpenDisposition::CURRENT_TAB);
  run_loop.Run();
  tab_helper()->speedreader::PageDistiller::RemoveObserver(&observer);

  EXPECT_TRUE(GetReaderButton()->GetVisible());

  EXPECT_TRUE(speedreader::DistillStates::IsDistillable(
      tab_helper()->PageDistillState()));

  ClickReaderButton();

  EXPECT_TRUE(speedreader::DistillStates::IsDistilled(
      tab_helper()->PageDistillState()));
}
