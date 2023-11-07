/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_STATUS_TRAY_H_
#define HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_STATUS_TRAY_H_

#include <windows.h>

#include <memory>
#include <string>

#include "hns/browser/hns_vpn/win/hns_vpn_wireguard_service/status_tray/status_icon/scoped_hwnd.h"
#include "ui/gfx/image/image_skia.h"

namespace hns_vpn {

class StatusIcon;

class StatusTray {
 public:
  StatusTray();

  StatusTray(const StatusTray&) = delete;
  StatusTray& operator=(const StatusTray&) = delete;

  ~StatusTray();

  static bool IconWindowExists();

  void CreateStatusIcon(const gfx::ImageSkia& image,
                        const std::u16string& tool_tip);
  StatusIcon* GetStatusIcon();

 private:
  // Static callback invoked when a message comes in to our messaging window.
  static LRESULT CALLBACK WndProcStatic(HWND hwnd,
                                        UINT message,
                                        WPARAM wparam,
                                        LPARAM lparam);
  LRESULT CALLBACK WndProc(HWND hwnd,
                           UINT message,
                           WPARAM wparam,
                           LPARAM lparam);

  // The window class of |window_|.
  ATOM atom_;

  // The handle of the module that contains the window procedure of |window_|.
  HMODULE instance_;

  // The window used for processing events.
  win::ScopedHWND window_;

  // The message ID of the "TaskbarCreated" message, sent to us when we need to
  // reset our status icons.
  UINT taskbar_created_message_;

  std::unique_ptr<StatusIcon> status_icon_;
};

}  // namespace hns_vpn

#endif  // HNS_BROWSER_HNS_VPN_WIN_HNS_VPN_WIREGUARD_SERVICE_STATUS_TRAY_STATUS_ICON_STATUS_TRAY_H_
