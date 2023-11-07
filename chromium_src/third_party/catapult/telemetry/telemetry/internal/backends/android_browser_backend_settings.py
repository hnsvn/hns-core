# Copyright (c) 2023 The Hns Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# you can obtain one at https://mozilla.org/MPL/2.0/.
"""A inline part of android_browser_backend_settings.py"""

ANDROID_HNS = GenericChromeBackendSettings(browser_type='android-hns',
                                             package='com.hns.browser')

ANDROID_HNS_BETA = GenericChromeBackendSettings(
    browser_type='android-hns-beta', package='com.hns.browser_beta')

ANDROID_HNS_DEV = GenericChromeBackendSettings(
    browser_type='android-hns-dev', package='com.hns.browser_dev')

ANDROID_HNS_NIGHTLY = GenericChromeBackendSettings(
    browser_type='android-hns-nightly', package='com.hns.browser_nightly')

HNS_ANDROID_BACKEND_SETTINGS = (ANDROID_HNS, ANDROID_HNS_BETA,
                                  ANDROID_HNS_DEV, ANDROID_HNS_NIGHTLY)

# Add hns items to chromium ANDROID_BACKEND_SETTINGS:
ANDROID_BACKEND_SETTINGS = HNS_ANDROID_BACKEND_SETTINGS + ANDROID_BACKEND_SETTINGS
