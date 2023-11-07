/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/request_otr/browser/request_otr_blocking_page.h"

#include <ostream>
#include <utility>

#include "base/notreached.h"
#include "base/strings/string_number_conversions.h"
#include "hns/components/l10n/common/localization_util.h"
#include "hns/components/request_otr/browser/request_otr_controller_client.h"
#include "components/grit/hns_components_resources.h"
#include "components/grit/hns_components_strings.h"
#include "components/security_interstitials/content/security_interstitial_controller_client.h"
#include "url/origin.h"

namespace request_otr {

// static
const security_interstitials::SecurityInterstitialPage::TypeID
    RequestOTRBlockingPage::kTypeForTesting =
        &RequestOTRBlockingPage::kTypeForTesting;

RequestOTRBlockingPage::RequestOTRBlockingPage(
    content::WebContents* web_contents,
    const GURL& request_url,
    std::unique_ptr<
        security_interstitials::SecurityInterstitialControllerClient>
        controller)
    : security_interstitials::SecurityInterstitialPage(web_contents,
                                                       request_url,
                                                       std::move(controller)) {}

RequestOTRBlockingPage::~RequestOTRBlockingPage() = default;

void RequestOTRBlockingPage::CommandReceived(const std::string& command) {
  if (command == "\"pageLoadComplete\"") {
    // content::WaitForRenderFrameReady sends this message when the page
    // load completes. Ignore it.
    return;
  }

  int cmd = 0;
  bool retval = base::StringToInt(command, &cmd);
  DCHECK(retval);
  RequestOTRControllerClient* request_otr_controller =
      static_cast<RequestOTRControllerClient*>(controller());

  switch (cmd) {
    case security_interstitials::CMD_DONT_PROCEED:
      request_otr_controller->Proceed();
      break;
    case security_interstitials::CMD_PROCEED:
      request_otr_controller->ProceedOTR();
      break;
    case security_interstitials::CMD_DO_REPORT:
      request_otr_controller->SetDontWarnAgain(true);
      break;
    case security_interstitials::CMD_DONT_REPORT:
      request_otr_controller->SetDontWarnAgain(false);
      break;
    default:
      NOTREACHED() << "Unsupported command: " << command;
  }
}

void RequestOTRBlockingPage::PopulateInterstitialStrings(
    base::Value::Dict& load_time_data) {
  load_time_data.Set("tabTitle", hns_l10n::GetLocalizedResourceUTF16String(
                                     IDS_REQUEST_OTR_TITLE));
  load_time_data.Set("heading", hns_l10n::GetLocalizedResourceUTF16String(
                                    IDS_REQUEST_OTR_HEADING));

  load_time_data.Set("primaryParagraph",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_REQUEST_OTR_PRIMARY_PARAGRAPH));

  url::Origin request_url_origin = url::Origin::Create(request_url());
  load_time_data.Set("domain", request_url_origin.Serialize());

  load_time_data.Set(
      "explanationParagraph",
      hns_l10n::GetLocalizedResourceUTF16String(IDS_REQUEST_OTR_EXPLANATION));

  load_time_data.Set("neverAskAgainText",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_REQUEST_OTR_NEVER_ASK_AGAIN_BUTTON));

  load_time_data.Set("neverAskAgainExplanationText",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_REQUEST_OTR_NEVER_ASK_AGAIN_EXPLANATION));

  load_time_data.Set("proceedOTRText",
                     hns_l10n::GetLocalizedResourceUTF16String(
                         IDS_REQUEST_OTR_PROCEED_OTR_BUTTON));

  load_time_data.Set("proceedText", hns_l10n::GetLocalizedResourceUTF16String(
                                        IDS_REQUEST_OTR_PROCEED_BUTTON));
}

int RequestOTRBlockingPage::GetHTMLTemplateId() {
  return IDR_REQUEST_OTR_INTERSTITIAL_HTML;
}

security_interstitials::SecurityInterstitialPage::TypeID
RequestOTRBlockingPage::GetTypeForTesting() {
  return RequestOTRBlockingPage::kTypeForTesting;
}

}  // namespace request_otr
