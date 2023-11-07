/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_browser_features.h"
#include "build/build_config.h"

namespace features {

// Cleanup Session Cookies on browser restart if Session Restore is enabled.
BASE_FEATURE(kHnsCleanupSessionCookiesOnSessionRestore,
             "HnsCleanupSessionCookiesOnSessionRestore",
             base::FEATURE_ENABLED_BY_DEFAULT);

// Sanitize url before copying, replaces default ctrl+c hotkey for urls.
BASE_FEATURE(kHnsCopyCleanLinkByDefault,
             "hns-copy-clean-link-by-default",
#if BUILDFLAG(IS_MAC)
             base::FEATURE_DISABLED_BY_DEFAULT
#else
             base::FEATURE_ENABLED_BY_DEFAULT
#endif
);

// Disable download warnings for dangerous files when Safe Browsing is
// disabled.
BASE_FEATURE(kHnsOverrideDownloadDangerLevel,
             "hns-override-download-danger-level",
             base::FEATURE_DISABLED_BY_DEFAULT);

}  // namespace features
