/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_shell_integration.h"

#define DefaultBrowserWorker HnsDefaultBrowserWorker
#include "src/chrome/browser/ui/webui/settings/settings_default_browser_handler.cc"
#undef DefaultBrowserWorker
