/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_ROUNDED_OMNIBOX_RESULTS_FRAME_H_
#define HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_ROUNDED_OMNIBOX_RESULTS_FRAME_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/views/omnibox/rounded_omnibox_results_frame.h"
#include "components/prefs/pref_member.h"

class Browser;
class LocationBarView;

class HnsRoundedOmniboxResultsFrame : public RoundedOmniboxResultsFrame {
 public:
  METADATA_HEADER(HnsRoundedOmniboxResultsFrame);
  HnsRoundedOmniboxResultsFrame(views::View* contents,
                                  LocationBarView* location_bar);
  ~HnsRoundedOmniboxResultsFrame() override;

 private:
  void UpdateShadowBorder();

  BooleanPrefMember show_vertical_tabs_;
  BooleanPrefMember show_window_title_for_vertical_tabs_;

  raw_ptr<Browser> browser_ = nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_ROUNDED_OMNIBOX_RESULTS_FRAME_H_
