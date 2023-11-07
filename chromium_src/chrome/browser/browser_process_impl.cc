/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "extensions/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_EXTENSIONS)
#include "hns/browser/extensions/hns_extensions_browser_client_impl.h"
#define ChromeExtensionsBrowserClient HnsExtensionsBrowserClientImpl
#endif
#include "src/chrome/browser/browser_process_impl.cc"
#if BUILDFLAG(ENABLE_EXTENSIONS)
#undef ChromeExtensionsBrowserClient
#endif
