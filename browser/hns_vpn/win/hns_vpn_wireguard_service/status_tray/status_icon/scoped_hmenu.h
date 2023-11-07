/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_SCOPED_HMENU_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_SCOPED_HMENU_H_

#include <windows.h>

#include "base/scoped_generic.h"

namespace hns_vpn {

namespace win {

namespace internal {

template <class T>
struct ScopedHMENUObjectTraits {
  static T InvalidValue() { return nullptr; }
  static void Free(T object) { DestroyMenu(object); }
};

}  // namespace internal

template <class T>
using ScopedHMENUObject =
    base::ScopedGeneric<T, internal::ScopedHMENUObjectTraits<T>>;

typedef ScopedHMENUObject<HMENU> ScopedHMENU;

}  // namespace win
}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_SCOPED_HMENU_H_
