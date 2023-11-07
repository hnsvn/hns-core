// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/common/channel_info.h"

#include "base/environment.h"
#include "base/strings/strcat.h"
#include "base/strings/string_util.h"
#include "hns/common/hns_channel_info_posix.h"
#include "build/build_config.h"
#include "components/version_info/version_info.h"

namespace chrome {

std::string GetChannelName(WithExtendedStable with_extended_stable) {
  std::string modifier;
  hns::GetChannelImpl(&modifier, nullptr);
  return modifier;
}

std::string GetChannelSuffixForDataDir() {
  return std::string();
}

#if BUILDFLAG(IS_LINUX)
std::string GetChannelSuffixForExtraFlagsEnvVarName() {
#if defined(OFFICIAL_BUILD)
  version_info::Channel product_channel(chrome::GetChannel());
  switch (product_channel) {
    case version_info::Channel::DEV:
      return "_DEV";
    case version_info::Channel::BETA:
      return "_BETA";
    case version_info::Channel::CANARY:
      return "_NIGHTLY";
    case version_info::Channel::STABLE:
      return "_STABLE";
    default:
      return std::string();
  }
#else   // defined(OFFICIAL_BUILD)
  const char* const channel_name = getenv("CHROME_VERSION_EXTRA");
  return channel_name
             ? base::StrCat(
                   {"_", base::ToUpperASCII(base::StringPiece(channel_name))})
             : std::string();
#endif  // defined(OFFICIAL_BUILD)
}
#endif  // BUILDFLAG(IS_LINUX)

#if BUILDFLAG(IS_LINUX)
std::string GetDesktopName(base::Environment* env) {
#if defined(OFFICIAL_BUILD)
  version_info::Channel product_channel(chrome::GetChannel());
  switch (product_channel) {
    case version_info::Channel::DEV:
      return "hns-browser-dev.desktop";
    case version_info::Channel::BETA:
      return "hns-browser-beta.desktop";
    case version_info::Channel::CANARY:
      return "hns-browser-nightly.desktop";
    default:
      return "hns-browser.desktop";
  }
#else  // defined(OFFICIAL_BUILD)
  // Allow $CHROME_DESKTOP to override the built-in value, so that development
  // versions can set themselves as the default without interfering with
  // non-official, packaged versions using the built-in value.
  std::string name;
  if (env->GetVar("CHROME_DESKTOP", &name) && !name.empty())
    return name;
  return "hns-browser.desktop";
#endif
}
#endif  // BUILDFLAG(IS_LINUX)

version_info::Channel GetChannel() {
  return hns::GetChannelImpl(nullptr, nullptr);
}

bool IsExtendedStableChannel() {
  // No extended stable channel for Hns.
  return false;
}

}  // namespace chrome
