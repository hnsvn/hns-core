/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/views/omnibox/hns_omnibox_result_view.h"

#include <memory>

#include "base/time/time.h"
#include "hns/browser/ui/views/omnibox/hns_search_conversion_promotion_view.h"
#include "hns/components/omnibox/browser/promotion_utils.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/ui/views/omnibox/omnibox_popup_view_views.h"
#include "chrome/browser/ui/views/omnibox/omnibox_suggestion_button_row_view.h"
#include "components/omnibox/browser/autocomplete_controller.h"
#include "components/omnibox/browser/autocomplete_match.h"
#include "components/omnibox/browser/autocomplete_provider_client.h"
#include "components/omnibox/browser/autocomplete_result.h"
#include "components/omnibox/browser/omnibox_controller.h"
#include "components/omnibox/browser/omnibox_edit_model.h"
#include "components/omnibox/browser/omnibox_popup_selection.h"
#include "ui/base/metadata/metadata_impl_macros.h"
#include "ui/base/window_open_disposition.h"
#include "ui/views/controls/button/image_button.h"

HnsOmniboxResultView::~HnsOmniboxResultView() = default;

void HnsOmniboxResultView::ResetChildrenVisibility() {
  // Reset children visibility. Their visibility could be configured later
  // based on |match_| and the current input.
  // NOTE: The first child in the result box is supposed to be the
  // `suggestion_container_`, which used to be stored as a data member.
  children().front()->SetVisible(true);
  button_row_->SetVisible(true);
  if (hns_search_promotion_view_) {
    hns_search_promotion_view_->SetVisible(false);
  }
}

void HnsOmniboxResultView::SetMatch(const AutocompleteMatch& match) {
  ResetChildrenVisibility();
  OmniboxResultView::SetMatch(match);

  if (IsHnsSearchPromotionMatch(match)) {
    UpdateForHnsSearchConversion();
  }
}

void HnsOmniboxResultView::OnSelectionStateChanged() {
  OmniboxResultView::OnSelectionStateChanged();

  HandleSelectionStateChangedForPromotionView();
}

void HnsOmniboxResultView::OpenMatch() {
  popup_view_->model()->OpenSelection(OmniboxPopupSelection(model_index_),
                                      base::TimeTicks::Now());
}

void HnsOmniboxResultView::RefreshOmniboxResult() {
  auto* controller = popup_view_->controller()->autocomplete_controller();

  // To refresh autocomplete result, start again with current input.
  controller->Start(controller->input());
}

void HnsOmniboxResultView::HandleSelectionStateChangedForPromotionView() {
  if (hns_search_promotion_view_ && IsHnsSearchPromotionMatch(match_)) {
    hns_search_promotion_view_->OnSelectionStateChanged(
        GetMatchSelected() &&
        popup_view_->GetSelection().state == OmniboxPopupSelection::NORMAL);
  }
}

void HnsOmniboxResultView::UpdateForHnsSearchConversion() {
  DCHECK(IsHnsSearchPromotionMatch(match_));

  // Hide upstream children and show our promotion view.
  // NOTE: The first child in the result box is supposed to be the
  // `suggestion_container_`, which used to be stored as a data member.
  children().front()->SetVisible(false);
  button_row_->SetVisible(false);

  if (!hns_search_promotion_view_) {
    auto* controller = popup_view_->controller()->autocomplete_controller();
    auto* prefs = controller->autocomplete_provider_client()->GetPrefs();
    hns_search_promotion_view_ =
        AddChildView(std::make_unique<HnsSearchConversionPromotionView>(
            this, g_browser_process->local_state(), prefs));
  }

  hns_search_promotion_view_->SetVisible(true);
  hns_search_promotion_view_->SetTypeAndInput(
      GetConversionTypeFromMatch(match_),
      popup_view_->controller()->autocomplete_controller()->input().text());
}

BEGIN_METADATA(HnsOmniboxResultView, OmniboxResultView)
END_METADATA
