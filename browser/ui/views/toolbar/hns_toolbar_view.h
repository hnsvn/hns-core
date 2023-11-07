/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_TOOLBAR_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_TOOLBAR_VIEW_H_

#include "base/memory/raw_ptr.h"
#include "base/scoped_observation.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "chrome/browser/profiles/profile_attributes_storage.h"
#include "chrome/browser/ui/views/toolbar/toolbar_view.h"
#include "components/prefs/pref_member.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
class HnsVPNButton;
#endif

class BookmarkButton;
class WalletButton;

class HnsToolbarView : public ToolbarView,
                         public ProfileAttributesStorage::Observer {
 public:
  explicit HnsToolbarView(Browser* browser, BrowserView* browser_view);
  ~HnsToolbarView() override;

  BookmarkButton* bookmark_button() const { return bookmark_; }
  WalletButton* wallet_button() const { return wallet_; }

#if BUILDFLAG(ENABLE_HNS_VPN)
  HnsVPNButton* hns_vpn_button() const { return hns_vpn_; }
  bool IsHnsVPNButtonVisible() const;
  void OnVPNButtonVisibilityChanged();
#endif

  void UpdateHorizontalPadding();

  void Init() override;
  void Layout() override;
  void Update(content::WebContents* tab) override;
  void OnThemeChanged() override;
  void OnEditBookmarksEnabledChanged();
  void OnLocationBarIsWideChanged();
  void OnShowBookmarksButtonChanged();
  void ShowBookmarkBubble(const GURL& url, bool already_bookmarked) override;
  void ViewHierarchyChanged(
      const views::ViewHierarchyChangedDetails& details) override;

 private:
  void LoadImages() override;
  void ResetLocationBarBounds();
  void ResetButtonBounds();
  void UpdateBookmarkVisibility();

  // ProfileAttributesStorage::Observer:
  void OnProfileAdded(const base::FilePath& profile_path) override;
  void OnProfileWasRemoved(const base::FilePath& profile_path,
                           const std::u16string& profile_name) override;

  raw_ptr<BookmarkButton> bookmark_ = nullptr;
  // Tracks the preference to determine whether bookmark editing is allowed.
  BooleanPrefMember edit_bookmarks_enabled_;

  raw_ptr<WalletButton> wallet_ = nullptr;

#if BUILDFLAG(ENABLE_HNS_VPN)
  raw_ptr<HnsVPNButton> hns_vpn_ = nullptr;
  BooleanPrefMember show_hns_vpn_button_;
  BooleanPrefMember hide_hns_vpn_button_by_policy_;
#endif

  BooleanPrefMember show_bookmarks_button_;

  BooleanPrefMember location_bar_is_wide_;

  BooleanPrefMember show_vertical_tabs_;
  BooleanPrefMember show_title_bar_on_vertical_tabs_;
#if BUILDFLAG(IS_LINUX)
  BooleanPrefMember use_custom_chrome_frame_;
#endif  // BUILDFLAG(IS_LINUX)

  // Whether this toolbar has been initialized.
  bool hns_initialized_ = false;
  // Tracks profile count to determine whether profile switcher should be shown.
  base::ScopedObservation<ProfileAttributesStorage,
                          ProfileAttributesStorage::Observer>
      profile_observer_{this};
};

#endif  // HNS_BROWSER_UI_VIEWS_TOOLBAR_HNS_TOOLBAR_VIEW_H_
