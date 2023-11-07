/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_BROWSER_H_
#define HNS_BROWSER_UI_HNS_BROWSER_H_

#include <memory>

#include "chrome/browser/ui/browser.h"

#if defined(TOOLKIT_VIEWS)
namespace sidebar {
class SidebarController;
}  // namespace sidebar
#endif

class HnsBrowserWindow;

namespace content {
class WebContents;
}  // namespace content

class HnsBrowser : public Browser {
 public:
  explicit HnsBrowser(const CreateParams& params);
  ~HnsBrowser() override;

  HnsBrowser(const HnsBrowser&) = delete;
  HnsBrowser& operator=(const HnsBrowser&) = delete;

  // Browser overrides:
  void ScheduleUIUpdate(content::WebContents* source,
                        unsigned changed_flags) override;
  bool ShouldDisplayFavicon(content::WebContents* web_contents) const override;
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;
  void FinishWarnBeforeClosing(WarnBeforeClosingResult result) override;
  void BeforeUnloadFired(content::WebContents* source,
                         bool proceed,
                         bool* proceed_to_fire_unload) override;
  bool TryToCloseWindow(
      bool skip_beforeunload,
      const base::RepeatingCallback<void(bool)>& on_close_confirmed) override;
  void UpdateTargetURL(content::WebContents* source, const GURL& url) override;
  void ResetTryToCloseWindow() override;

  void TabStripEmpty() override;
  // Returns true when we should ask browser closing to users before handling
  // any warning/onbeforeunload handlers.
  bool ShouldAskForBrowserClosingBeforeHandlers();

#if defined(TOOLKIT_VIEWS)
  sidebar::SidebarController* sidebar_controller() {
    return sidebar_controller_.get();
  }
#endif

  HnsBrowserWindow* hns_window();

  void set_confirmed_to_close(bool close) { confirmed_to_close_ = close; }

 private:
  friend class HnsTestLauncherDelegate;
  friend class WindowClosingConfirmBrowserTest;
  friend class InProcessBrowserTest;

  // static
  static void SuppressBrowserWindowClosingDialogForTesting(bool suppress);

  std::unique_ptr<sidebar::SidebarController> sidebar_controller_;

  // Set true when user allowed to close browser before starting any
  // warning or onbeforeunload handlers.
  bool confirmed_to_close_ = false;
};

#endif  // HNS_BROWSER_UI_HNS_BROWSER_H_
