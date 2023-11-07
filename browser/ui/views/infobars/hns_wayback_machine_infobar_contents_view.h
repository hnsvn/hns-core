/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_

#include <vector>

#include "base/memory/raw_ptr.h"
#include "hns/components/hns_wayback_machine/wayback_machine_url_fetcher.h"
#include "ui/base/metadata/metadata_header_macros.h"
#include "ui/views/view.h"

namespace content {
class WebContents;
}  // namespace content

namespace views {
class Checkbox;
class ImageView;
class Label;
}  // namespace views

class HnsWaybackMachineInfoBarButtonContainer;
class GURL;
class PrefService;

// Includes all view controls except close button that managed by InfoBarView.
class HnsWaybackMachineInfoBarContentsView
    : public views::View,
      public WaybackMachineURLFetcher::Client {
 public:
  METADATA_HEADER(HnsWaybackMachineInfoBarContentsView);

  explicit HnsWaybackMachineInfoBarContentsView(
      content::WebContents* contents);
  ~HnsWaybackMachineInfoBarContentsView() override;

  HnsWaybackMachineInfoBarContentsView(
      const HnsWaybackMachineInfoBarContentsView&) = delete;
  HnsWaybackMachineInfoBarContentsView& operator=(
      const HnsWaybackMachineInfoBarContentsView&) = delete;

 private:
  using Labels = std::vector<views::Label*>;
  using Views = std::vector<views::View*>;

  // views::View overrides:
  void OnThemeChanged() override;

  // WaybackMachineURLFetcher::Client overrides:
  void OnWaybackURLFetched(const GURL& latest_wayback_url) override;

  void InitializeChildren();
  views::Label* CreateLabel(const std::u16string& text);
  SkColor GetColor(int id) const;
  void UpdateChildrenVisibility(bool show_before_checking_views);
  void FetchWaybackURL();
  void LoadURL(const GURL& url);
  void HideInfobar();

  void FetchURLButtonPressed();
  void OnCheckboxUpdated();

  // Used for labels theme changing all together.
  Labels labels_;
  Views views_visible_before_checking_;
  Views views_visible_after_checking_;
  raw_ptr<content::WebContents> contents_ = nullptr;
  WaybackMachineURLFetcher wayback_machine_url_fetcher_;

  raw_ptr<HnsWaybackMachineInfoBarButtonContainer> fetch_url_button_ =
      nullptr;
  raw_ptr<views::Checkbox> dont_ask_again_checkbox_ = nullptr;
  const raw_ptr<PrefService> pref_service_ = nullptr;
  raw_ptr<views::ImageView> wayback_spot_graphic_ = nullptr;
  bool wayback_url_fetch_requested_ = false;
};

#endif  // HNS_BROWSER_UI_VIEWS_INFOBARS_HNS_WAYBACK_MACHINE_INFOBAR_CONTENTS_VIEW_H_
