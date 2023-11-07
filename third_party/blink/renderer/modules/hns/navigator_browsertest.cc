// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#include "base/path_service.h"
#include "hns/components/constants/hns_paths.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"
#include "content/public/test/browser_test_utils.h"

namespace {

const char kDetectHnsTest[] = "/detect_hns.html";

}  // namespace

class NavigatorGetHnsDetectedTest : public InProcessBrowserTest {
 public:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();

    content::SetupCrossSiteRedirector(embedded_test_server());

    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    embedded_test_server()->ServeFilesFromDirectory(test_data_dir);

    ASSERT_TRUE(embedded_test_server()->Start());
  }
};

IN_PROC_BROWSER_TEST_F(NavigatorGetHnsDetectedTest, IsDetected) {
  GURL url = embedded_test_server()->GetURL(kDetectHnsTest);
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  EXPECT_EQ(true, EvalJs(contents, "getHnsDetected()"));
}

IN_PROC_BROWSER_TEST_F(NavigatorGetHnsDetectedTest,
                       IsDetectedInServiceWorker) {
  GURL url = embedded_test_server()->GetURL("/simple.html");
  ASSERT_TRUE(ui_test_utils::NavigateToURL(browser(), url));
  content::WebContents* contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  std::string result;
  EXPECT_EQ(content::EvalJs(contents, R"(
        (() => {
          return new Promise(resolve => {
            navigator.serviceWorker.addEventListener('message', msg => {
              resolve(msg.data);
            });
            navigator.serviceWorker.register('./detect_hns_service_worker.js')
              .then(registration => {
                if (registration.active) {
                  registration.active.postMessage('isHns');
                } else if (registration.installing) {
                  registration.installing.addEventListener('statechange', () => {
                    if (registration.active) {
                      registration.active.postMessage('isHns');
                    }
                  });
                }
              })
          })
        })();
      )"),
            "HNS");
}
