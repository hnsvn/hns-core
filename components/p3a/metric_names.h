/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_P3A_METRIC_NAMES_H_
#define HNS_COMPONENTS_P3A_METRIC_NAMES_H_

#include "base/containers/fixed_flat_set.h"
#include "base/strings/string_piece.h"

namespace p3a {

// Allowlist for histograms that we collect.
// A metric must be listed here to be reported.
//
// Please keep them properly sorted within their categories.
//
// Could be replaced with something built dynamically,
// although so far we've found it a useful review point.
//
// TODO(iefremov) Clean up obsolete metrics.
//
// clang-format off
constexpr inline auto kCollectedTypicalHistograms =
  base::MakeFixedFlatSet<base::StringPiece>({
    "Hns.AIChat.AvgPromptCount",
    "Hns.AIChat.ChatCount",
    "Hns.AIChat.Enabled",
    "Hns.Core.BookmarksCountOnProfileLoad.2",
    "Hns.Core.CrashReportsEnabled",
    "Hns.Core.DomainsLoaded",
    "Hns.Core.IsDefault",
    "Hns.Core.LastTimeIncognitoUsed",
    "Hns.Core.NumberOfExtensions",
    "Hns.Core.PagesLoaded",
    "Hns.Core.TabCount",
    "Hns.Core.TorEverUsed",
    "Hns.Core.WeeklyUsage",
    "Hns.Core.WindowCount.2",
    "Hns.Importer.ImporterSource.2",
    "Hns.NTP.CustomizeUsageStatus",
    "Hns.NTP.NewTabsCreated.2",
    "Hns.NTP.SponsoredImagesEnabled",
    "Hns.NTP.SponsoredNewTabsCreated",
    "Hns.Omnibox.SearchCount",
    "Hns.P3A.SentAnswersCount",
    "Hns.Playlist.FirstTimeOffset",
    "Hns.Playlist.NewUserReturning",
    "Hns.Playlist.UsageDaysInWeek",
    "Hns.Rewards.AdTypesEnabled",
    "Hns.Rewards.AutoContributionsState.3",
    "Hns.Rewards.EnabledSource",
    "Hns.Rewards.InlineTipTrigger",
    "Hns.Rewards.TipsState.2",
    "Hns.Rewards.ToolbarButtonTrigger",
    "Hns.Rewards.WalletBalance.3",
    "Hns.Rewards.WalletState",
    "Hns.Savings.BandwidthSavingsMB",
    "Hns.Search.DefaultEngine.4",
    "Hns.Search.Promo.BannerA",
    "Hns.Search.Promo.BannerB",
    "Hns.Search.Promo.BannerC",
    "Hns.Search.Promo.BannerD",
    "Hns.Search.Promo.Button",
    "Hns.Search.Promo.NewTabPage",
    "Hns.Search.SwitchEngine",
    "Hns.Search.WebDiscoveryEnabled",
    "Hns.Shields.AdBlockSetting",
    "Hns.Shields.CookieListEnabled",
    "Hns.Shields.CookieListPrompt",
    "Hns.Shields.DomainAdsSettingsAboveGlobal",
    "Hns.Shields.DomainAdsSettingsBelowGlobal",
    "Hns.Shields.DomainFingerprintSettingsAboveGlobal",
    "Hns.Shields.DomainFingerprintSettingsBelowGlobal",
    "Hns.Shields.FingerprintBlockSetting",
    "Hns.Shields.UsageStatus",
    "Hns.Sidebar.Enabled",
    "Hns.SpeedReader.Enabled",
    "Hns.SpeedReader.ToggleCount",
    "Hns.Today.DirectFeedsTotal",
    "Hns.Today.LastUsageTime",
    "Hns.Today.NewUserReturning",
    "Hns.Today.WeeklyAddedDirectFeedsCount",
    "Hns.Today.WeeklyDisplayAdsViewedCount",
    "Hns.Today.WeeklySessionCount",
    "Hns.Today.WeeklyTotalCardViews",
    "Hns.Toolbar.FrequentMenuGroup",
    "Hns.Toolbar.MenuDismissRate",
    "Hns.Toolbar.MenuOpens",
    "Hns.Sync.Status.2",
    "Hns.Sync.ProgressTokenEverReset",
    "Hns.VerticalTabs.GroupTabs",
    "Hns.VerticalTabs.OpenTabs",
    "Hns.VerticalTabs.PinnedTabs",
    "Hns.VPN.NewUserReturning",
    "Hns.VPN.DaysInMonthUsed",
    "Hns.VPN.LastUsageTime",
    "Hns.Wallet.ActiveEthAccounts",
    "Hns.Wallet.ActiveFilAccounts",
    "Hns.Wallet.ActiveSolAccounts",
    "Hns.Wallet.EthProvider.4",
    "Hns.Wallet.EthTransactionSent",
    "Hns.Wallet.FilTransactionSent",
    "Hns.Wallet.KeyringCreated",
    "Hns.Wallet.LastUsageTime",
    "Hns.Wallet.NewUserBalance",
    "Hns.Wallet.NewUserReturning",
    "Hns.Wallet.NFTCount",
    "Hns.Wallet.NFTDiscoveryEnabled",
    "Hns.Wallet.NFTNewUser",
    "Hns.Wallet.OnboardingConversion.3",
    "Hns.Wallet.SolProvider.2",
    "Hns.Wallet.SolTransactionSent",
    "Hns.Wallet.UsageWeekly",
    "Hns.Welcome.InteractionStatus.2",

    // IPFS
    "Hns.IPFS.IPFSCompanionInstalled",
    "Hns.IPFS.DetectionPromptCount",
    "Hns.IPFS.GatewaySetting",
    "Hns.IPFS.DaemonRunTime",
    "Hns.IPFS.LocalNodeRetention",

    // P2A
    // Ad Opportunities
    "Hns.P2A.TotalAdOpportunities",
    "Hns.P2A.AdOpportunitiesPerSegment.architecture",
    "Hns.P2A.AdOpportunitiesPerSegment.artsentertainment",
    "Hns.P2A.AdOpportunitiesPerSegment.automotive",
    "Hns.P2A.AdOpportunitiesPerSegment.business",
    "Hns.P2A.AdOpportunitiesPerSegment.careers",
    "Hns.P2A.AdOpportunitiesPerSegment.cellphones",
    "Hns.P2A.AdOpportunitiesPerSegment.crypto",
    "Hns.P2A.AdOpportunitiesPerSegment.education",
    "Hns.P2A.AdOpportunitiesPerSegment.familyparenting",
    "Hns.P2A.AdOpportunitiesPerSegment.fashion",
    "Hns.P2A.AdOpportunitiesPerSegment.folklore",
    "Hns.P2A.AdOpportunitiesPerSegment.fooddrink",
    "Hns.P2A.AdOpportunitiesPerSegment.gaming",
    "Hns.P2A.AdOpportunitiesPerSegment.healthfitness",
    "Hns.P2A.AdOpportunitiesPerSegment.history",
    "Hns.P2A.AdOpportunitiesPerSegment.hobbiesinterests",
    "Hns.P2A.AdOpportunitiesPerSegment.home",
    "Hns.P2A.AdOpportunitiesPerSegment.law",
    "Hns.P2A.AdOpportunitiesPerSegment.military",
    "Hns.P2A.AdOpportunitiesPerSegment.other",
    "Hns.P2A.AdOpportunitiesPerSegment.personalfinance",
    "Hns.P2A.AdOpportunitiesPerSegment.pets",
    "Hns.P2A.AdOpportunitiesPerSegment.realestate",
    "Hns.P2A.AdOpportunitiesPerSegment.science",
    "Hns.P2A.AdOpportunitiesPerSegment.sports",
    "Hns.P2A.AdOpportunitiesPerSegment.technologycomputing",
    "Hns.P2A.AdOpportunitiesPerSegment.travel",
    "Hns.P2A.AdOpportunitiesPerSegment.weather",
    "Hns.P2A.AdOpportunitiesPerSegment.untargeted",
    // Ad Impressions
    "Hns.P2A.TotalAdImpressions",
    "Hns.P2A.AdImpressionsPerSegment.architecture",
    "Hns.P2A.AdImpressionsPerSegment.artsentertainment",
    "Hns.P2A.AdImpressionsPerSegment.automotive",
    "Hns.P2A.AdImpressionsPerSegment.business",
    "Hns.P2A.AdImpressionsPerSegment.careers",
    "Hns.P2A.AdImpressionsPerSegment.cellphones",
    "Hns.P2A.AdImpressionsPerSegment.crypto",
    "Hns.P2A.AdImpressionsPerSegment.education",
    "Hns.P2A.AdImpressionsPerSegment.familyparenting",
    "Hns.P2A.AdImpressionsPerSegment.fashion",
    "Hns.P2A.AdImpressionsPerSegment.folklore",
    "Hns.P2A.AdImpressionsPerSegment.fooddrink",
    "Hns.P2A.AdImpressionsPerSegment.gaming",
    "Hns.P2A.AdImpressionsPerSegment.healthfitness",
    "Hns.P2A.AdImpressionsPerSegment.history",
    "Hns.P2A.AdImpressionsPerSegment.hobbiesinterests",
    "Hns.P2A.AdImpressionsPerSegment.home",
    "Hns.P2A.AdImpressionsPerSegment.law",
    "Hns.P2A.AdImpressionsPerSegment.military",
    "Hns.P2A.AdImpressionsPerSegment.other",
    "Hns.P2A.AdImpressionsPerSegment.personalfinance",
    "Hns.P2A.AdImpressionsPerSegment.pets",
    "Hns.P2A.AdImpressionsPerSegment.realestate",
    "Hns.P2A.AdImpressionsPerSegment.science",
    "Hns.P2A.AdImpressionsPerSegment.sports",
    "Hns.P2A.AdImpressionsPerSegment.technologycomputing",
    "Hns.P2A.AdImpressionsPerSegment.travel",
    "Hns.P2A.AdImpressionsPerSegment.weather",
    "Hns.P2A.AdImpressionsPerSegment.untargeted"
});

constexpr inline auto kCollectedSlowHistograms =
  base::MakeFixedFlatSet<base::StringPiece>({
    "Hns.Accessibility.DisplayZoomEnabled",
    "Hns.Core.DocumentsDirectorySizeMB",
    "Hns.Core.ProfileCount",
    "Hns.Core.UsageMonthly",
    "Hns.General.BottomBarLocation",
    "Hns.P3A.TestSlowMetric",
    "Hns.Playlist.LastUsageTime",
    "Hns.PrivacyHub.IsEnabled",
    "Hns.PrivacyHub.Views",
    "Hns.ReaderMode.NumberReaderModeActivated",
    "Hns.Rewards.TipsSent.2",
    "Hns.Sync.EnabledTypes",
    "Hns.Sync.SyncedObjectsCount",
    "Hns.Today.UsageMonthly",
    "Hns.Toolbar.ForwardNavigationAction",
    "Hns.Wallet.UsageMonthly"
});

constexpr inline auto kCollectedExpressHistograms =
  base::MakeFixedFlatSet<base::StringPiece>({
    "Hns.AIChat.UsageDaily",
    "Hns.Core.UsageDaily",
    "Hns.Rewards.EnabledInstallationTime",
    "Hns.Today.IsEnabled",
    "Hns.Today.UsageDaily",
    "Hns.Uptime.BrowserOpenTime",
    "Hns.Wallet.UsageDaily"
});

// List of metrics that should only be sent once per latest histogram update.
// Once the metric value has been sent, the value will be removed from the log store.
constexpr inline auto kEphemeralHistograms =
  base::MakeFixedFlatSet<base::StringPiece>({
    "Hns.AIChat.AvgPromptCount",
    "Hns.AIChat.ChatCount",
    "Hns.AIChat.UsageDaily",
    "Hns.Playlist.UsageDaysInWeek",
    "Hns.Playlist.FirstTimeOffset",
    "Hns.PrivacyHub.Views",
    "Hns.Rewards.EnabledInstallationTime",
    "Hns.Rewards.EnabledSource",
    "Hns.Rewards.InlineTipTrigger",
    "Hns.Rewards.TipsSent",
    "Hns.Rewards.ToolbarButtonTrigger",
    "Hns.Today.IsEnabled",
    "Hns.Today.UsageDaily",
    "Hns.Today.UsageMonthly",
    "Hns.VerticalTabs.GroupTabs",
    "Hns.VerticalTabs.OpenTabs",
    "Hns.VerticalTabs.PinnedTabs",
    "Hns.Wallet.NewUserBalance",
    "Hns.Wallet.NFTCount",
    "Hns.Wallet.NFTNewUser",
    "Hns.Wallet.OnboardingConversion.3",
    "Hns.Wallet.UsageDaily",
    "Hns.Wallet.UsageMonthly",
    "Hns.Wallet.UsageWeekly"
});

// clang-format on

}  // namespace p3a

#endif  // HNS_COMPONENTS_P3A_METRIC_NAMES_H_
