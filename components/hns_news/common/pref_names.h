// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_HNS_NEWS_COMMON_PREF_NAMES_H_
#define HNS_COMPONENTS_HNS_NEWS_COMMON_PREF_NAMES_H_

namespace hns_news {
namespace prefs {

constexpr char kNewTabPageShowToday[] = "hns.new_tab_page.show_hns_news";
constexpr char kHnsNewsSources[] = "hns.today.sources";
constexpr char kHnsNewsChannels[] = "hns.news.channels";
constexpr char kHnsNewsDirectFeeds[] = "hns.today.userfeeds";
constexpr char kHnsNewsIntroDismissed[] = "hns.today.intro_dismissed";
constexpr char kHnsNewsOptedIn[] = "hns.today.opted_in";
constexpr char kHnsNewsDaysInMonthUsedCount[] =
    "hns.today.p3a_days_in_month_count";
constexpr char kShouldShowToolbarButton[] =
    "hns.today.should_show_toolbar_button";
constexpr char kHnsNewsWeeklySessionCount[] =
    "hns.today.p3a_weekly_session_count";
constexpr char kHnsNewsWeeklyCardViewsCount[] =
    "hns.today.p3a_weekly_card_views_count";
constexpr char kHnsNewsWeeklyCardVisitsCount[] =
    "hns.today.p3a_weekly_card_visits_count";
constexpr char kHnsNewsWeeklyDisplayAdViewedCount[] =
    "hns.today.p3a_weekly_display_ad_viewed_count";
constexpr char kHnsNewsWeeklyAddedDirectFeedsCount[] =
    "hns.today.p3a_weekly_added_direct_feeds_count";
constexpr char kHnsNewsTotalCardViews[] = "hns.today.p3a_total_card_views";
constexpr char kHnsNewsCurrSessionCardViews[] =
    "hns.today.p3a_curr_session_card_views";
constexpr char kHnsNewsFirstSessionTime[] =
    "hns.today.p3a_first_session_time";
constexpr char kHnsNewsUsedSecondDay[] = "hns.today.p3a_used_second_day";
constexpr char kHnsNewsLastSessionTime[] =
    "hns.today.p3a_last_session_time";
constexpr char kHnsNewsWasEverEnabled[] = "hns.today.p3a_was_ever_enabled";

// Dictionary value keys
constexpr char kHnsNewsDirectFeedsKeyTitle[] = "title";
constexpr char kHnsNewsDirectFeedsKeySource[] = "source";

}  // namespace prefs

}  // namespace hns_news

#endif  // HNS_COMPONENTS_HNS_NEWS_COMMON_PREF_NAMES_H_
