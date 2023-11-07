/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_vpn/hns_vpn_localized_string_provider.h"

#include "base/no_destructor.h"
#include "hns/components/l10n/common/localization_util.h"
#include "components/grit/hns_components_strings.h"
#include "content/public/browser/url_data_source.h"
#include "ui/base/webui/web_ui_util.h"

namespace hns_vpn {

void AddLocalizedStrings(content::WebUIDataSource* html_source) {
  static constexpr webui::LocalizedString kLocalizedStrings[] = {
      {"hnsVpn", IDS_HNS_VPN},
      {"hnsVpnConnect", IDS_HNS_VPN_CONNECT},
      {"hnsVpnConnecting", IDS_HNS_VPN_CONNECTING},
      {"hnsVpnConnected", IDS_HNS_VPN_CONNECTED},
      {"hnsVpnDisconnecting", IDS_HNS_VPN_DISCONNECTING},
      {"hnsVpnDisconnected", IDS_HNS_VPN_DISCONNECTED},
      {"hnsVpnConnectionFailed", IDS_HNS_VPN_CONNECTION_FAILED},
      {"hnsVpnUnableConnectToServer", IDS_HNS_VPN_UNABLE_CONNECT_TO_SERVER},
      {"hnsVpnTryAgain", IDS_HNS_VPN_TRY_AGAIN},
      {"hnsVpnChooseAnotherServer", IDS_HNS_VPN_CHOOSE_ANOTHER_SERVER},
      {"hnsVpnUnableConnectInfo", IDS_HNS_VPN_UNABLE_CONNECT_INFO},
      {"hnsVpnBuy", IDS_HNS_VPN_BUY},
      {"hnsVpnPurchased", IDS_HNS_VPN_HAS_PURCHASED},
      {"hnsVpnPoweredBy", IDS_HNS_VPN_POWERED_BY},
      {"hnsVpnSettingsPanelHeader", IDS_HNS_VPN_SETTINGS_PANEL_HEADER},
      {"hnsVpnErrorPanelHeader", IDS_HNS_VPN_ERROR_PANEL_HEADER},
      {"hnsVpnStatus", IDS_HNS_VPN_STATUS},
      {"hnsVpnExpires", IDS_HNS_VPN_EXPIRES},
      {"hnsVpnManageSubscription", IDS_HNS_VPN_MANAGE_SUBSCRIPTION},
      {"hnsVpnContactSupport", IDS_HNS_VPN_CONTACT_SUPPORT},
      {"hnsVpnAbout", IDS_HNS_VPN_ABOUT},
      {"hnsVpnFeature1", IDS_HNS_VPN_FEATURE_1},
      {"hnsVpnFeature2", IDS_HNS_VPN_FEATURE_2},
      {"hnsVpnFeature3", IDS_HNS_VPN_FEATURE_3},
      {"hnsVpnFeature4", IDS_HNS_VPN_FEATURE_4},
      {"hnsVpnFeature5", IDS_HNS_VPN_FEATURE_5},
      {"hnsVpnLoading", IDS_HNS_VPN_LOADING},
      {"hnsVpnPurchaseFailed", IDS_HNS_VPN_PURCHASE_FAILED},
      {"hnsVpnSupportTicketFailed", IDS_HNS_VPN_SUPPORT_TICKET_FAILED},
      {"hnsVpnEditPaymentMethod", IDS_HNS_VPN_EDIT_PAYMENT},
      {"hnsVpnPaymentFailure", IDS_HNS_VPN_PAYMENT_FAILURE},
      {"hnsVpnPaymentFailureReason", IDS_HNS_VPN_PAYMENT_FAILURE_REASON},
      {"hnsVpnSupportEmail", IDS_HNS_VPN_SUPPORT_EMAIL},
      {"hnsVpnSupportEmailNotValid", IDS_HNS_VPN_SUPPORT_EMAIL_NOT_VALID},
      {"hnsVpnSupportSubject", IDS_HNS_VPN_SUPPORT_SUBJECT},
      {"hnsVpnSupportSubjectNotSet", IDS_HNS_VPN_SUPPORT_SUBJECT_NOTSET},
      {"hnsVpnSupportSubjectOtherConnectionProblem",
       IDS_HNS_VPN_SUPPORT_SUBJECT_OTHER_CONNECTION_PROBLEM},
      {"hnsVpnSupportSubjectNoInternet",
       IDS_HNS_VPN_SUPPORT_SUBJECT_NO_INTERNET},
      {"hnsVpnSupportSubjectSlowConnection",
       IDS_HNS_VPN_SUPPORT_SUBJECT_SLOW_CONNECTION},
      {"hnsVpnSupportSubjectWebsiteDoesntWork",
       IDS_HNS_VPN_SUPPORT_SUBJECT_WEBSITE_DOESNT_WORK},
      {"hnsVpnSupportSubjectOther", IDS_HNS_VPN_SUPPORT_SUBJECT_OTHER},
      {"hnsVpnSupportBody", IDS_HNS_VPN_SUPPORT_BODY},
      {"hnsVpnSupportOptionalHeader", IDS_HNS_VPN_SUPPORT_OPTIONAL_HEADER},
      {"hnsVpnSupportOptionalNotes", IDS_HNS_VPN_SUPPORT_OPTIONAL_NOTES},
      {"hnsVpnSupportOptionalNotesPrivacyPolicy",
       IDS_HNS_VPN_SUPPORT_OPTIONAL_NOTES_PRIVACY_POLICY},
      {"hnsVpnSupportOptionalVpnHostname",
       IDS_HNS_VPN_SUPPORT_OPTIONAL_VPN_HOSTNAME},
      {"hnsVpnSupportOptionalAppVersion",
       IDS_HNS_VPN_SUPPORT_OPTIONAL_APP_VERSION},
      {"hnsVpnSupportOptionalOsVersion",
       IDS_HNS_VPN_SUPPORT_OPTIONAL_OS_VERSION},
      {"hnsVpnSupportNotes", IDS_HNS_VPN_SUPPORT_NOTES},
      {"hnsVpnSupportSubmit", IDS_HNS_VPN_SUPPORT_SUBMIT},
      {"hnsVpnConnectNotAllowed", IDS_HNS_VPN_CONNECT_NOT_ALLOWED},
      {"hnsVpnSupportTimezone", IDS_HNS_VPN_SUPPORT_TIMEZONE},
      {"hnsVpnSessionExpiredTitle",
       IDS_HNS_VPN_MAIN_PANEL_SESSION_EXPIRED_PART_TITLE},
      {"hnsVpnSettingsTooltip", IDS_HNS_VPN_MAIN_PANEL_VPN_SETTINGS_TITLE},
      {"hnsVpnSessionExpiredContent",
       IDS_HNS_VPN_MAIN_PANEL_SESSION_EXPIRED_PART_CONTENT},
  };

  for (const auto& str : kLocalizedStrings) {
    std::u16string l10n_str =
        hns_l10n::GetLocalizedResourceUTF16String(str.id);
    html_source->AddString(str.name, l10n_str);
  }
}

}  // namespace hns_vpn
