/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_LOCATION_BAR_HNS_LOCATION_BAR_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_LOCATION_BAR_HNS_LOCATION_BAR_VIEW_H_

#include <vector>

#include "base/gtest_prod_util.h"
#include "hns/browser/ui/views/location_bar/hns_news_location_view.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"

class HnsActionsContainer;
class HnsActionsContainerTest;
class PlaylistActionIconView;
class RewardsBrowserTest;
class SkPath;

#if BUILDFLAG(ENABLE_TOR)
class OnionLocationView;
#endif

#if BUILDFLAG(ENABLE_IPFS)
class IPFSLocationView;
#endif

namespace policy {
FORWARD_DECLARE_TEST(HnsRewardsPolicyTest, RewardsIconIsHidden);
}

// The purposes of this subclass are to:
// - Add the HnsActionsContainer to the location bar
class HnsLocationBarView : public LocationBarView {
 public:
  using LocationBarView::LocationBarView;

  HnsLocationBarView(const HnsLocationBarView&) = delete;
  HnsLocationBarView& operator=(const HnsLocationBarView&) = delete;

  void Init() override;
  void Update(content::WebContents* contents) override;
  void OnChanged() override;
  HnsActionsContainer* GetHnsActionsContainer() { return hns_actions_; }
#if BUILDFLAG(ENABLE_TOR)
  OnionLocationView* GetOnionLocationView() { return onion_location_view_; }
#endif

#if BUILDFLAG(ENABLE_IPFS)
  IPFSLocationView* GetIPFSLocationView() { return ipfs_location_view_; }
#endif
  // LocationBarView:
  std::vector<views::View*> GetTrailingViews() override;

  ui::ImageModel GetLocationIcon(LocationIconView::Delegate::IconFetchedCallback
                                     on_icon_fetched) const override;
  void OnOmniboxBlurred() override;

  // views::View:
  gfx::Size CalculatePreferredSize() const override;
  void OnThemeChanged() override;
  void ChildVisibilityChanged(views::View* child) override;

  int GetBorderRadius() const override;

  SkPath GetFocusRingHighlightPath() const;
  ContentSettingImageView* GetContentSettingsImageViewForTesting(size_t idx);
  bool ShouldShowIPFSLocationView() const;
  HnsActionsContainer* hns_actions_contatiner_view() {
    return hns_actions_;
  }

  void ShowPlaylistBubble();

 private:
  FRIEND_TEST_ALL_PREFIXES(policy::HnsRewardsPolicyTest, RewardsIconIsHidden);
  FRIEND_TEST_ALL_PREFIXES(PlaylistBrowserTest, AddItemsToList);
  friend class ::HnsActionsContainerTest;
  friend class ::RewardsBrowserTest;

  PlaylistActionIconView* GetPlaylistActionIconView();

  raw_ptr<HnsActionsContainer> hns_actions_ = nullptr;
  raw_ptr<HnsNewsLocationView> hns_news_location_view_ = nullptr;
#if BUILDFLAG(ENABLE_TOR)
  raw_ptr<OnionLocationView> onion_location_view_ = nullptr;
#endif
#if BUILDFLAG(ENABLE_IPFS)
  raw_ptr<IPFSLocationView> ipfs_location_view_ = nullptr;
#endif
};

#endif  // HNS_BROWSER_UI_VIEWS_LOCATION_BAR_HNS_LOCATION_BAR_VIEW_H_
