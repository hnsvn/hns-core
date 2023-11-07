/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include <string>

#include "base/strings/strcat.h"
#include "base/strings/utf_string_conversions.h"
#include "components/download/public/common/download_url_parameters.h"
#include "url/gurl.h"

namespace {

std::u16string GetHnsFilenameForURL(const GURL& url) {
  if (!url.has_host() || url.HostIsIPAddress()) {
    return u"hns_screenshot.png";
  }

  return base::ASCIIToUTF16(
      base::StrCat({"hns_screenshot_", url.host_piece(), ".png"}));
}

}  // namespace

#define set_suggested_name(...) \
  set_suggested_name(           \
      GetHnsFilenameForURL(web_contents_->GetLastCommittedURL()))

#include "src/chrome/browser/ui/views/sharing_hub/screenshot/screenshot_captured_bubble.cc"

#undef set_suggested_name
