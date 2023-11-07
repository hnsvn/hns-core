/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/installer/util/hns_shell_util.h"

#include "base/notreached.h"
#include "chrome/install_static/install_util.h"
#include "components/version_info/channel.h"

namespace installer {

std::wstring GetProgIdForFileType() {
  switch (install_static::GetChromeChannel()) {
    case version_info::Channel::STABLE:
      return L"HnsFile";
    case version_info::Channel::BETA:
      return L"HnsBFile";
    case version_info::Channel::DEV:
      return L"HnsDFile";
    case version_info::Channel::CANARY:
      return L"HnsSSFile";
    default:
      NOTREACHED();
      return L"HnsFile";
  }
}

bool ShouldUseFileTypeProgId(const std::wstring& ext) {
  return (ext == L".pdf" || ext == L".svg");
}

}  // namespace installer
