/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_BASE_THREADING_THREAD_RESTRICTIONS_H_
#define HNS_CHROMIUM_SRC_BASE_THREADING_THREAD_RESTRICTIONS_H_

#include "hns/components/widevine/static_buildflags.h"

class HnsBrowsingDataRemoverDelegate;
namespace ipfs {
class IpfsService;
}
namespace hns {
class ProcessLauncher;
}

#if BUILDFLAG(WIDEVINE_ARM64_DLL_FIX)
namespace component_updater {
class WidevineArm64DllInstaller;
}
#endif

#define HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_H_BASE \
  friend class ::HnsBrowsingDataRemoverDelegate;     \
  friend class ipfs::IpfsService;                      \
  friend class hns::ProcessLauncher;

#if BUILDFLAG(WIDEVINE_ARM64_DLL_FIX)
// WidevineArm64DllInstaller needs to use TimedWait:
#define HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_WIDEVINE_ARM64_DLL_FIX \
  friend class component_updater::WidevineArm64DllInstaller;
#else
#define HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_WIDEVINE_ARM64_DLL_FIX
#endif

#define HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_H \
  HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_H_BASE  \
  HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_WIDEVINE_ARM64_DLL_FIX

#include "src/base/threading/thread_restrictions.h"  // IWYU pragma: export
#undef HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_H
#undef HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_H_BASE
#undef HNS_SCOPED_ALLOW_BASE_SYNC_PRIMITIVES_WIDEVINE_ARM64_DLL_FIX

#endif  // HNS_CHROMIUM_SRC_BASE_THREADING_THREAD_RESTRICTIONS_H_
