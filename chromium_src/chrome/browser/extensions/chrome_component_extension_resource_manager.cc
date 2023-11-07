/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "chrome/browser/extensions/chrome_component_extension_resource_manager.h"

#include "hns/components/hns_extension/grit/hns_extension_generated_map.h"
#include "hns/components/hns_extension/grit/hns_extension_resources_map.h"
#include "hns/components/hns_webtorrent/browser/buildflags/buildflags.h"

#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
#include "hns/components/hns_webtorrent/grit/hns_webtorrent_generated_map.h"
#include "hns/components/hns_webtorrent/grit/hns_webtorrent_resources_map.h"
#endif

#if BUILDFLAG(ENABLE_HNS_WEBTORRENT)
#define HNS_WEBTORRENT_RESOURCES                            \
  AddComponentResourceEntries(kHnsWebtorrentResources,      \
                              kHnsWebtorrentResourcesSize); \
  AddComponentResourceEntries(kHnsWebtorrentGenerated,      \
                              kHnsWebtorrentGeneratedSize);
#else
#define HNS_WEBTORRENT_RESOURCES
#endif

#define HNS_CHROME_COMPONENT_EXTENSION_RESOURCE_MANAGER_DATA_DATA  \
  AddComponentResourceEntries(kHnsExtension, kHnsExtensionSize); \
  AddComponentResourceEntries(kHnsExtensionGenerated,              \
                              kHnsExtensionGeneratedSize);         \
  HNS_WEBTORRENT_RESOURCES

#include "src/chrome/browser/extensions/chrome_component_extension_resource_manager.cc"
#undef HNS_CHROME_COMPONENT_EXTENSION_RESOURCE_MANAGER_DATA_DATA
