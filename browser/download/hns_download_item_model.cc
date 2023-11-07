/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/download/hns_download_item_model.h"

#include <string>

#include "base/strings/string_util.h"
#include "base/strings/utf_string_conversions.h"
#include "hns/components/l10n/common/localization_util.h"
#include "chrome/browser/download/download_ui_model.h"
#include "components/download/public/common/download_item.h"
#include "components/strings/grit/components_strings.h"
#include "services/network/public/cpp/is_potentially_trustworthy.h"
#include "ui/gfx/text_constants.h"
#include "ui/gfx/text_elider.h"
#include "url/gurl.h"
#include "url/origin.h"
#include "url/url_constants.h"

using download::DownloadItem;

HnsDownloadItemModel::HnsDownloadItemModel(DownloadUIModel* model)
    : model_(model) {}

HnsDownloadItemModel::~HnsDownloadItemModel() = default;

// Adds origin url to the tooltip text and "Not secure", if needed.
std::u16string HnsDownloadItemModel::GetTooltipText() {
  std::u16string tooltip = model_->GetTooltipText();

  bool is_secure;
  std::u16string origin_url = GetOriginURLText(&is_secure);

  if (!origin_url.empty()) {
    tooltip += u"\n";
    if (!is_secure) {
      tooltip += hns_l10n::GetLocalizedResourceUTF16String(
                     IDS_NOT_SECURE_VERBOSE_STATE) +
                 char16_t(' ');
    }
    tooltip += origin_url;
  }

  return tooltip;
}

// Returns origin url text and sets |is_secure|.
std::u16string HnsDownloadItemModel::GetOriginURLText(bool* is_secure) {
  *is_secure = false;
  if (!model_->GetDownloadItem()) {
    return std::u16string();
  }

  const GURL gurl = model_->GetDownloadItem()->GetURL();
  if (gurl.is_empty()) {
    return std::u16string();
  }

  std::string origin;
  if (gurl.is_valid()) {
    *is_secure = network::IsUrlPotentiallyTrustworthy(gurl);
    if (gurl.SchemeIs(url::kAboutScheme)) {
      origin = gurl.spec();
    } else {
      origin = url::Origin::Create(gurl).Serialize();
      if (gurl.SchemeIsFile()) {
        // url::Origin::Serialize() does an ASCII serialization of the Origin as
        // per Section 6.2 of RFC 6454, with the addition that all Origins with
        // a 'file' scheme serialize to "file://" (see url/origin.{h,cc}).
        DCHECK_EQ(origin, "file://");

        // However, we want to return 'file:///' (with a trailing '/') in this
        // case, see the HnsDownloadItemModelTest.GetOriginUrlText unit test.
        origin = "file:///";
      }
    }
  } else {
    origin = gurl.possibly_invalid_spec();
  }

  return base::UTF8ToUTF16(origin);
}
