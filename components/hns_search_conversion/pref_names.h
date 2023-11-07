/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_SEARCH_CONVERSION_PREF_NAMES_H_
#define HNS_COMPONENTS_HNS_SEARCH_CONVERSION_PREF_NAMES_H_

class PrefService;

namespace hns_search_conversion {
namespace prefs {

constexpr char kDismissed[] = "hns.hns_search_conversion.dismissed";
constexpr char kMaybeLaterClickedTime[] =
    "hns.hns_search_conversion.maybe_later_clicked_time";

constexpr char kP3AActionStatuses[] =
    "hns.hns_search_conversion.action_statuses";

constexpr char kP3ADefaultEngineChanged[] =
    "hns.hns_search_conversion.default_changed";

constexpr char kP3ABannerShown[] =
    "hns.hns_search_conversion.banner_shown";  // DEPRECATED
constexpr char kP3ABannerTriggered[] =
    "hns.hns_search_conversion.banner_triggered";  // DEPRECATED
constexpr char kP3AButtonShown[] =
    "hns.hns_search_conversion.button_shown";  // DEPRECATED
constexpr char kP3ANTPShown[] =
    "hns.hns_search_conversion.ntp_shown";  // DEPRECATED
constexpr char kP3AButtonTriggered[] =
    "hns.hns_search_conversion.button_triggered";  // DEPRECATED
constexpr char kP3ANTPTriggered[] =
    "hns.hns_search_conversion.ntp_triggered";  // DEPRECATED

}  // namespace prefs
}  // namespace hns_search_conversion

#endif  // HNS_COMPONENTS_HNS_SEARCH_CONVERSION_PREF_NAMES_H_
