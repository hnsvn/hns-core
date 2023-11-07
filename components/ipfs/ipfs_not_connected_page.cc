/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/ipfs/ipfs_not_connected_page.h"

#include <ostream>
#include <utility>

#include "base/notreached.h"
#include "base/strings/string_number_conversions.h"
#include "hns/components/l10n/common/localization_util.h"
#include "components/grit/hns_components_resources.h"
#include "components/grit/hns_components_strings.h"
#include "components/security_interstitials/content/security_interstitial_controller_client.h"

namespace ipfs {

// static
const security_interstitials::SecurityInterstitialPage::TypeID
    IPFSNotConnectedPage::kTypeForTesting =
        &IPFSNotConnectedPage::kTypeForTesting;

IPFSNotConnectedPage::IPFSNotConnectedPage(
    content::WebContents* web_contents,
    const GURL& request_url,
    std::unique_ptr<
        security_interstitials::SecurityInterstitialControllerClient>
        controller)
    : security_interstitials::SecurityInterstitialPage(web_contents,
                                                       request_url,
                                                       std::move(controller)) {}

IPFSNotConnectedPage::~IPFSNotConnectedPage() = default;

void IPFSNotConnectedPage::CommandReceived(const std::string& command) {
  if (command == "\"pageLoadComplete\"") {
    // content::WaitForRenderFrameReady sends this message when the page
    // load completes. Ignore it.
    return;
  }

  int cmd = 0;
  bool retval = base::StringToInt(command, &cmd);
  DCHECK(retval);

  switch (cmd) {
    case security_interstitials::CMD_PROCEED:
      controller()->Proceed();
      break;
    default:
      NOTREACHED() << "Unsupported command: " << command;
  }
}

void IPFSNotConnectedPage::PopulateInterstitialStrings(
    base::Value::Dict& load_time_data) {
  load_time_data.Set("tabTitle", hns_l10n::GetLocalizedResourceUTF16String(
                                     IDS_IPFS_NOT_CONNECTED_TITLE));
  load_time_data.Set("heading", hns_l10n::GetLocalizedResourceUTF16String(
                                    IDS_IPFS_NOT_CONNECTED_HEADING));

  load_time_data.Set("primaryParagraph",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_IPFS_NOT_CONNECTED_PRIMARY_PARAGRAPH));

  load_time_data.Set("primaryButtonText",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_IPFS_NOT_CONNECTED_PRIMARY_BUTTON));
  load_time_data.Set("openDetails",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_IPFS_NOT_CONNECTED_OPEN_DETAILS_BUTTON));
  load_time_data.Set("closeDetails",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_IPFS_NOT_CONNECTED_CLOSE_DETAILS_BUTTON));
  load_time_data.Set("explanationParagraph",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_IPFS_NOT_CONNECTED_EXPLANATION));
  load_time_data.Set("finalParagraph", std::u16string());
}

int IPFSNotConnectedPage::GetHTMLTemplateId() {
  return IDR_IPFS_INTERSTITIAL_HTML;
}

security_interstitials::SecurityInterstitialPage::TypeID
IPFSNotConnectedPage::GetTypeForTesting() {
  return IPFSNotConnectedPage::kTypeForTesting;
}

}  // namespace ipfs
