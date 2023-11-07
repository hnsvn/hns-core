/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/common/pref_names.h"

namespace hns_rewards {
namespace prefs {

const char kDisabledByPolicy[] = "hns.rewards.disabled_by_policy";
const char kUserVersion[] = "hns.rewards.user_version";
const char kCurrentUserVersion[] = "2.5";
const char kHideButton[] = "hns.hide_hns_rewards_button";
const char kShowButton[] = "hns.show_hns_rewards_button";
const char kShowLocationBarButton[] =
    "hns.rewards.show_hns_rewards_button_in_location_bar";
const char kEnabled[] = "hns.rewards.enabled";
const char kDeclaredGeo[] = "hns.rewards.declared_geo";
const char kAdsEnabledTimeDelta[] = "hns.rewards.ads_enabled_time_delta";
const char kAdsEnabledTimestamp[] = "hns.rewards.ads_enabled_timestamp";
const char kNotifications[] = "hns.rewards.notifications";
const char kNotificationTimerInterval[]=
    "hns.rewards.notification_timer_interval";
const char kBackupNotificationInterval[] =
    "hns.rewards.backup_notification_interval";
const char kBackupSucceeded[] = "hns.rewards.backup_succeeded";
const char kUserHasFunded[] = "hns.rewards.user_has_funded";
const char kUserHasClaimedGrant[] = "hns.rewards.user_has_claimed_grant";
const char kAddFundsNotification[] = "hns.rewards.add_funds_notification";
const char kNotificationStartupDelay[] =
    "hns.rewards.notification_startup_delay";
const char kExternalWallets[] = "hns.rewards.external_wallets";
const char kServerPublisherListStamp[] =
    "hns.rewards.publisher_prefix_list_stamp";
const char kUpholdAnonAddress[] =
    "hns.rewards.uphold_anon_address";
const char kBadgeText[] = "hns.rewards.badge_text";
const char kUseRewardsStagingServer[] = "hns.rewards.use_staging_server";
const char kExternalWalletType[] = "hns.rewards.external_wallet_type";
const char kPromotionLastFetchStamp[] =
    "hns.rewards.promotion_last_fetch_stamp";
const char kPromotionCorruptedMigrated[] =
    "hns.rewards.promotion_corrupted_migrated2";
const char kAnonTransferChecked[] =  "hns.rewards.anon_transfer_checked";
const char kVersion[] =  "hns.rewards.version";
const char kMinVisitTime[] = "hns.rewards.ac.min_visit_time";
const char kMinVisits[] = "hns.rewards.ac.min_visits";
const char kAllowNonVerified[] =  "hns.rewards.ac.allow_non_verified";
const char kAllowVideoContribution[] =
    "hns.rewards.ac.allow_video_contributions";
const char kScoreA[] = "hns.rewards.ac.score.a";
const char kScoreB[] = "hns.rewards.ac.score.b";
const char kAutoContributeEnabled[] = "hns.rewards.ac.enabled";
const char kAutoContributeAmount[] = "hns.rewards.ac.amount";
const char kNextReconcileStamp[] = "hns.rewards.ac.next_reconcile_stamp";
const char kCreationStamp[] = "hns.rewards.creation_stamp";
const char kRecoverySeed[] = "hns.rewards.wallet.seed";
const char kPaymentId[] = "hns.rewards.wallet.payment_id";
const char kInlineTipButtonsEnabled[] =
    "hns.rewards.inline_tip_buttons_enabled";
const char kInlineTipRedditEnabled[] = "hns.rewards.inline_tip.reddit";
const char kInlineTipTwitterEnabled[] = "hns.rewards.inline_tip.twitter";
const char kInlineTipGithubEnabled[] = "hns.rewards.inline_tip.github";
const char kParametersRate[] = "hns.rewards.parameters.rate";
const char kParametersAutoContributeChoice[] =
    "hns.rewards.parameters.ac.choice";
const char kParametersAutoContributeChoices[] =
    "hns.rewards.parameters.ac.choices";
const char kParametersTipChoices[] =
    "hns.rewards.parameters.tip.choices";
const char kParametersMonthlyTipChoices[] =
    "hns.rewards.parameters.tip.monthly_choices";
const char kParametersPayoutStatus[] = "hns.rewards.parameters.payout_status";
const char kParametersWalletProviderRegions[] =
    "hns.rewards.parameters.wallet_provider_regions";
const char kParametersVBatDeadline[] = "hns.rewards.parameters.vbat_deadline";
const char kParametersVBatExpired[] = "hns.rewards.parameters.vbat_expired";
const char kFetchOldBalance[] =
    "hns.rewards.fetch_old_balance";
const char kEmptyBalanceChecked[] =
    "hns.rewards.empty_balance_checked";
const char kWalletHns[] =
    "hns.rewards.wallets.hns";
const char kWalletUphold[] =
    "hns.rewards.wallets.uphold";
const char kWalletBitflyer[] = "hns.rewards.wallets.bitflyer";
const char kWalletGemini[] = "hns.rewards.wallets.gemini";
const char kWalletZebPay[] = "hns.rewards.wallets.zebpay";
const char kWalletCreationEnvironment[] =
    "hns.rewards.wallet_creation_environment";
const char kAdsWereDisabled[] = "hns.hns_ads.were_disabled";
const char kHasAdsP3AState[] = "hns.hns_ads.has_p3a_state";

}  // namespace prefs
}  // namespace hns_rewards
