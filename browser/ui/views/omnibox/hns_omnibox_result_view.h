/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_OMNIBOX_RESULT_VIEW_H_
#define HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_OMNIBOX_RESULT_VIEW_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/views/omnibox/omnibox_result_view.h"
#include "ui/base/metadata/metadata_header_macros.h"

class HnsSearchConversionPromotionView;

// This will render hns specific matches such as the hnsr search conversion
// promotion.
class HnsOmniboxResultView : public OmniboxResultView {
 public:
  METADATA_HEADER(HnsOmniboxResultView);
  using OmniboxResultView::OmniboxResultView;
  HnsOmniboxResultView(const HnsOmniboxResultView&) = delete;
  HnsOmniboxResultView& operator=(const HnsOmniboxResultView&) = delete;
  ~HnsOmniboxResultView() override;

  // OmniboxResultView overrides:
  void SetMatch(const AutocompleteMatch& match) override;
  void OnSelectionStateChanged() override;

  void OpenMatch();
  void RefreshOmniboxResult();

 private:
  void ResetChildrenVisibility();
  void UpdateForHnsSearchConversion();
  void HandleSelectionStateChangedForPromotionView();

  // Hns search conversion promotion
  raw_ptr<HnsSearchConversionPromotionView> hns_search_promotion_view_ =
      nullptr;
};

#endif  // HNS_BROWSER_UI_VIEWS_OMNIBOX_HNS_OMNIBOX_RESULT_VIEW_H_
