/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/public/prefs/pref_names.h"  // IWYU pragma: keep

namespace hns_ads::prefs {

// Stores the preferences version number
const char kVersion[] = "hns.hns_ads.prefs.current_version";

// Prefix for preference names pertaining to p2a weekly metrics
const char kP2AStoragePrefNamePrefix[] = "hns.weekly_storage.";

// Stores whether we should show the My First notification ad
const char kShouldShowOnboardingNotification[] =
    "hns.hns_ads.should_show_my_first_ad_notification";

// Stores the last normalized screen position of custom notification ads and
// whether to fallback from native to custom notification ads if native
// notifications are disabled
const char kNotificationAdLastNormalizedDisplayCoordinateX[] =
    "hns.hns_ads.ad_notification.last_normalized_display_coordinate_x";
const char kNotificationAdLastNormalizedDisplayCoordinateY[] =
    "hns.hns_ads.ad_notification.last_normalized_display_coordinate_y";
const char kNotificationAdDidFallbackToCustom[] =
    "hns.hns_ads.ad_notification.did_fallback_to_custom";

// Stores the supported country codes current schema version number
const char kSupportedCountryCodesLastSchemaVersion[] =
    "hns.hns_ads.supported_regions_last_schema_version_number";

// Stores whether user has opted-in to notifications ads
const char kOptedInToNotificationAds[] = "hns.hns_ads.enabled";
const char kEnabledForLastProfile[] = "hns.hns_ads.enabled_last_profile";
const char kEverEnabledForAnyProfile[] =
    "hns.hns_ads.ever_enabled_any_profile";

// Stores a diagnostic id
const char kDiagnosticId[] = "hns.hns_ads.diagnostics.id";

// Stores the maximum number of notification ads per hour
const char kMaximumNotificationAdsPerHour[] = "hns.hns_ads.ads_per_hour";

// Notification ads
const char kNotificationAds[] = "hns.hns_ads.notification_ads";
const char kServeAdAt[] = "hns.hns_ads.serve_ad_at";

// Browser version
const char kBrowserVersionNumber[] = "hns.hns_ads.browser_version_number";

// Stores whether Hns ads should allow subdivision ad targeting
const char kShouldAllowSubdivisionTargeting[] =
    "hns.hns_ads.should_allow_ads_subdivision_targeting";

// Stores the selected subdivision targeting code
const char kSubdivisionTargetingSubdivision[] =
    "hns.hns_ads.ads_subdivision_targeting_code";

// Stores the automatically detected subdivision targeting code
const char kSubdivisionTargetingAutoDetectedSubdivision[] =
    "hns.hns_ads.automatically_detected_ads_subdivision_targeting_code";

// Stores catalog id
const char kCatalogId[] = "hns.hns_ads.catalog_id";

// Stores catalog version
const char kCatalogVersion[] = "hns.hns_ads.catalog_version";

// Stores catalog ping
const char kCatalogPing[] = "hns.hns_ads.catalog_ping";

// Stores catalog last updated
const char kCatalogLastUpdated[] = "hns.hns_ads.catalog_last_updated";

// Stores issuers
const char kIssuerPing[] = "hns.hns_ads.issuer_ping";
const char kIssuers[] = "hns.hns_ads.issuers";

// Stores epsilon greedy bandit
const char kEpsilonGreedyBanditArms[] =
    "hns.hns_ads.epsilon_greedy_bandit_arms.v2";
const char kEpsilonGreedyBanditEligibleSegments[] =
    "hns.hns_ads.epsilon_greedy_bandit_eligible_segments.v2";

// Rewards
const char kNextTokenRedemptionAt[] =
    "hns.hns_ads.rewards.next_time_redemption_at";

// Stores migration status
const char kHasMigratedClientState[] =
    "hns.hns_ads.state.has_migrated.client.v5";
const char kHasMigratedConfirmationState[] =
    "hns.hns_ads.state.has_migrated.confirmations.v5";
const char kHasMigratedConversionState[] =
    "hns.hns_ads.migrated.conversion_state";
const char kHasMigratedNotificationState[] =
    "hns.hns_ads.has_migrated.notification_state";
const char kHasMigratedRewardsState[] =
    "hns.hns_ads.migrated.rewards_state";
const char kShouldMigrateVerifiedRewardsUser[] =
    "hns.hns_ads.rewards.verified_user.should_migrate";

}  // namespace hns_ads::prefs
