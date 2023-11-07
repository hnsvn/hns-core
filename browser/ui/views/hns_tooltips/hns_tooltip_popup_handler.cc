/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_tooltips/hns_tooltip_popup_handler.h"

#include <map>
#include <string>

#include "hns/browser/ui/hns_tooltips/hns_tooltip.h"
#include "hns/browser/ui/views/hns_tooltips/hns_tooltip_popup.h"

namespace {

std::map<std::string, hns_tooltips::HnsTooltipPopup* /* NOT OWNED */>
    tooltip_popups_;

}  // namespace

namespace hns_tooltips {

HnsTooltipPopupHandler::HnsTooltipPopupHandler() = default;

HnsTooltipPopupHandler::~HnsTooltipPopupHandler() = default;

// static
void HnsTooltipPopupHandler::Show(Profile* profile,
                                    std::unique_ptr<HnsTooltip> tooltip) {
  DCHECK(profile);
  DCHECK(tooltip);

  const std::string tooltip_id = tooltip->id();
  DCHECK(!tooltip_popups_[tooltip_id]);
  tooltip_popups_[tooltip_id] =
      new hns_tooltips::HnsTooltipPopup(profile, std::move(tooltip));
}

// static
void HnsTooltipPopupHandler::Close(const std::string& tooltip_id) {
  DCHECK(!tooltip_id.empty());

  if (!tooltip_popups_[tooltip_id]) {
    return;
  }

  tooltip_popups_[tooltip_id]->Close(false);
}

// static
void HnsTooltipPopupHandler::Destroy(const std::string& tooltip_id) {
  DCHECK(!tooltip_id.empty());

  // Note: The pointed-to HnsTooltipPopup members are deallocated by their
  // containing Widgets
  tooltip_popups_.erase(tooltip_id);
}

}  // namespace hns_tooltips
