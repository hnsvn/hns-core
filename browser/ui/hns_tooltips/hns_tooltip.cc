/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_tooltips/hns_tooltip.h"

#include <vector>

#include "base/strings/string_util.h"

namespace hns_tooltips {

HnsTooltip::HnsTooltip(const std::string& id,
                           const HnsTooltipAttributes& attributes,
                           base::WeakPtr<HnsTooltipDelegate> delegate)
    : id_(id), attributes_(attributes), delegate_(std::move(delegate)) {}

HnsTooltip::~HnsTooltip() = default;

std::u16string HnsTooltip::accessible_name() const {
  std::vector<std::u16string> accessible_lines;

  if (!attributes_.title().empty()) {
    accessible_lines.push_back(attributes_.title());
  }

  if (!attributes_.body().empty()) {
    accessible_lines.push_back(attributes_.body());
  }

  return base::JoinString(accessible_lines, u"\n");
}

}  // namespace hns_tooltips
