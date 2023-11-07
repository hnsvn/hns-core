/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_webui_source.h"

#include <map>
#include <string>
#include <vector>

#include "base/strings/utf_string_conversions.h"
#include "hns/components/constants/url_constants.h"
#include "hns/components/ipfs/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "build/build_config.h"
#include "chrome/browser/profiles/profile.h"
#include "components/grit/hns_components_resources.h"
#include "components/grit/hns_components_strings.h"
#include "components/grit/components_resources.h"
#include "content/public/browser/web_ui_data_source.h"
#include "services/network/public/mojom/content_security_policy.mojom.h"
#include "ui/base/l10n/l10n_util.h"
#include "ui/base/webui/resource_path.h"

#if !BUILDFLAG(IS_ANDROID)
#include "hns/browser/ui/webui/navigation_bar_data_provider.h"
#include "hns/components/hns_rewards/resources/grit/hns_rewards_resources.h"
#include "chrome/browser/ui/browser_finder.h"
#include "chrome/browser/ui/browser_window.h"
#include "chrome/browser/ui/color/chrome_color_id.h"
#include "chrome/grit/chromium_strings.h"
#include "content/public/browser/web_contents.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/color/color_provider.h"
#include "ui/color/color_provider_utils.h"
#endif

namespace {

struct WebUISimpleItem {
  const char* name;
  int id;
};

void AddLocalizedStringsBulk(content::WebUIDataSource* html_source,
                             const std::vector<WebUISimpleItem>& simple_items) {
  for (auto simple_item : simple_items) {
    html_source->AddLocalizedString(simple_item.name, simple_item.id);
  }
}

void AddResourcePaths(content::WebUIDataSource* html_source,
                      const std::vector<WebUISimpleItem>& simple_items) {
  for (auto simple_item : simple_items) {
    html_source->AddResourcePath(simple_item.name, simple_item.id);
  }
}

void CustomizeWebUIHTMLSource(content::WebUI* web_ui,
                              const std::string& name,
                              content::WebUIDataSource* source) {
#if !BUILDFLAG(IS_ANDROID)
  if (name == "rewards" || name == "wallet") {
    NavigationBarDataProvider::Initialize(source, Profile::FromWebUI(web_ui));
  }
#endif

  // clang-format off
  static std::map<std::string, std::vector<WebUISimpleItem> > resources = {
#if !BUILDFLAG(IS_ANDROID)
    {
      std::string("newtab"), {
        { "img/toolbar/menu_btn.svg", IDR_HNS_COMMON_TOOLBAR_IMG },
        // Hash path is the MD5 of the file contents,
        // webpack image loader does this
        { "fd85070af5114d6ac462c466e78448e4.svg", IDR_HNS_NEW_TAB_IMG1 },
        { "314e7529efec41c8867019815f4d8dad.svg", IDR_HNS_NEW_TAB_IMG4 },
        { "6c337c63662ee0ba4e57f6f8156d69ce.svg", IDR_HNS_NEW_TAB_IMG2 },
        // New tab Backgrounds
        { "dylan-malval_sea-min.webp", IDR_HNS_NEW_TAB_BACKGROUND1 },
        // private tab
        { "c168145d6bf1abf2c0322636366f7dbe.svg", IDR_HNS_PRIVATE_TAB_TOR_IMG },               // NOLINT
        { "dbdc336ccc651b8a7c925b3482d6e65a.svg", IDR_HNS_PRIVATE_TAB_IMG }
    }
    }, {
      std::string("rewards"), {
        { "favicon.ico", IDR_HNS_REWARDS_FAVICON },
      }
    }, {
      std::string("welcome"), {
        { "favicon.ico", IDR_HNS_WELCOME_PAGE_FAVICON }
      }
    },
#endif
    {
      std::string("adblock"), {}
    }
  };
  AddResourcePaths(source, resources[name]);

  // clang-format off
  static std::map<std::string, std::vector<WebUISimpleItem> >
                                                           localized_strings = {
    {
      std::string("newtab"), {
        { "adsTrackersBlocked", IDS_HNS_NEW_TAB_TOTAL_ADS_TRACKERS_BLOCKED },
        { "httpsUpgraded", IDS_HNS_NEW_TAB_TOTAL_HTTPS_UPGRADES },
        { "estimatedTimeSaved", IDS_HNS_NEW_TAB_TOTAL_TIME_SAVED },
        { "estimatedBandwidthSaved",
            IDS_HNS_NEW_TAB_ESTIMATED_BANDWIDTH_SAVED },
        { "thumbRemoved", IDS_HNS_NEW_TAB_THUMB_REMOVED },
        { "undoRemoved", IDS_HNS_NEW_TAB_UNDO_REMOVED },
        { "close", IDS_HNS_NEW_TAB_CLOSE },
        { "restoreAll", IDS_HNS_NEW_TAB_RESTORE_ALL },
        { "second", IDS_HNS_NEW_TAB_SECOND },
        { "seconds", IDS_HNS_NEW_TAB_SECONDS },
        { "minute", IDS_HNS_NEW_TAB_MINUTE },
        { "minutes", IDS_HNS_NEW_TAB_MINUTES },
        { "hour", IDS_HNS_NEW_TAB_HOUR },
        { "hours", IDS_HNS_NEW_TAB_HOURS },
        { "day", IDS_HNS_NEW_TAB_DAY },
        { "days", IDS_HNS_NEW_TAB_DAYS },
        { "B", IDS_HNS_NEW_TAB_BYTES },
        { "KB", IDS_HNS_NEW_TAB_KILOBYTES },
        { "MB", IDS_HNS_NEW_TAB_MEGABYTES },
        { "GB", IDS_HNS_NEW_TAB_GIGABYTES },
        { "photoBy", IDS_HNS_NEW_TAB_PHOTO_BY },
        { "hide", IDS_HNS_NEW_TAB_HIDE },
        { "preferencesPageTitle", IDS_HNS_NEW_TAB_PREFERENCES_PAGE_TITLE },
        { "bookmarksPageTitle", IDS_HNS_NEW_TAB_BOOKMARKS_PAGE_TITLE },
        { "historyPageTitle", IDS_HNS_NEW_TAB_HISTORY_PAGE_TITLE },
        { "dashboardSettingsTitle",
            IDS_HNS_NEW_TAB_DASHBOARD_SETTINGS_TITLE },
        { "customize", IDS_HNS_NEW_TAB_CUSTOMIZE },
        { "showBackgroundImage", IDS_HNS_NEW_TAB_SHOW_BACKGROUND_IMAGE },
        { "hnsBackgroundImageOptionTitle", IDS_HNS_NEW_TAB_HNS_BACKGROUND_IMAGE_OPTION_TITLE },  // NOLINT
        { "customBackgroundImageOptionTitle", IDS_HNS_NEW_TAB_CUSTOM_BACKGROUND_IMAGE_OPTION_TITLE },  // NOLINT
        { "customBackgroundImageOptionUploadLabel", IDS_HNS_NEW_TAB_CUSTOM_BACKGROUND_IMAGE_OPTION_UPLOAD_LABEL },  // NOLINT
        { "showHnsStats", IDS_HNS_NEW_TAB_SHOW_HNS_STATS },
        { "showClock", IDS_HNS_NEW_TAB_SHOW_CLOCK },
        { "clockFormat", IDS_HNS_NEW_TAB_CLOCK_FORMAT },
        { "clockFormatDefault", IDS_HNS_NEW_TAB_CLOCK_FORMAT_DEFAULT },
        { "clockFormat12", IDS_HNS_NEW_TAB_CLOCK_FORMAT_12 },
        { "clockFormat24", IDS_HNS_NEW_TAB_CLOCK_FORMAT_24 },
        { "addTopSiteDialogTitle", IDS_HNS_NEW_TAB_ADD_TOP_SITE_DIALOG_TITLE },  // NOLINT
        { "editTopSiteDialogTitle", IDS_HNS_NEW_TAB_EDIT_TOP_SITE_DIALOG_TITLE },  // NOLINT
        { "editSiteTileMenuItem", IDS_HNS_NEW_TAB_EDIT_SITE_TILE_MENU_ITEM },
        { "removeTileMenuItem", IDS_HNS_NEW_TAB_REMOVE_TILE_MENU_ITEM },
        { "addTopSiteDialogURLLabel", IDS_HNS_NEW_TAB_ADD_TOP_SITE_DIALOG_URL_LABEL },  // NOLINT
        { "addTopSiteDialogURLInputPlaceHolder", IDS_HNS_NEW_TAB_ADD_TOP_SITE_DIALOG_URL_INPUT_PLACEHOLDER },  // NOLINT
        { "addTopSiteDialogNameLabel", IDS_HNS_NEW_TAB_ADD_TOP_SITE_DIALOG_NAME_LABEL },  // NOLINT
        { "addTopSiteDialogNameInputPlaceHolder", IDS_HNS_NEW_TAB_ADD_TOP_SITE_DIALOG_NAME_INPUT_PLACEHOLDER },  // NOLINT
        { "addTopSiteDialogSaveButtonLabel", IDS_HNS_NEW_TAB_ADD_TOP_SITE_DIALOG_SAVE_BUTTON_LABEL },  // NOLINT
        { "addTopSiteDialogCancelButtonLabel", IDS_HNS_NEW_TAB_ADD_TOP_SITE_DIALOG_CANCEL_BUTTON_LABEL },  // NOLINT
        { "showTopSites", IDS_HNS_NEW_TAB_SHOW_TOP_SITES },
        { "showFavoritesLabel", IDS_HNS_NEW_TAB_SHOW_FAVORITES_LABEL },
        { "showFavoritesDesc", IDS_HNS_NEW_TAB_SHOW_FAVORITES_DESC },
        { "showFrecencyLabel", IDS_HNS_NEW_TAB_SHOW_FRECENCY_LABEL },
        { "showFrecencyDesc", IDS_HNS_NEW_TAB_SHOW_FRECENCY_DESC },
        { "addSiteMenuLabel", IDS_HNS_NEW_TAB_ADD_SITE_MENU_LABEL },
        { "showFrecencyMenuLabel", IDS_HNS_NEW_TAB_SHOW_FRECENCY_MENU_LABEL },
        { "showFavoritesMenuLabel", IDS_HNS_NEW_TAB_SHOW_FAVORITES_MENU_LABEL },  // NOLINT
        { "showRewards", IDS_HNS_NEW_TAB_SHOW_REWARDS },
        { "cards", IDS_HNS_NEW_TAB_SHOW_CARDS },
        { "brandedWallpaperOptIn", IDS_HNS_NEW_TAB_BRANDED_WALLPAPER_OPT_IN },
        { "sponsoredImageEarningTitle", IDS_HNS_NEW_TAB_SPONSORED_IMAGE_EARNING_TITLE},
        { "sponsoredImageEnableRewards", IDS_HNS_NEW_TAB_SPONSORED_IMAGE_ENABLE_REWARDS},
        { "sponsoredImageNotEarningTitle", IDS_HNS_NEW_TAB_SPONSORED_IMAGE_NOT_EARNING_TITLE},
        { "sponsoredImageOffRewardsOnDescription", IDS_HNS_NEW_TAB_SPONSORED_IMAGE_OFF_REWARDS_ON_DESCRPTION},  // NOLINT
        { "sponsoredImageOnDescription", IDS_HNS_NEW_TAB_SPONSORED_IMAGE_ON_DESCRIPTION},  // NOLINT
        { "sponsoredImageOnRewardsOnNoCustodianDescription", IDS_HNS_NEW_TAB_SPONSORED_IMAGE_ON_REWARDS_ON_NO_CUSTODIAN_DESCRIPTION},  // NOLINT
        { "sponsoredImageRewardsOffDescription", IDS_HNS_NEW_TAB_SPONSORED_IMAGE_REWARDS_OFF_DESCRIPTION},  // NOLINT
        { "topSitesTitle", IDS_HNS_NEW_TAB_TOP_SITES },
        { "statsTitle", IDS_HNS_NEW_TAB_STATS },
        { "clockTitle", IDS_HNS_NEW_TAB_CLOCK },
        { "backgroundImageTitle", IDS_HNS_NEW_TAB_BACKGROUND_IMAGE },
        { "settingsNavigateBack", IDS_HNS_NEW_TAB_SETTINGS_BACK },
        { "hnsBackgroundsTitle", IDS_HNS_NEW_TAB_HNS_BACKGROUND},
        { "solidColorTitle", IDS_HNS_NEW_TAB_SOLID_COLOR},
        { "gradientColorTitle", IDS_HNS_NEW_TAB_GRADIENT_COLOR},
        { "refreshBackgroundOnNewTab", IDS_HNS_NEW_TAB_REFRESH_BACKGROUND_ON_NEW_TAB},  // NOLINT
        { "rewardsOpenPanel", IDS_HNS_NEW_TAB_REWARDS_OPEN_PANEL },

        // search promotion
        { "searchPromotionNTPPopupTitle1", IDS_HNS_NEW_TAB_SEARCH_PROMOTION_POPUP_TITLE_1},  // NOLINT
        { "searchPromotionNTPPopupTitle2", IDS_HNS_NEW_TAB_SEARCH_PROMOTION_POPUP_TITLE_2},  // NOLINT
        { "searchPromotionNTPPopupDesc", IDS_HNS_NEW_TAB_SEARCH_PROMOTION_POPUP_DESC},       // NOLINT
        { "searchPromotionNTPPopupBottom", IDS_HNS_NEW_TAB_SEARCH_PROMOTION_POPUP_BOTTOM},   // NOLINT
        { "searchPromotionSearchBoxPlaceholderLabel", IDS_HNS_NEW_TAB_SEARCH_PROMOTION_SEARCH_BOX_PLACEHOLDER},  // NOLINT

        { "hnsNewsTitle", IDS_HNS_NEWS_TITLE },
        { "hnsNewsStatusFetching", IDS_HNS_NEWS_STATUS_FETCHING},
        { "hnsNewsActionRefresh", IDS_HNS_NEWS_ACTION_REFRESH},
        { "hnsNewsScrollHint", IDS_HNS_NEWS_SCROLL_HINT},
        { "hnsNewsResetAction", IDS_HNS_NEWS_RESET_ACTION},
        { "hnsNewsResetConfirm", IDS_HNS_NEWS_RESET_CONFIRM},
        { "hnsNewsCategoryNameAll", IDS_HNS_NEWS_CATEGORY_NAME_ALL},
        { "hnsNewsSourcesTitle", IDS_HNS_NEWS_SOURCES_TITLE},
        { "hnsNewsDisableSourceCommand",
            IDS_HNS_NEWS_DISABLE_SOURCE_COMMAND},
        { "promoted", IDS_HNS_NEWS_PROMOTED },
        { "ad", IDS_HNS_NEWS_DISPLAY_AD_LABEL },

        { "hnsNewsIntroTitle", IDS_HNS_NEWS_INTRO_TITLE },
        { "hnsNewsIntroDescription", IDS_HNS_NEWS_INTRO_DESCRIPTION },
        { "hnsNewsIntroDescriptionTwo", IDS_HNS_NEWS_INTRO_DESCRIPTION_TWO },  // NOLINT
        { "hnsNewsOptInActionLabel", IDS_HNS_NEWS_OPT_IN_ACTION_LABEL },
        { "hnsNewsOptOutActionLabel", IDS_HNS_NEWS_OPT_OUT_ACTION_LABEL },
        { "hnsNewsBackToDashboard", IDS_HNS_NEWS_BACK_TO_DASHBOARD },
        { "hnsNewsBackButton", IDS_HNS_NEWS_BACK_BUTTON },
        { "hnsNewsSearchPlaceholderLabel", IDS_HNS_NEWS_SEARCH_PLACEHOLDER_LABEL},  // NOLINT
        { "hnsNewsChannelsHeader", IDS_HNS_NEWS_BROWSE_CHANNELS_HEADER},  // NOLINT
        { "hnsNewsViewAllButton", IDS_HNS_NEWS_VIEW_ALL_BUTTON},
        { "hnsNewsAllSourcesHeader", IDS_HNS_NEWS_ALL_SOURCES_HEADER},
        { "hnsNewsFeedsHeading", IDS_HNS_NEWS_FEEDS_HEADING},
        { "hnsNewsFollowButtonFollowing", IDS_HNS_NEWS_FOLLOW_BUTTON_FOLLOWING},  // NOLINT
        { "hnsNewsFollowButtonNotFollowing", IDS_HNS_NEWS_FOLLOW_BUTTON_NOT_FOLLOWING},  // NOLINT
        { "hnsNewsDirectSearchButton", IDS_HNS_NEWS_DIRECT_SEARCH_BUTTON},  // NOLINT
        { "hnsNewsDirectSearchNoResults", IDS_HNS_NEWS_DIRECT_SEARCH_NO_RESULTS},  // NOLINT
        { "hnsNewsSearchResultsNoResults", IDS_HNS_NEWS_SEARCH_RESULTS_NO_RESULTS},  // NOLINT
        { "hnsNewsSearchResultsLocalResults", IDS_HNS_NEWS_SEARCH_RESULTS_LOCAL_RESULTS},  // NOLINT
        { "hnsNewsSearchResultsDirectResults", IDS_HNS_NEWS_SEARCH_RESULTS_DIRECT_RESULTS},  // NOLINT
        { "hnsNewsSearchQueryTooShort", IDS_HNS_NEWS_SEARCH_QUERY_TOO_SHORT},  // NOLINT
        { "hnsNewsSuggestionsTitle", IDS_HNS_NEWS_SUGGESTIONS_TITLE},
        { "hnsNewsSuggestionsSubtitle", IDS_HNS_NEWS_SUGGESTIONS_SUBTITLE},
        { "hnsNewsErrorHeading", IDS_HNS_NEWS_ERROR_HEADING},
        { "hnsNewsErrorMessage", IDS_HNS_NEWS_ERROR_MESSAGE},
        { "hnsNewsErrorActionLabel", IDS_HNS_NEWS_ERROR_ACTION_LABEL},
        { "hnsNewsNoContentHeading", IDS_HNS_NEWS_NO_CONTENT_HEADING},
        { "hnsNewsNoContentMessage", IDS_HNS_NEWS_NO_CONTENT_MESSAGE},
        { "hnsNewsNoContentActionLabel", IDS_HNS_NEWS_NO_CONTENT_ACTION_LABEL},  // NOLINT
        // Hns News Channels
        { "hnsNewsChannel-Hns", IDS_HNS_NEWS_CHANNEL_HNS},
        { "hnsNewsChannel-Business", IDS_HNS_NEWS_CHANNEL_BUSINESS},
        { "hnsNewsChannel-Cars", IDS_HNS_NEWS_CHANNEL_CARS},
        { "hnsNewsChannel-Crypto", IDS_HNS_NEWS_CHANNEL_CRYPTO},
        { "hnsNewsChannel-Culture", IDS_HNS_NEWS_CHANNEL_CULTURE},
        { "hnsNewsChannel-Entertainment", IDS_HNS_NEWS_CHANNEL_ENTERTAINMENT}, // NOLINT
        { "hnsNewsChannel-Entertainment News", IDS_HNS_NEWS_CHANNEL_ENTERTAINMENT_NEWS}, // NOLINT
        { "hnsNewsChannel-Fashion", IDS_HNS_NEWS_CHANNEL_FASHION},
        { "hnsNewsChannel-Film and TV", IDS_HNS_NEWS_CHANNEL_FILM_AND_TV},
        { "hnsNewsChannel-Food", IDS_HNS_NEWS_CHANNEL_FOOD},
        { "hnsNewsChannel-Fun", IDS_HNS_NEWS_CHANNEL_FUN},
        { "hnsNewsChannel-Gaming", IDS_HNS_NEWS_CHANNEL_GAMING},
        { "hnsNewsChannel-Health", IDS_HNS_NEWS_CHANNEL_HEALTH},
        { "hnsNewsChannel-Home", IDS_HNS_NEWS_CHANNEL_HOME},
        { "hnsNewsChannel-Music", IDS_HNS_NEWS_CHANNEL_MUSIC},
        { "hnsNewsChannel-Politics", IDS_HNS_NEWS_CHANNEL_POLITICS},
        { "hnsNewsChannel-Regional News", IDS_HNS_NEWS_CHANNEL_REGIONAL_NEWS},
        { "hnsNewsChannel-Science", IDS_HNS_NEWS_CHANNEL_SCIENCE},
        { "hnsNewsChannel-Sports", IDS_HNS_NEWS_CHANNEL_SPORTS},
        { "hnsNewsChannel-Travel", IDS_HNS_NEWS_CHANNEL_TRAVEL},
        { "hnsNewsChannel-Technology", IDS_HNS_NEWS_CHANNEL_TECHNOLOGY},
        { "hnsNewsChannel-Tech News", IDS_HNS_NEWS_CHANNEL_TECH_NEWS},
        { "hnsNewsChannel-Tech Reviews", IDS_HNS_NEWS_CHANNEL_TECH_REVIEWS},
        { "hnsNewsChannel-Top News", IDS_HNS_NEWS_CHANNEL_TOP_NEWS},
        { "hnsNewsChannel-Top Sources", IDS_HNS_NEWS_CHANNEL_TOP_SOURCES},
        { "hnsNewsChannel-US News", IDS_HNS_NEWS_CHANNEL_US_NEWS},
        { "hnsNewsChannel-Weather", IDS_HNS_NEWS_CHANNEL_WEATHER},
        { "hnsNewsChannel-World News", IDS_HNS_NEWS_CHANNEL_WORLD_NEWS},
        { "hnsNewsPopularTitle", IDS_HNS_NEWS_POPULAR_TITLE},


        { "addWidget", IDS_HNS_NEW_TAB_WIDGET_ADD },
        { "hideWidget", IDS_HNS_NEW_TAB_WIDGET_HIDE },
        { "rewardsWidgetDesc", IDS_HNS_NEW_TAB_REWARDS_WIDGET_DESC },
        { "cardsToggleTitle", IDS_HNS_NEW_TAB_CARDS_TITLE },
        { "cardsToggleDesc", IDS_HNS_NEW_TAB_CARDS_DESC },
        { "editCardsTitle", IDS_EDIT_CARDS_TITLE },
        { "hnsRewardsTitle", IDS_HNS_NEW_TAB_HNS_REWARDS_TITLE },
#if !BUILDFLAG(IS_ANDROID)
        // Private Tab - General
        { "learnMore", IDS_HNS_PRIVATE_NEW_TAB_LEARN_MORE },
        { "done", IDS_HNS_PRIVATE_NEW_TAB_DONE },
        { "headerLabel", IDS_HNS_PRIVATE_NEW_TAB_THIS_IS_A },

        // Private Tab - Header Private Window
        { "headerTitle", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW },
        { "headerText", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_DESC },
        { "headerText1", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_DESC1 },
        { "headerText2", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_DESC2 },
        { "headerButton", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_BUTTON },

        // Private Tab - Header Private Window with Tor
        { "headerTorTitle", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_TOR },
        { "headerTorText", IDS_HNS_PRIVATE_NEW_TAB_PRIVATE_WINDOW_TOR_DESC },

        // Private Tab - Box for DDG
        { "boxDdgButton", IDS_HNS_PRIVATE_NEW_TAB_BOX_DDG_BUTTON },

        // Private Tab - Box for Tor
        { "boxTorLabel", IDS_HNS_PRIVATE_NEW_TAB_BOX_TOR_LABEL },
        { "boxTorLabel2", IDS_HNS_PRIVATE_NEW_TAB_BOX_TOR_LABEL_2 },
        { "boxTorTitle", IDS_HNS_PRIVATE_NEW_TAB_BOX_TOR_TITLE },

        // Private Tab - Private Window with Tor - Tor Box
        { "boxTorText", IDS_HNS_PRIVATE_NEW_TAB_BOX_TOR_TEXT_1 },

        // Private Tab - Private Window - Tor Box
        { "boxTorText2", IDS_HNS_PRIVATE_NEW_TAB_BOX_TOR_TEXT_2 },
        { "boxTorButton", IDS_HNS_PRIVATE_NEW_TAB_BOX_TOR_BUTTON },
#endif  // !BUILDFLAG(IS_ANDROID)

        // Hns Talk shortcut
        { "hnsTalkPromptTitle", IDS_HNS_TALK_PROMPT_TITLE },

        // Rewards widget
        { "rewardsAboutRewards", IDS_REWARDS_WIDGET_ABOUT_REWARDS },
        { "rewardsAdGrantAmount", IDS_REWARDS_WIDGET_AD_GRANT_AMOUNT },
        { "rewardsAdGrantTitle", IDS_REWARDS_WIDGET_AD_GRANT_TITLE },
        { "rewardsBalanceInfoText", IDS_REWARDS_WIDGET_BALANCE_INFO_TEXT },
        { "rewardsHnsRewards", IDS_REWARDS_WIDGET_HNS_REWARDS },
        { "rewardsClaimRewards", IDS_REWARDS_WIDGET_CLAIM_REWARDS },
        { "rewardsClaimTokens", IDS_REWARDS_WIDGET_CLAIM_TOKENS },
        { "rewardsConnectAccount", IDS_REWARDS_CONNECT_ACCOUNT },
        { "rewardsConnectAccountNoProviders", IDS_REWARDS_CONNECT_ACCOUNT_NO_PROVIDERS },  // NOLINT
        { "rewardsConnectAccountText", IDS_REWARDS_CONNECT_ACCOUNT_TEXT },
        { "rewardsContinue", IDS_REWARDS_WIDGET_CONTINUE},
        { "rewardsEarningsTitle", IDS_REWARDS_ESTIMATED_EARNINGS_TITLE },
        { "rewardsEarningInfoText", IDS_REWARDS_WIDGET_EARNING_INFO_TEXT },
        { "rewardsHowDoesItWork", IDS_REWARDS_WIDGET_HOW_DOES_IT_WORK},
        { "rewardsGrantDaysRemaining", IDS_REWARDS_WIDGET_GRANT_DAYS_REMAINING },  // NOLINT
        { "rewardsLearnMore", IDS_REWARDS_LEARN_MORE },
        { "rewardsManageAds", IDS_REWARDS_WALLET_MANAGE_ADS },
        { "rewardsOptInHeader", IDS_REWARDS_WIDGET_OPT_IN_HEADER },
        { "rewardsOptInTerms", IDS_HNS_REWARDS_ONBOARDING_TERMS },
        { "rewardsOptInText", IDS_REWARDS_WIDGET_OPT_IN_TEXT },
        { "rewardsLogInToSeeBalance", IDS_REWARDS_LOG_IN_TO_SEE_BALANCE },
        { "rewardsPaymentCheckStatus", IDS_REWARDS_PAYMENT_CHECK_STATUS },
        { "rewardsPaymentCompleted", IDS_REWARDS_PAYMENT_COMPLETED },
        { "rewardsPaymentPending", IDS_REWARDS_PAYMENT_PENDING },
        { "rewardsPaymentProcessing", IDS_REWARDS_PAYMENT_PROCESSING },
        { "rewardsPaymentSupport", IDS_REWARDS_PAYMENT_SUPPORT },
        { "rewardsSelectCountryHeader", IDS_REWARDS_WIDGET_SELECT_COUNTRY_HEADER},  // NOLINT
        { "rewardsSelectCountryText", IDS_REWARDS_WIDGET_SELECT_COUNTRY_TEXT},
        { "rewardsSettings", IDS_REWARDS_WIDGET_SETTINGS },
        { "rewardsStartUsingRewards", IDS_REWARDS_WIDGET_START_USING_REWARDS },
        { "rewardsBalanceTitle", IDS_REWARDS_WIDGET_BALANCE_TITLE },
        { "rewardsTokenGrantTitle", IDS_REWARDS_WIDGET_TOKEN_GRANT_TITLE },
        { "rewardsWidgetHnsRewards", IDS_HNS_UI_HNS_REWARDS },
        { "rewardsBrowserCannotReceiveAds",  IDS_REWARDS_BROWSER_CANNOT_RECEIVE_ADS },  // NOLINT
        { "rewardsBrowserNeedsUpdateToSeeAds",  IDS_REWARDS_BROWSER_NEEDS_UPDATE_TO_SEE_ADS },  // NOLINT
        { "rewardsUnsupportedRegionNoticeHeader", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_HEADER},  // NOLINT
        { "rewardsUnsupportedRegionNoticeSubheader", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_SUBHEADER},  // NOLINT
        { "rewardsUnsupportedRegionNoticeLearnMore", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_LEARN_MORE},  // NOLINT
        { "rewardsUnsupportedRegionNoticeText1", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_TEXT_1},  // NOLINT
        { "rewardsUnsupportedRegionNoticeText2", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_TEXT_2},  // NOLINT
        {"rewardsVBATNoticeText1", IDS_REWARDS_VBAT_NOTICE_TEXT1},
        {"rewardsVBATNoticeText2", IDS_REWARDS_VBAT_NOTICE_TEXT2},
        {"rewardsVBATNoticeTitle1", IDS_REWARDS_VBAT_NOTICE_TITLE1},
        {"rewardsVBATNoticeTitle2", IDS_REWARDS_VBAT_NOTICE_TITLE2},

        { "loading", IDS_HNS_REWARDS_LOADING_LABEL },

        // Hns Talk  Widget
        { "hnsTalkWidgetTitle", IDS_HNS_TALK_WIDGET_TITLE },
        { "hnsTalkWidgetWelcomeTitle", IDS_HNS_TALK_WIDGET_WELCOME_TITLE },
        { "hnsTalkWidgetStartButton", IDS_HNS_TALK_WIDGET_START_BUTTON },
        { "hnsTalkWidgetAboutData", IDS_HNS_TALK_WIDGET_ABOUT_DATA },
      }
    }, {
      std::string("wallet"), {
        { "cryptoWalletsWelcome", IDS_HNS_WALLET_WELCOME },
        { "cryptoWalletsDisclosureOne", IDS_HNS_WALLET_DISCLOSURE_ONE },
        { "cryptoWalletsDisclosureTwo", IDS_HNS_WALLET_DISCLOSURE_TWO },
        { "cryptoWalletsDisclosureThree", IDS_HNS_WALLET_DISCLOSURE_THREE },
        { "cryptoWalletsDisclosureFour", IDS_HNS_WALLET_DISCLOSURE_FOUR },
        { "cryptoWalletsHnsRewards", IDS_HNS_WALLET_HNS_REWARDS },
        { "cryptoWalletsDownloading", IDS_HNS_WALLET_DOWNLOADING },
        { "cryptoWalletsDisclosureConfirm", IDS_HNS_WALLET_DISCLOSURE_CONFIRM }         // NOLINT
      }
    }, {
      std::string("rewards"), {
        { "adsCurrentEarnings",  IDS_HNS_REWARDS_LOCAL_ADS_CURRENT_EARNINGS },
        { "adsDescription",  IDS_HNS_REWARDS_LOCAL_ADS_DESCRIPTION },
        { "adsDescriptionEarn",  IDS_HNS_REWARDS_LOCAL_ADS_DESCRIPTION_EARN },
        { "adsTotalReceivedLabel",  IDS_HNS_REWARDS_LOCAL_ADS_TOTAL_RECEIVED },  // NOLINT
        { "adsNotSupportedRegion", IDS_HNS_REWARDS_LOCAL_ADS_NOT_SUPPORTED_REGION },           // NOLINT
        { "adsNotSupportedDevice", IDS_HNS_REWARDS_LOCAL_ADS_NOT_SUPPORTED_DEVICE },           // NOLINT
        { "adsPaymentDate",  IDS_HNS_REWARDS_LOCAL_ADS_PAYMENT_DATE },
        { "adsPerHour",  IDS_HNS_REWARDS_LOCAL_ADS_PER_HOUR },
        { "adsPerHour0",  IDS_HNS_REWARDS_LOCAL_ADS_PER_HOUR_0 },
        { "adsPerHour1",  IDS_HNS_REWARDS_LOCAL_ADS_PER_HOUR_1 },
        { "adsPerHour2",  IDS_HNS_REWARDS_LOCAL_ADS_PER_HOUR_2 },
        { "adsPerHour3",  IDS_HNS_REWARDS_LOCAL_ADS_PER_HOUR_3 },
        { "adsPerHour4",  IDS_HNS_REWARDS_LOCAL_ADS_PER_HOUR_4 },
        { "adsPerHour5",  IDS_HNS_REWARDS_LOCAL_ADS_PER_HOUR_5 },
        { "adsPerHour10",  IDS_HNS_REWARDS_LOCAL_ADS_PER_HOUR_10 },
        { "adsSubdivisionTargetingTitle",  IDS_HNS_REWARDS_LOCAL_ADS_SUBDIVISION_TARGETING_TITLE },  // NOLINT
        { "adsSubdivisionTargetingDescription",  IDS_HNS_REWARDS_LOCAL_ADS_SUBDIVISION_TARGETING_DESCRIPTION },  // NOLINT
        { "adsSubdivisionTargetingLearn",  IDS_HNS_REWARDS_LOCAL_ADS_SUBDIVISION_TARGETING_LEARN },  // NOLINT
        { "adsSubdivisionTargetingAutoDetectedAs",  IDS_HNS_REWARDS_LOCAL_ADS_SUBDIVISION_TARGETING_AUTO_DETECTED_AS },  // NOLINT
        { "adsSubdivisionTargetingAutoDetect",  IDS_HNS_REWARDS_LOCAL_ADS_SUBDIVISION_TARGETING_AUTO_DETECT },  // NOLINT
        { "adsSubdivisionTargetingDisable",  IDS_HNS_REWARDS_LOCAL_ADS_SUBDIVISION_TARGETING_DISABLE },  // NOLINT
        { "adsSubdivisionTargetingDisabled",  IDS_HNS_REWARDS_LOCAL_ADS_SUBDIVISION_TARGETING_DISABLED },  // NOLINT
        { "adsTitle",  IDS_HNS_REWARDS_LOCAL_ADS_TITLE },
        { "newTabAdCountLabel", IDS_HNS_REWARDS_NEW_TAB_AD_COUNT_LABEL},
        { "notificationAdCountLabel", IDS_HNS_REWARDS_NOTIFICATION_AD_COUNT_LABEL},  // NOLINT
        { "newsAdCountLabel", IDS_HNS_REWARDS_NEWS_AD_COUNT_LABEL},
        { "newsAdInfo", IDS_HNS_REWARDS_NEWS_AD_INFO },
        { "newsAdInfoDisabled", IDS_HNS_REWARDS_NEWS_AD_INFO_DISABLED },
        { "appErrorTitle", IDS_REWARDS_APP_ERROR_TITLE },

        { "bat", IDS_HNS_UI_BAT_REWARDS_TEXT },
        { "contributionTitle",  IDS_HNS_REWARDS_LOCAL_CONTR_TITLE },
        { "contributionDesc",  IDS_HNS_REWARDS_LOCAL_CONTR_DESC },
        { "contributionMonthly",  IDS_HNS_REWARDS_LOCAL_CONTR_MONTHLY },
        { "contributionNextDate",  IDS_HNS_REWARDS_LOCAL_CONTR_NEXT_DATE },
        { "contributionSites",  IDS_HNS_REWARDS_LOCAL_CONTR_SITES },
        { "contributionVisitSome",  IDS_HNS_REWARDS_LOCAL_CONTR_VISIT_SOME },
        { "contributionMinTime",  IDS_HNS_REWARDS_LOCAL_CONTR_MIN_TIME },
        { "contributionMinVisits",  IDS_HNS_REWARDS_LOCAL_CONTR_MIN_VISITS },
        { "contributionOther",  IDS_HNS_REWARDS_LOCAL_CONTR_OTHER },
        { "contributionVisit1",  IDS_HNS_REWARDS_LOCAL_CONTR_VISIT_1 },
        { "contributionVisit5",  IDS_HNS_REWARDS_LOCAL_CONTR_VISIT_5 },
        { "contributionVisit10",  IDS_HNS_REWARDS_LOCAL_CONTR_VISIT_10 },
        { "contributionTime5",  IDS_HNS_REWARDS_LOCAL_CONTR_TIME_5 },
        { "contributionTime8",  IDS_HNS_REWARDS_LOCAL_CONTR_TIME_8 },
        { "contributionTime60",  IDS_HNS_REWARDS_LOCAL_CONTR_TIME_60 },
        { "contributionUpTo",  IDS_HNS_REWARDS_LOCAL_CONTR_UP_TO },

        { "disconnectWalletFailed",  IDS_HNS_REWARDS_LOCAL_SERVER_DISCONNECT_FAILED },         // NOLINT
        { "donationTitle",  IDS_HNS_REWARDS_LOCAL_DONAT_TITLE },
        { "donationDesc",  IDS_HNS_REWARDS_LOCAL_DONAT_DESC },
        { "donationDescLearnMore",  IDS_HNS_REWARDS_LOCAL_DONAT_DESC_LEARN_MORE },             // NOLINT
        { "donationTotalDonations",  IDS_HNS_REWARDS_LOCAL_DONAT_TOTAL_DONATIONS },            // NOLINT
        { "donationVisitSome",  IDS_HNS_REWARDS_LOCAL_DONAT_VISIT_SOME },
        { "donationAbility",  IDS_HNS_REWARDS_LOCAL_DONAT_ABILITY },
        { "monthlyTipsTitle",  IDS_HNS_REWARDS_LOCAL_MONTHLY_TIPS_TITLE },
        { "monthlyTipsDesc",  IDS_HNS_REWARDS_LOCAL_MONTHLY_TIPS_DESC },
        { "monthlyTipsEmpty", IDS_HNS_REWARDS_LOCAL_MONTHLY_TIPS_EMPTY },
        { "nextContribution", IDS_HNS_REWARDS_LOCAL_NEXT_CONTRIBUTION },
        { "redirectModalError", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_ERROR },
        { "redirectModalClose", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_CLOSE },
        { "redirectModalDeviceLimitReachedText", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_DEVICE_LIMIT_REACHED_TEXT},     // NOLINT
        { "redirectModalDeviceLimitReachedTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_DEVICE_LIMIT_REACHED_TITLE},     // NOLINT
        { "redirectModalFlaggedWalletText1", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_FLAGGED_WALLET_TEXT_1},  // NOLINT
        { "redirectModalFlaggedWalletText2", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_FLAGGED_WALLET_TEXT_2},  // NOLINT
        { "redirectModalFlaggedWalletText3", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_FLAGGED_WALLET_TEXT_3},  // NOLINT
        { "redirectModalFlaggedWalletText4", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_FLAGGED_WALLET_TEXT_4},  // NOLINT
        { "redirectModalFlaggedWalletTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_FLAGGED_WALLET_TITLE},   // NOLINT
        { "redirectModalKYCRequiredTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_KYC_REQUIRED_TITLE },     // NOLINT
        { "redirectModalKYCRequiredText", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_KYC_REQUIRED_TEXT },     // NOLINT
        { "redirectModalMismatchedCountriesText", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_MISMATCHED_COUNTRIES_TEXT},     // NOLINT
        { "redirectModalMismatchedCountriesTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_MISMATCHED_COUNTRIES_TITLE},     // NOLINT
        { "redirectModalMismatchedProviderAccountsText", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_MISMATCHED_PROVIDER_ACCOUNTS_TEXT},     // NOLINT
        { "redirectModalMismatchedProviderAccountsTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_MISMATCHED_PROVIDER_ACCOUNTS_TITLE},     // NOLINT
        { "redirectModalProviderUnavailableText1", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_PROVIDER_UNAVAILABLE_TEXT_1},     // NOLINT
        { "redirectModalProviderUnavailableText2", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_PROVIDER_UNAVAILABLE_TEXT_2},     // NOLINT
        { "redirectModalProviderUnavailableTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_PROVIDER_UNAVAILABLE_TITLE},      // NOLINT
        { "redirectModalRegionNotSupportedText1", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_REGION_NOT_SUPPORTED_TEXT_1},     // NOLINT
        { "redirectModalRegionNotSupportedText2", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_REGION_NOT_SUPPORTED_TEXT_2},     // NOLINT
        { "redirectModalRegionNotSupportedTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_REGION_NOT_SUPPORTED_TITLE},     // NOLINT
        { "redirectModalUpholdBATNotAllowedText", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_UPHOLD_BAT_NOT_ALLOWED_TEXT},     // NOLINT
        { "redirectModalUpholdBATNotAllowedTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_UPHOLD_BAT_NOT_ALLOWED_TITLE},     // NOLINT
        { "redirectModalUpholdInsufficientCapabilitiesText", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_UPHOLD_INSUFFICIENT_CAPABILITIES_TEXT},     // NOLINT
        { "redirectModalUpholdInsufficientCapabilitiesTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_UPHOLD_INSUFFICIENT_CAPABILITIES_TITLE},     // NOLINT
        { "redirectModalWalletOwnershipVerificationFailureText", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_WALLET_OWNERSHIP_VERIFICATION_FAILURE_TEXT},     // NOLINT
        { "redirectModalWalletOwnershipVerificationFailureTitle", IDS_HNS_REWARDS_LOCAL_REDIRECT_MODAL_WALLET_OWNERSHIP_VERIFICATION_FAILURE_TITLE},     // NOLINT
        { "tosAndPp", IDS_HNS_REWARDS_LOCAL_TOS_AND_PP},     // NOLINT

        { "click",  IDS_HNS_REWARDS_LOCAL_ADS_CONFIRMATION_TYPE_CLICK },
        { "dismiss",  IDS_HNS_REWARDS_LOCAL_ADS_CONFIRMATION_TYPE_DISMISS },
        { "landed",  IDS_HNS_REWARDS_LOCAL_ADS_CONFIRMATION_TYPE_LANDED },
        { "view",  IDS_HNS_REWARDS_LOCAL_ADS_CONFIRMATION_TYPE_VIEW },

        { "rewardsBrowserCannotReceiveAds",  IDS_REWARDS_BROWSER_CANNOT_RECEIVE_ADS },  // NOLINT
        { "rewardsBrowserNeedsUpdateToSeeAds",  IDS_REWARDS_BROWSER_NEEDS_UPDATE_TO_SEE_ADS },  // NOLINT

        { "activityCopy", IDS_HNS_UI_ACTIVITY_COPY },
        { "activityNote", IDS_HNS_UI_ACTIVITY_NOTE },
        { "ads", IDS_HNS_UI_ADS},
        { "adsCurrentlyViewing", IDS_HNS_UI_ADS_CURRENTLY_VIEWING },
        { "adsHistoryNone", IDS_HNS_UI_ADS_HISTORY_NONE },
        { "adsHistorySubTitle", IDS_HNS_UI_ADS_HISTORY_SUBTITLE },
        { "adsHistoryTitle", IDS_HNS_UI_ADS_HISTORY_TITLE },
        { "adsGrantReceived", IDS_HNS_UI_ADS_GRANT_RECEIVED },
        { "all", IDS_HNS_UI_ADS_ALL },
        { "amount", IDS_HNS_UI_AMOUNT },
        { "autoContribute", IDS_HNS_UI_HNS_CONTRIBUTE_TITLE },
        { "autoContributeTransaction", IDS_HNS_UI_HNS_CONTRIBUTE_TRANSACTION },              // NOLINT
        { "hnsRewards", IDS_HNS_UI_HNS_REWARDS },
        { "cancel", IDS_HNS_UI_CANCEL },
        { "captchaDrag", IDS_HNS_UI_CAPTCHA_DRAG },
        { "captchaTarget", IDS_HNS_UI_CAPTCHA_TARGET },
        { "category", IDS_HNS_UI_ADS_CATEGORY },
        { "claim", IDS_HNS_UI_CLAIM },
        { "contribute", IDS_HNS_UI_CONTRIBUTE },
        { "date", IDS_HNS_UI_DATE },
        { "deposits", IDS_HNS_UI_DEPOSITS },
        { "description", IDS_HNS_UI_DESCRIPTION },
        { "donation", IDS_HNS_UI_DONATION },
        { "done", IDS_HNS_UI_DONE },
        { "earningsAds", IDS_HNS_UI_EARNINGS_ADS },
        { "earningsViewDepositHistory", IDS_HNS_UI_EARNINGS_VIEW_DEPOSIT_HISTORY },            // NOLINT
        { "excludeSite", IDS_HNS_UI_EXCLUDE_SITE },
        { "excludedSitesText", IDS_HNS_UI_EXCLUDED_SITES },
        { "grantDisclaimer", IDS_HNS_UI_GRANT_DISCLAIMER },
        { "grantTitleUGP", IDS_HNS_UI_GRANT_TITLE_UGP },
        { "grantSubtitleUGP", IDS_HNS_UI_GRANT_SUBTITLE_UGP },
        { "grantAmountTitleUGP", IDS_HNS_UI_GRANT_AMOUNT_TITLE_UGP },
        { "grantDateTitleUGP", IDS_HNS_UI_GRANT_DATE_TITLE_UGP },
        { "grantTitleAds", IDS_HNS_UI_GRANT_TITLE_ADS },
        { "grantSubtitleAds", IDS_HNS_UI_GRANT_SUBTITLE_ADS },
        { "grantAmountTitleAds", IDS_HNS_UI_GRANT_AMOUNT_TITLE_ADS },
        { "grantDateTitleAds", IDS_HNS_UI_GRANT_DATE_TITLE_ADS },
        { "grantExpire", IDS_HNS_UI_GRANT_EXPIRE },
        { "import", IDS_HNS_UI_IMPORT },
        { "learnMore", IDS_HNS_UI_LEARN_MORE },
        { "markAsInappropriate", IDS_HNS_UI_ADS_MARK_AS_INAPPROPRIATE },
        { "markAsInappropriateChecked", IDS_HNS_UI_ADS_MARK_AS_INAPPROPRIATE_CHECKED },        // NOLINT
        { "monthlyTipsBang", IDS_HNS_UI_MONTHLY_TIPS_BANG },
        { "monthlyTips", IDS_HNS_UI_MONTHLY_TIPS },
        { "noActivity", IDS_HNS_UI_NO_ACTIVITY },
        { "noAdsHistory", IDS_HNS_UI_ADS_NO_ADS_HISTORY },
        { "off", IDS_HNS_UI_OFF },
        { "ok", IDS_HNS_UI_OK },
        { "on", IDS_HNS_UI_ON },
        { "onboardingEarnHeader", IDS_HNS_REWARDS_ONBOARDING_EARN_HEADER },
        { "onboardingEarnText", IDS_HNS_REWARDS_ONBOARDING_EARN_TEXT },
        { "onboardingStartUsingRewards", IDS_HNS_REWARDS_ONBOARDING_START_USING_REWARDS },  // NOLINT
        { "onboardingStartUsingRewardsTextOnly", IDS_HNS_REWARDS_ONBOARDING_START_USING_REWARDS_TEXT_ONLY },  // NOLINT
        { "onboardingTerms", IDS_HNS_REWARDS_ONBOARDING_TERMS },
        { "oneTimeDonation", IDS_HNS_UI_ONE_TIME_DONATION },
        { "openAdsHistory", IDS_HNS_UI_OPEN_ADS_HISTORY },
        { "optOutTooltip", IDS_HNS_UI_ADS_OPT_OUT_TOOLTIP },
        { "payment", IDS_HNS_UI_PAYMENT },
        { "pinnedSitesHeader", IDS_HNS_UI_PAYMENT_PINNED_SITES_HEADER },
        { "pinnedSitesMsg", IDS_HNS_UI_PAYMENT_PINNED_SITES_MSG },
        { "pinnedSitesOne", IDS_HNS_UI_PAYMENT_PINNED_SITES_ONE },
        { "pinnedSitesTwo", IDS_HNS_UI_PAYMENT_PINNED_SITES_TWO },
        { "pinnedSitesThree", IDS_HNS_UI_PAYMENT_PINNED_SITES_THREE },
        { "pinnedSitesFour", IDS_HNS_UI_PAYMENT_PINNED_SITES_FOUR },
        { "pleaseNote", IDS_HNS_UI_PLEASE_NOTE },
        { "print", IDS_HNS_UI_PRINT },
        { "processingRequest", IDS_HNS_UI_PROCESSING_REQUEST },
        { "processingRequestButton", IDS_HNS_UI_PROCESSING_REQUEST_BUTTON },
        { "processorHnsTokens", IDS_HNS_UI_PROCESSOR_HNS_TOKENS },
        { "processorUphold", IDS_HNS_UI_PROCESSOR_UPHOLD },
        { "processorBitflyer", IDS_HNS_UI_PROCESSOR_BITFLYER },
        { "processorGemini", IDS_HNS_UI_PROCESSOR_GEMINI },
        { "recurring", IDS_HNS_UI_RECURRING },
        { "recurringDonations", IDS_HNS_UI_RECURRING_DONATIONS },
        { "relaunch", IDS_HNS_UI_RELAUNCH },
        { "remove", IDS_HNS_UI_REMOVE },
        { "removeAdFromSaved", IDS_HNS_UI_REMOVE_AD_FROM_SAVED },
        { "reset", IDS_HNS_UI_RESET },
        { "resetWallet", IDS_HNS_UI_RESET_WALLET },
        { "clearExcludeList", IDS_HNS_UI_CLEAR_EXCLUDE_LIST },
        { "reviewSitesMsg", IDS_HNS_UI_REVIEW_SITE_MSG },
        { "rewardsAdGrantAmount", IDS_REWARDS_WIDGET_AD_GRANT_AMOUNT },
        { "rewardsAdGrantTitle", IDS_REWARDS_WIDGET_AD_GRANT_TITLE },
        { "rewardsClaimRewards", IDS_REWARDS_WIDGET_CLAIM_REWARDS },
        { "rewardsClaimTokens", IDS_REWARDS_WIDGET_CLAIM_TOKENS },
        { "rewardsConnectAccount", IDS_REWARDS_CONNECT_ACCOUNT },
        { "rewardsContribute", IDS_HNS_UI_REWARDS_CONTRIBUTE },
        { "rewardsContributeAttention", IDS_HNS_UI_REWARDS_CONTRIBUTE_ATTENTION },             // NOLINT
        { "rewardsGrantDaysRemaining", IDS_REWARDS_WIDGET_GRANT_DAYS_REMAINING },  // NOLINT
        { "rewardsLearnMore", IDS_REWARDS_LEARN_MORE },
        { "rewardsLogInToSeeBalance", IDS_REWARDS_LOG_IN_TO_SEE_BALANCE },
        { "rewardsPaymentCheckStatus", IDS_REWARDS_PAYMENT_CHECK_STATUS },
        { "rewardsPaymentCompleted", IDS_REWARDS_PAYMENT_COMPLETED },
        { "rewardsPaymentPending", IDS_REWARDS_PAYMENT_PENDING },
        { "rewardsPaymentProcessing", IDS_REWARDS_PAYMENT_PROCESSING },
        { "rewardsPaymentSupport", IDS_REWARDS_PAYMENT_SUPPORT },
        { "rewardsResetConsent", IDS_HNS_UI_REWARDS_RESET_CONSENT },
        { "rewardsResetText", IDS_HNS_UI_REWARDS_RESET_TEXT },
        { "rewardsTokenGrantTitle", IDS_REWARDS_WIDGET_TOKEN_GRANT_TITLE },
        { "rewardsSummary", IDS_HNS_UI_REWARDS_SUMMARY },
        { "rewardsVBATNoticeText1", IDS_REWARDS_VBAT_NOTICE_TEXT1 },
        { "rewardsVBATNoticeText2", IDS_REWARDS_VBAT_NOTICE_TEXT2 },
        { "rewardsVBATNoticeTitle1", IDS_REWARDS_VBAT_NOTICE_TITLE1 },
        { "rewardsVBATNoticeTitle2", IDS_REWARDS_VBAT_NOTICE_TITLE2 },
        { "saved", IDS_HNS_UI_ADS_SAVED },
        { "saveAd", IDS_HNS_UI_ADS_SAVE_AD },
        { "seeAllItems", IDS_HNS_UI_SEE_ALL_ITEMS },
        { "sendTip", IDS_HNS_UI_SEND_TIP },
        { "settings", IDS_HNS_UI_SETTINGS },
        { "site", IDS_HNS_UI_SITE },
        { "tipOnLike", IDS_HNS_UI_TIP_ON_LIKE },
        { "tokenGrantReceived", IDS_HNS_UI_TOKEN_GRANT_RECEIVED },
        { "token", IDS_HNS_UI_TOKEN },
        { "tokens", IDS_HNS_UI_TOKENS },
        { "total", IDS_HNS_UI_TOTAL },
        { "transactions", IDS_HNS_UI_TRANSACTIONS },
        { "type", IDS_HNS_UI_TYPE },
        { "unsupportedRegionNoticeHeader", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_HEADER},  // NOLINT
        { "unsupportedRegionNoticeSubheader", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_SUBHEADER},  // NOLINT
        { "unsupportedRegionNoticeLearnMore", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_LEARN_MORE},  // NOLINT
        { "unsupportedRegionNoticeText1", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_TEXT_1},  // NOLINT
        { "unsupportedRegionNoticeText2", IDS_HNS_REWARDS_UNSUPPORTED_REGION_NOTICE_TEXT_2},  // NOLINT
        { "verifiedPublisher", IDS_HNS_UI_VERIFIED_PUBLISHER },
        { "viewDetails" , IDS_HNS_UI_VIEW_DETAILS },
        { "viewMonthly", IDS_HNS_UI_VIEW_MONTHLY },
        { "walletActivity", IDS_HNS_UI_WALLET_ACTIVITY },
        { "walletBalance", IDS_HNS_UI_WALLET_BALANCE },
        { "yourWallet", IDS_HNS_UI_YOUR_WALLET },

        { "excludedSites", IDS_HNS_UI_EXCLUDED_SITES_TEXT },
        { "removeFromExcluded", IDS_HNS_UI_REMOVE_FROM_EXCLUDED },
        { "rewardsExcludedText1", IDS_HNS_UI_REWARDS_EXCLUDED_TEXT_1 },
        { "rewardsExcludedText2", IDS_HNS_UI_REWARDS_EXCLUDED_TEXT_2 },
        { "showAll", IDS_HNS_UI_SHOW_ALL },
        { "viewedSites", IDS_HNS_UI_VIEWED_SITES },

        { "promoLearnMore", IDS_HNS_UI_PROMO_LEARN_MORE },
        { "promoDismiss", IDS_HNS_UI_PROMO_DISMISS },

        { "bitflyerVerificationPromoTitle", IDS_HNS_UI_BITFLYER_VERIFICATION_PROMO_TITLE },  // NOLINT
        { "bitflyerVerificationPromoInfo", IDS_HNS_UI_BITFLYER_VERIFICATION_PROMO_INFO },    // NOLINT

        { "hnsCreatorsPromoTitle", IDS_HNS_UI_HNS_CREATORS_PROMO_TITLE },
        { "hnsCreatorsPromoInfo1", IDS_HNS_UI_HNS_CREATORS_PROMO_INFO_1 },
        { "hnsCreatorsPromoInfo2", IDS_HNS_UI_HNS_CREATORS_PROMO_INFO_2 },

        { "geminiPromoTitle", IDS_HNS_UI_GEMINI_PROMO_TITLE },
        { "geminiPromoInfo1", IDS_HNS_UI_GEMINI_PROMO_INFO_1 },
        { "geminiPromoInfo2", IDS_HNS_UI_GEMINI_PROMO_INFO_2 },

        { "tapNetworkTitle", IDS_HNS_UI_TAP_NETWORK_TITLE },
        { "tapNetworkInfo", IDS_HNS_UI_TAP_NETWORK_INFO },
        { "tapNetworkDisclaimer", IDS_HNS_UI_TAP_NETWORK_DISCLAIMER },

        { "upholdPromoTitle", IDS_HNS_UI_UPHOLD_PROMO_TITLE },
        { "upholdPromoInfo", IDS_HNS_UI_UPHOLD_PROMO_INFO },

        { "connectAccountNoProviders", IDS_REWARDS_CONNECT_ACCOUNT_NO_PROVIDERS },  // NOLINT
        { "connectAccountText", IDS_REWARDS_CONNECT_ACCOUNT_TEXT },
        { "learnMore", IDS_REWARDS_LEARN_MORE },
        { "connectWalletHeader", IDS_HNS_REWARDS_CONNECT_WALLET_HEADER },  // NOLINT
        { "connectWalletDisclaimer", IDS_HNS_REWARDS_CONNECT_WALLET_DISCLAIMER },  // NOLINT
        { "connectWalletListItem1", IDS_HNS_REWARDS_CONNECT_WALLET_LIST_ITEM_1 },  // NOLINT
        { "connectWalletListItem2", IDS_HNS_REWARDS_CONNECT_WALLET_LIST_ITEM_2 },  // NOLINT
        { "connectWalletListItem3", IDS_HNS_REWARDS_CONNECT_WALLET_LIST_ITEM_3 },  // NOLINT
        { "connectWalletLearnMore", IDS_HNS_REWARDS_CONNECT_WALLET_LEARN_MORE },  // NOLINT
        { "connectWalletProviderNotAvailable", IDS_HNS_REWARDS_CONNECT_WALLET_PROVIDER_NOT_AVAILABLE },  // NOLINT
        { "contributionPendingUntil", IDS_HNS_REWARDS_CONTRIBUTION_PENDING_UNTIL },  // NOLINT

        { "walletAccountLink", IDS_REWARDS_WALLET_ACCOUNT_LINK },
        { "walletAutoContribute", IDS_REWARDS_WALLET_AUTO_CONTRIBUTE },
        { "walletDisconnected", IDS_REWARDS_WALLET_DISCONNECTED },
        { "walletEstimatedEarnings", IDS_REWARDS_ESTIMATED_EARNINGS_TITLE },
        { "walletLogIntoYourAccount", IDS_REWARDS_WALLET_LOG_INTO_YOUR_ACCOUNT },  // NOLINT
        { "walletMonthlyTips", IDS_REWARDS_WALLET_MONTHLY_TIPS },
        { "walletOneTimeTips", IDS_REWARDS_WALLET_ONE_TIME_TIPS },
        { "walletRewardsFromAds", IDS_REWARDS_WALLET_REWARDS_FROM_ADS },
        { "walletRewardsSummary", IDS_REWARDS_WALLET_REWARDS_SUMMARY },
        { "walletUnverified", IDS_REWARDS_WALLET_UNVERIFIED },
        { "walletViewStatement", IDS_REWARDS_WALLET_VIEW_STATEMENT },
        { "walletVerified", IDS_REWARDS_WALLET_VERIFIED },
        { "walletBalanceInfoText", IDS_REWARDS_WIDGET_BALANCE_INFO_TEXT},
        { "walletBalanceTitle", IDS_REWARDS_WALLET_BALANCE_TITLE },
        { "walletEarningInfoText", IDS_REWARDS_WIDGET_EARNING_INFO_TEXT },

        { "loading", IDS_HNS_REWARDS_LOADING_LABEL },
      }
    }, {
      std::string("adblock"), {
        { "additionalFiltersTitle", IDS_ADBLOCK_ADDITIONAL_FILTERS_TITLE },
        { "additionalFiltersWarning", IDS_ADBLOCK_ADDITIONAL_FILTERS_WARNING },                  // NOLINT
        { "adsBlocked", IDS_ADBLOCK_TOTAL_ADS_BLOCKED },
        { "customFiltersTitle", IDS_ADBLOCK_CUSTOM_FILTERS_TITLE },
        { "customFiltersInstructions", IDS_ADBLOCK_CUSTOM_FILTERS_INSTRUCTIONS },                // NOLINT
        { "customListSubscriptionsTitle", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_TITLE },         // NOLINT
        { "customListSubscriptionsInstructions", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_INSTRUCTIONS },  // NOLINT
        { "customListSubscriptionsDisclaimer", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_DISCLAIMER },      // NOLINT

        { "customListSubscriptionsEnterSubscriptionUrlPlaceholder", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_ENTER_SUBSCRIPTION_URL_PLACEHOLDER }, // NOLINT
        { "customListSubscriptionsSubmitNewSubscription", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_SUBMIT_NEW_SUBSCRIPTION },    // NOLINT
        { "customListSubscriptionsCancelAddSubscription", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_CANCEL_ADD_SUBSCRIPTION },    // NOLINT
        { "customListSubscriptionsAddNewFilterList", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_ADD_NEW_FILTER_LIST },             // NOLINT
        { "customListSubscriptionsTableFilterListColumn", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_TABLE_FILTER_LIST_COLUMN },   // NOLINT
        { "customListSubscriptionsTableLastUpdatedColumn", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_TABLE_LAST_UPDATED_COLUMN }, // NOLINT
        { "customListSubscriptionsTriggerUpdate", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_TRIGGER_UPDATE },                     // NOLINT
        { "customListSubscriptionsViewListSource", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_VIEW_LIST_SOURCE },                  // NOLINT
        { "customListSubscriptionsUnsubscribe", IDS_ADBLOCK_CUSTOM_LIST_SUBSCRIPTIONS_UNSUBSCRIBE },                          // NOLINT
      }
    }, {
#if BUILDFLAG(ENABLE_IPFS_INTERNALS_WEBUI)
      std::string("ipfs-internals"), {
        { "connectedPeersTitle", IDS_IPFS_CONNECTED_PEERS_TITLE },
        { "addressesConfigTitle", IDS_IPFS_ADDRESSES_CONFIG_TITLE },
        { "repoStatsTitle", IDS_IPFS_REPO_STATS_TITLE },
        { "daemonStatusTitle", IDS_IPFS_DAEMON_STATUS_TITLE },
        { "api", IDS_IPFS_API },
        { "gateway", IDS_IPFS_GATEWAY },
        { "swarm", IDS_IPFS_SWARM },
        { "objects", IDS_IPFS_REPO_OBJECTS },
        { "size", IDS_IPFS_REPO_SIZE },
        { "storage", IDS_IPFS_REPO_STORAGE },
        { "path", IDS_IPFS_REPO_PATH },
        { "version", IDS_IPFS_REPO_VERSION },
        { "launched", IDS_IPFS_LAUNCHED },
        { "notLaunched", IDS_IPFS_NOT_LAUNCHED },
        { "launch", IDS_IPFS_LAUNCH },
        { "shutdown", IDS_IPFS_SHUTDOWN },
        { "notInstalled", IDS_IPFS_NOT_INSTALLED },
        { "nodeInfoTitle", IDS_IPFS_NODE_INFO_TITLE },
        { "id", IDS_IPFS_NODE_INFO_ID },
        { "agentVersion", IDS_IPFS_NODE_INFO_VERSION },
        { "restart", IDS_IPFS_RESTART },
        { "learnMore", IDS_IPFS_LEARN_MORE },
        { "installAndLaunch", IDS_IPFS_INSTALL_AND_LAUNCH },
        { "openWebUI", IDS_IPFS_OPEN_WEBUI },
        { "peerDetailsLink", IDS_IPFS_PEERS_DETAILS_LINK },
        { "installing", IDS_IPFS_INSTALLING },
        { "runGarbageCollectionTitle", IDS_IPFS_RUN_GC_BUTTON },
        { "gcError", IDS_IPFS_GC_ERROR },
        { "rotateIdentity", IDS_IPFS_ROTATE_IDENTITY_TITLE }
      }
    }, {
#endif
      std::string("rewards-internals"), {
        { "adDiagnosticId", IDS_HNS_REWARDS_INTERNALS_AD_DIAGNOSTIC_ID },
        { "adDiagnosticInfo", IDS_HNS_REWARDS_INTERNALS_AD_DIAGNOSTIC_INFO },                  // NOLINT
        { "adsNotInitialized", IDS_HNS_REWARDS_INTERNALS_ADS_NOT_INITIALIZED },                // NOLINT
        { "amount", IDS_HNS_REWARDS_INTERNALS_AMOUNT },
        { "autoRefresh", IDS_HNS_REWARDS_INTERNALS_AUTO_REFRESH },
        { "balanceInfo", IDS_HNS_REWARDS_INTERNALS_BALANCE_INFO },
        { "bat", IDS_HNS_UI_BAT_TEXT },
        { "bootStamp", IDS_HNS_REWARDS_INTERNALS_BOOT_STAMP },
        { "clearButton", IDS_HNS_REWARDS_INTERNALS_CLEAR_BUTTON },
        { "contributedAmount", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTED_AMOUNT },
        { "contributionCreatedAt", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTED_CREATED_AT },         // NOLINT
        { "contribution", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION },
        { "contributionProcessor", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_PROCESSOR },         // NOLINT
        { "contributionStep", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP },
        { "contributionStepAutoContributeTableEmpty", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_AUTO_CONTRIBUTE_TABLE_EMPTY },  // NOLINT
        { "contributionStepNotEnoughFunds", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_NOT_ENOUGH_FUNDS },  // NOLINT
        { "contributionStepFailed", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_FAILED },      // NOLINT
        { "contributionStepCompleted", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_COMPLETED },// NOLINT
        { "contributionStepNo", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_NO },              // NOLINT
        { "contributionStepStart", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_START },        // NOLINT
        { "contributionStepPrepare", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_PREPARE },    // NOLINT
        { "contributionStepReserve", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_RESERVE },    // NOLINT
        { "contributionStepExternalTransaction", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_EXTERNAL_TRANSACTION },  // NOLINT
        { "contributionStepCreds", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_CREDS },        // NOLINT
        { "contributionStepRewardsOff", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_REWARDS_OFF },        // NOLINT
        { "contributionStepAutoContributeOff", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_AUTO_CONTRIBUTE_OFF },        // NOLINT
        { "contributionStepRetryCount", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_STEP_RETRY_COUNT },        // NOLINT
        { "eventLogKey", IDS_HNS_REWARDS_INTERNALS_EVENT_LOG_KEY },
        { "eventLogValue", IDS_HNS_REWARDS_INTERNALS_EVENT_LOG_VALUE },
        { "eventLogTime", IDS_HNS_REWARDS_INTERNALS_EVENT_LOG_TIME },
        { "mainDisclaimer", IDS_HNS_REWARDS_INTERNALS_MAIN_DISCLAIMER },
        { "logDisclaimer", IDS_HNS_REWARDS_INTERNALS_LOG_DISCLAIMER },
        { "rewardsTypeAuto", IDS_HNS_REWARDS_INTERNALS_REWARDS_TYPE_AUTO },                    // NOLINT
        { "rewardsTypeOneTimeTip", IDS_HNS_REWARDS_INTERNALS_REWARDS_TYPE_ONE_TIME_TIP },      // NOLINT
        { "rewardsTypeRecurringTip", IDS_HNS_REWARDS_INTERNALS_REWARDS_TYPE_RECURRING_TIP },   // NOLINT
        { "contributionType", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTION_TYPE },
        { "contributions", IDS_HNS_REWARDS_INTERNALS_CONTRIBUTIONS },
        { "custodian", IDS_HNS_REWARDS_INTERNALS_CUSTODIAN },
        { "custodianMemberId", IDS_HNS_REWARDS_INTERNALS_CUSTODIAN_MEMBER_ID },                // NOLINT
        { "downloadButton", IDS_HNS_REWARDS_INTERNALS_DOWNLOAD_BUTTON },
        { "externalWallet", IDS_HNS_REWARDS_INTERNALS_EXTERNAL_WALLET },
        { "invalid", IDS_HNS_REWARDS_INTERNALS_INVALID },
        { "keyInfoSeed", IDS_HNS_REWARDS_INTERNALS_KEY_INFO_SEED },
        { "logNotice", IDS_HNS_REWARDS_INTERNALS_LOG_NOTICE },
        { "mainTitle", IDS_HNS_REWARDS_INTERNALS_MAIN_TITLE },
        { "notSet", IDS_HNS_REWARDS_INTERNALS_NOT_SET },
        { "personaId", IDS_HNS_REWARDS_INTERNALS_PERSONA_ID },
        { "processorHnsTokens", IDS_HNS_UI_PROCESSOR_HNS_TOKENS },
        { "processorUphold", IDS_HNS_UI_PROCESSOR_UPHOLD },
        { "processorBitflyer", IDS_HNS_UI_PROCESSOR_BITFLYER },
        { "processorGemini", IDS_HNS_UI_PROCESSOR_GEMINI },
        { "processorZebPay", IDS_HNS_UI_PROCESSOR_ZEBPAY },
        { "promotionAds", IDS_HNS_REWARDS_INTERNALS_PROMOTION_ADS },
        { "promotionAmount", IDS_HNS_REWARDS_INTERNALS_PROMOTION_AMOUNT },
        { "promotionClaimedAt", IDS_HNS_REWARDS_INTERNALS_PROMOTION_CLAIMED_AT },              // NOLINT
        { "promotionClaimId", IDS_HNS_REWARDS_INTERNALS_PROMOTION_CLAIM_ID },
        { "promotionExpiresAt", IDS_HNS_REWARDS_INTERNALS_PROMOTION_EXPIRES_AT },              // NOLINT
        { "promotionId", IDS_HNS_REWARDS_INTERNALS_PROMOTION_ID },
        { "promotionLegacyClaimed", IDS_HNS_REWARDS_INTERNALS_PROMOTION_LEGACY_CLAIMED },      // NOLINT
        { "promotionLegacyNo", IDS_HNS_REWARDS_INTERNALS_PROMOTION_LEGACY_NO },                // NOLINT
        { "promotionLegacyYes", IDS_HNS_REWARDS_INTERNALS_PROMOTION_LEGACY_YES },              // NOLINT
        { "promotions", IDS_HNS_REWARDS_INTERNALS_PROMOTIONS },
        { "promotionStatus", IDS_HNS_REWARDS_INTERNALS_PROMOTION_STATUS },
        { "promotionStatusActive", IDS_HNS_REWARDS_INTERNALS_PROMOTION_STATUS_ACTIVE },        // NOLINT
        { "promotionStatusAttested", IDS_HNS_REWARDS_INTERNALS_PROMOTION_STATUS_ATTESTED },    // NOLINT
        { "promotionStatusFinished", IDS_HNS_REWARDS_INTERNALS_PROMOTION_STATUS_FINISHED },    // NOLINT
        { "promotionStatusOver", IDS_HNS_REWARDS_INTERNALS_PROMOTION_STATUS_OVER },            // NOLINT
        { "promotionType", IDS_HNS_REWARDS_INTERNALS_PROMOTION_TYPE },
        { "promotionUGP", IDS_HNS_REWARDS_INTERNALS_PROMOTION_UGP },
        { "promotionVersion", IDS_HNS_REWARDS_INTERNALS_PROMOTION_VERSION },
        { "refreshButton", IDS_HNS_REWARDS_INTERNALS_REFRESH_BUTTON },
        { "retryCount", IDS_HNS_REWARDS_INTERNALS_RETRY_COUNT },
        { "rewardsCountry", IDS_HNS_REWARDS_INTERNALS_REWARDS_COUNTRY },
        { "tabAdDiagnostics", IDS_HNS_REWARDS_INTERNALS_TAB_AD_DIAGNOSTICS },
        { "tabGeneralInfo", IDS_HNS_REWARDS_INTERNALS_TAB_GENERAL_INFO },
        { "tabLogs", IDS_HNS_REWARDS_INTERNALS_TAB_LOGS },
        { "tabPromotions", IDS_HNS_REWARDS_INTERNALS_TAB_PROMOTIONS },
        { "tabContributions", IDS_HNS_REWARDS_INTERNALS_TAB_CONTRIBUTIONS },
        { "tabEventLogs", IDS_HNS_REWARDS_INTERNALS_TAB_EVENT_LOGS },
        { "totalAmount", IDS_HNS_REWARDS_INTERNALS_TOTAL_AMOUNT },
        { "totalBalance", IDS_HNS_REWARDS_INTERNALS_TOTAL_BALANCE },
        { "userId", IDS_HNS_REWARDS_INTERNALS_USER_ID },
        { "valid", IDS_HNS_REWARDS_INTERNALS_VALID },
        { "walletAddress", IDS_HNS_REWARDS_INTERNALS_WALLET_ADDRESS },
        { "walletHistory", IDS_HNS_REWARDS_INTERNALS_WALLET_HISTORY },
        { "walletInfo", IDS_HNS_REWARDS_INTERNALS_WALLET_INFO },
        { "walletNotCreated", IDS_HNS_REWARDS_INTERNALS_WALLET_NOT_CREATED },
        { "walletPaymentId", IDS_HNS_REWARDS_INTERNALS_WALLET_PAYMENT_ID },
        { "walletStatus", IDS_HNS_REWARDS_INTERNALS_WALLET_STATUS },
        { "walletStatusNotConnected", IDS_HNS_REWARDS_INTERNALS_WALLET_STATUS_NOT_CONNECTED },    // NOLINT
        { "walletStatusVerified", IDS_HNS_REWARDS_INTERNALS_WALLET_STATUS_VERIFIED },    // NOLINT
        { "walletStatusDisconnectedVerified", IDS_HNS_REWARDS_INTERNALS_WALLET_STATUS_DISCONNECTED_VERIFIED },    // NOLINT
        { "walletCreationEnvironment", IDS_HNS_REWARDS_INTERNALS_WALLET_CREATION_ENVIRONMENT },    // NOLINT
        { "currentEnvironment", IDS_HNS_REWARDS_INTERNALS_CURRENT_ENVIRONMENT},    // NOLINT

        { "loading", IDS_HNS_REWARDS_LOADING_LABEL },
      }
    }, {
#if BUILDFLAG(ENABLE_TOR)
      std::string("tor-internals"), {
        { "tabGeneralInfo", IDS_TOR_INTERNALS_TAB_GENERAL_INFO },
        { "tabLogs", IDS_TOR_INTERNALS_TAB_LOGS },
        { "torControlEvents", IDS_TOR_INTERNALS_TOR_CONTROL_EVENTS },
        { "torVersion", IDS_TOR_INTERNALS_TOR_VERSION },
        { "torPid", IDS_TOR_INTERNALS_TOR_PID },
        { "torProxyURI", IDS_TOR_INTERNALS_TOR_PROXY_URI },
        { "torConnectionStatus", IDS_TOR_INTERNALS_TOR_CONNECTION_STATUS },
        { "torInitProgress", IDS_TOR_INTERNALS_TOR_INIT_PROGRESS },
      }
    }, {
#endif
#if !BUILDFLAG(IS_ANDROID)
      std::string("webcompat"), {
        // Report modal
        { "reportModalTitle", IDS_HNS_WEBCOMPATREPORTER_REPORT_MODAL_TITLE },
        { "reportExplanation", IDS_HNS_WEBCOMPATREPORTER_REPORT_EXPLANATION },
        { "reportDisclaimer", IDS_HNS_WEBCOMPATREPORTER_REPORT_DISCLAIMER },
        { "reportDetails", IDS_HNS_WEBCOMPATREPORTER_REPORT_DETAILS },
        { "reportContactPlaceholder",
          IDS_HNS_WEBCOMPATREPORTER_REPORT_CONTACT_PLACEHOLDER },
        { "reportContactLabel",
          IDS_HNS_WEBCOMPATREPORTER_REPORT_CONTACT_LABEL },
        { "cancel", IDS_HNS_WEBCOMPATREPORTER_CANCEL },
        { "submit", IDS_HNS_WEBCOMPATREPORTER_SUBMIT },
        // Confirmation modal
        { "thankYou", IDS_HNS_WEBCOMPATREPORTER_THANK_YOU },
        { "confirmationNotice",
            IDS_HNS_WEBCOMPATREPORTER_CONFIRMATION_NOTICE },
      }
#endif
    }
  };
  // clang-format on
  AddLocalizedStringsBulk(source, localized_strings[name]);
}  // NOLINT(readability/fn_size)

content::WebUIDataSource* CreateWebUIDataSource(
    content::WebUI* web_ui,
    const std::string& name,
    const webui::ResourcePath* resource_map,
    size_t resource_map_size,
    int html_resource_id,
    bool disable_trusted_types_csp) {
  content::WebUIDataSource* source =
      content::WebUIDataSource::CreateAndAdd(Profile::FromWebUI(web_ui), name);
  // Some parts of Hns's UI pages are not yet migrated to work without doing
  // assignments of strings directly into |innerHTML| elements (i.e. see usage
  // of |dangerouslySetInnerHTML| in .tsx files). This will break Hns due to
  // committing a Trusted Types related violation now that Trusted Types are
  // enforced on WebUI pages (see crrev.com/c/2234238 and crrev.com/c/2353547).
  // We should migrate those pages not to require using |innerHTML|, but for now
  // we just restore pre-Cromium 87 behaviour for pages that are not ready yet.
  if (disable_trusted_types_csp) {
    source->DisableTrustedTypesCSP();
  } else {
    // Allow a policy to be created so that we
    // can allow trusted HTML and trusted lazy-load script sources.
    source->OverrideContentSecurityPolicy(
        network::mojom::CSPDirectiveName::TrustedTypes,
        "trusted-types default;");
  }

  source->UseStringsJs();
  source->SetDefaultResource(html_resource_id);
  // Add generated resource paths
  for (size_t i = 0; i < resource_map_size; ++i) {
    source->AddResourcePath(resource_map[i].path, resource_map[i].id);
  }
  CustomizeWebUIHTMLSource(web_ui, name, source);
  return source;
}

}  // namespace

content::WebUIDataSource* CreateAndAddWebUIDataSource(
    content::WebUI* web_ui,
    const std::string& name,
    const webui::ResourcePath* resource_map,
    size_t resource_map_size,
    int html_resource_id,
    bool disable_trusted_types_csp) {
  content::WebUIDataSource* data_source =
      CreateWebUIDataSource(web_ui, name, resource_map, resource_map_size,
                            html_resource_id, disable_trusted_types_csp);
  return data_source;
}

// Android doesn't need WebUI WebContents to match background color
#if !BUILDFLAG(IS_ANDROID)

void AddBackgroundColorToSource(content::WebUIDataSource* source,
                                content::WebContents* contents) {
  // Get the specific background color for the type of browser window
  // that the contents is in.
  // TODO(petemill): we do not use web_contents->GetColorProvider()
  // here because it does not include HnsPrivateWindowThemeSupplier. This
  // should get fixed, potentially via `WebContents::SetColorProviderSource`.
  auto* browser_window =
      BrowserWindow::FindBrowserWindowWithWebContents(contents);
  if (!browser_window) {
    // Some newly created WebContents aren't yet attached
    // to a browser window, so get any that match the current profile,
    // which is fine for color provider.
    Profile* profile =
        Profile::FromBrowserContext(contents->GetBrowserContext());
    const Browser* browser = chrome::FindBrowserWithProfile(profile);
    if (browser) {
      browser_window = browser->window();
    }
  }
  if (!browser_window) {
    DLOG(ERROR) << "No BrowserWindow could be found for WebContents";
    return;
  }
  const ui::ColorProvider* color_provider = browser_window->GetColorProvider();
  SkColor ntp_background_color =
      color_provider->GetColor(kColorNewTabPageBackground);
  // Set to a template replacement string that can be inserted to the
  // html.
  std::string ntp_background_color_css =
      ui::ConvertSkColorToCSSColor(ntp_background_color);
  source->AddString("backgroundColor", ntp_background_color_css);
}

#endif  // !BUILDFLAG(IS_ANDROID)
