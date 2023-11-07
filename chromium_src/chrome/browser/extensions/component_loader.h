/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_COMPONENT_LOADER_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_COMPONENT_LOADER_H_

#define AddDefaultComponentExtensions virtual AddDefaultComponentExtensions
#define AddHangoutServicesExtension virtual AddHangoutServicesExtension
#define AddNetworkSpeechSynthesisExtension    \
  AddNetworkSpeechSynthesisExtensionUnused(); \
  friend class HnsComponentLoader;          \
  void AddNetworkSpeechSynthesisExtension

#include "src/chrome/browser/extensions/component_loader.h"  // IWYU pragma: export
#undef AddDefaultComponentExtensions
#undef AddHangoutServicesExtension
#undef AddNetworkSpeechSynthesisExtension

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_EXTENSIONS_COMPONENT_LOADER_H_
