/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_DELEGATE_H_
#define HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_DELEGATE_H_

#include <string>

#include "base/memory/weak_ptr.h"

namespace hns_tooltips {

class HnsTooltipDelegate
    : public base::SupportsWeakPtr<HnsTooltipDelegate> {
 public:
  virtual ~HnsTooltipDelegate() = default;

  // Called when the tooltip is shown
  virtual void OnTooltipShow(const std::string& tooltip_id) {}

  // Called when the tooltip is closed. If closed by a user explicitly
  // then |by_user| should be true, otherwise false
  virtual void OnTooltipClose(const std::string& tooltip_id,
                              const bool by_user) {}

  // Called when the underlying widget for the tooltip is destroyed
  virtual void OnTooltipWidgetDestroyed(const std::string& tooltip_id) {}

  // Called when the Ok button is pressed
  virtual void OnTooltipOkButtonPressed(const std::string& tooltip_id) {}

  // Called when the Cancel button is pressed
  virtual void OnTooltipCancelButtonPressed(const std::string& tooltip_id) {}
};

}  // namespace hns_tooltips

#endif  // HNS_BROWSER_UI_HNS_TOOLTIPS_HNS_TOOLTIP_DELEGATE_H_
