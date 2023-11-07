/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_wallet/wallet_common_ui.h"
#include "hns/components/l10n/common/localization_util.h"
#include "chrome/browser/chooser_controller/title_util.h"
#include "chrome/grit/generated_resources.h"
#include "components/grit/hns_components_strings.h"
#include "content/public/browser/web_contents.h"
#include "ui/base/l10n/l10n_util.h"
#include "url/gurl.h"

namespace {

std::u16string HnsCreateTitleLabel() {
  auto wallet_title =
      hns_l10n::GetLocalizedResourceUTF16String(IDS_HNS_WALLET);
  return l10n_util::GetStringFUTF16(IDS_HID_CHOOSER_PROMPT, wallet_title);
}

}  // namespace

#define CreateChooserTitle                         \
  hns_wallet::IsHnsWalletOrigin(               \
      render_frame_host->GetLastCommittedOrigin()) \
      ? HnsCreateTitleLabel()                    \
      : CreateChooserTitle
#include "src/chrome/browser/ui/hid/hid_chooser_controller.cc"
#undef CreateChooserTitle
