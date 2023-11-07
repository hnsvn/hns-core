/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_CONSTANTS_URL_CONSTANTS_H_
#define HNS_COMPONENTS_CONSTANTS_URL_CONSTANTS_H_

extern const char kChromeExtensionScheme[];
extern const char kHnsUIScheme[];
extern const char kMagnetScheme[];
extern const char kWidevineTOS[];
extern const char kRewardsUpholdSupport[];
extern const char kP3ALearnMoreURL[];
extern const char kP3ASettingsLink[];
extern const char kImportDataHelpURL[];
extern const char kCryptoWalletsLearnMoreURL[];
extern const char kPermissionPromptLearnMoreUrl[];
extern const char kPermissionPromptHardwareAccessPrivacyRisksURL[];
extern const char kSpeedreaderLearnMoreUrl[];
extern const char kWebDiscoveryLearnMoreUrl[];
extern const char kHnsSearchHost[];
extern const char kWidevineLearnMoreUrl[];

// This is introduced to replace |kDownloadChromeUrl| in
// outdated_upgrade_bubble_view.cc"
// |kDownloadChromeUrl| couldn't be replaced with char array because array
// should be initialized with initialize list or string literal.
// So, this macro is used.
#define kDownloadHnsUrl "https://www.hns.com/download"

#endif  // HNS_COMPONENTS_CONSTANTS_URL_CONSTANTS_H_
