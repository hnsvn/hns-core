/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_POPUP_HANDLER_H_
#define HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_POPUP_HANDLER_H_

#include <memory>
#include <string>

class Profile;

namespace hns_tooltips {

class HnsTooltip;

class HnsTooltipPopupHandler {
 public:
  HnsTooltipPopupHandler();
  ~HnsTooltipPopupHandler();

  HnsTooltipPopupHandler(const HnsTooltipPopupHandler&) = delete;
  HnsTooltipPopupHandler& operator=(const HnsTooltipPopupHandler&) = delete;

  // Show the |tooltip| for the given |profile|.
  static void Show(Profile* profile, std::unique_ptr<HnsTooltip> tooltip);

  // Close the tooltip with the associated |tooltip_id|.
  static void Close(const std::string& tooltip_id);

  // Destroy the tooltip with the associated |tooltip_id|.
  static void Destroy(const std::string& tooltip_id);
};

}  // namespace hns_tooltips

#endif  // HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_POPUP_HANDLER_H_
