/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/notreached.h"
#include "base/strings/stringprintf.h"
#include "hns/installer/util/hns_shell_util.h"
#include "chrome/common/chrome_icon_resources_win.h"
#include "chrome/install_static/install_util.h"
#include "components/version_info/channel.h"

namespace {

#if defined(OFFICIAL_BUILD)

int GetIconIndexForFileType() {
  switch (install_static::GetChromeChannel()) {
    case version_info::Channel::STABLE:
      return icon_resources::kFileTypeIndex;
    case version_info::Channel::BETA:
      return icon_resources::kBetaFileTypeIndex;
    case version_info::Channel::DEV:
      return icon_resources::kDevFileTypeIndex;
    case version_info::Channel::CANARY:
      return icon_resources::kSxSFileTypeIndex;
    default:
      NOTREACHED();
      return icon_resources::kFileTypeIndex;
  }
}

#endif  // OFFICIAL_BUILD

}  // namespace

#define HNS_IPFS L"ipfs"
#define HNS_IPNS L"ipns"

#define HNS_BROWSER_PROTOCOL_ASSOCIATIONS HNS_IPFS, HNS_IPNS,

#define HNS_POTENTIAL_PROTOCOL_ASSOCIATIONS HNS_IPFS, HNS_IPNS,

#if defined(OFFICIAL_BUILD)
// Add HnsFile prog id in registry with proper icon.
// This prog id will be referenced from serveral file association reg entry.
#define HNS_GET_CHROME_PROG_ID_ENTRIES                     \
  app_info.prog_id = installer::GetProgIdForFileType();      \
  app_info.file_type_icon_index = GetIconIndexForFileType(); \
  GetProgIdEntries(app_info, entries);

// Give HnsXXFile prog id for some file type.(ex, .pdf or .svg) instead of
// HnsHTML.
#define HNS_GET_APP_EXT_REGISTRATION_ENTRIES                         \
  if (installer::ShouldUseFileTypeProgId(ext)) {                       \
    entries->push_back(std::make_unique<RegistryEntry>(                \
        key_name, installer::GetProgIdForFileType(), std::wstring())); \
    return;                                                            \
  }

// Give HnsXXFile prog id for some file type.(ex, .pdf or .svg) instead of
// HnsHTML.
#define HNS_GET_SHELL_INTEGRATION_ENTRIES                               \
  const std::wstring file_ext = ShellUtil::kPotentialFileAssociations[i]; \
  if (installer::ShouldUseFileTypeProgId(file_ext)) {                     \
    entries->push_back(std::make_unique<RegistryEntry>(                   \
        capabilities + L"\\FileAssociations", file_ext,                   \
        installer::GetProgIdForFileType()));                              \
    continue;                                                             \
  }
#else
#define HNS_GET_CHROME_PROG_ID_ENTRIES
#define HNS_GET_APP_EXT_REGISTRATION_ENTRIES
#define HNS_GET_SHELL_INTEGRATION_ENTRIES
#endif

#include "src/chrome/installer/util/shell_util.cc"
#undef HNS_GET_SHELL_INTEGRATION_ENTRIES
#undef HNS_GET_APP_EXT_REGISTRATION_ENTRIES
#undef HNS_GET_CHROME_PROG_ID_ENTRIES
#undef HNS_IPFS
#undef HNS_IPNS
#undef HNS_POTENTIAL_PROTOCOL_ASSOCIATIONS
#undef HNS_BROWSER_PROTOCOL_ASSOCIATIONS
