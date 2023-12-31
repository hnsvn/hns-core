/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "chrome/browser/ui/webui/settings/browser_lifetime_handler.h"

#include "hns/browser/sparkle_buildflags.h"

#if BUILDFLAG(ENABLE_SPARKLE)
#include "hns/browser/ui/webui/settings/hns_relaunch_handler_mac.h"
#endif

#define BrowserLifetimeHandler BrowserLifetimeHandler_ChromiumImpl
#include "src/chrome/browser/ui/webui/settings/browser_lifetime_handler.cc"
#undef BrowserLifetimeHandler

namespace settings {

BrowserLifetimeHandler::~BrowserLifetimeHandler() {}

void BrowserLifetimeHandler::HandleRelaunch(const base::Value::List& args) {
#if BUILDFLAG(ENABLE_SPARKLE)
  hns_relaunch_handler::RelaunchOnMac();
#else
  BrowserLifetimeHandler_ChromiumImpl::HandleRelaunch(args);
#endif
}

}  // namespace settings
