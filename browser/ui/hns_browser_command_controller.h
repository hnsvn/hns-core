/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_BROWSER_COMMAND_CONTROLLER_H_
#define HNS_BROWSER_UI_HNS_BROWSER_COMMAND_CONTROLLER_H_

#include <string>

#include "base/memory/raw_ref.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "hns/components/tor/buildflags/buildflags.h"
#include "chrome/browser/ui/browser_command_controller.h"
#include "components/prefs/pref_change_registrar.h"

#if BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/components/hns_vpn/browser/hns_vpn_service_observer.h"
#endif

class HnsAppMenuBrowserTest;
class HnsAppMenuModelBrowserTest;
class HnsBrowserCommandControllerTest;

// This namespace is needed for a chromium_src override
namespace chrome {

class HnsBrowserCommandController : public chrome::BrowserCommandController
#if BUILDFLAG(ENABLE_HNS_VPN)
    ,
                                      public hns_vpn::HnsVPNServiceObserver
#endif
{
 public:
  explicit HnsBrowserCommandController(Browser* browser);
  HnsBrowserCommandController(const HnsBrowserCommandController&) = delete;
  HnsBrowserCommandController& operator=(
      const HnsBrowserCommandController&) = delete;
  ~HnsBrowserCommandController() override;

#if BUILDFLAG(ENABLE_TOR)
  void UpdateCommandForTor();
#endif

 private:
  friend class ::HnsAppMenuBrowserTest;
  friend class ::HnsAppMenuModelBrowserTest;
  friend class ::HnsBrowserCommandControllerTest;

  // Overriden from CommandUpdater:
  bool SupportsCommand(int id) const override;
  bool IsCommandEnabled(int id) const override;
  bool ExecuteCommandWithDisposition(
      int id,
      WindowOpenDisposition disposition,
      base::TimeTicks time_stamp = base::TimeTicks::Now()) override;
  void AddCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(int id, CommandObserver* observer) override;
  void RemoveCommandObserver(CommandObserver* observer) override;
  bool UpdateCommandEnabled(int id, bool state) override;

#if BUILDFLAG(ENABLE_HNS_VPN)
  // hns_vpn::HnsVPNServiceObserver overrides:
  void OnPurchasedStateChanged(
      hns_vpn::mojom::PurchasedState state,
      const absl::optional<std::string>& description) override;
#endif

  void InitHnsCommandState();
  void UpdateCommandForHnsRewards();
  void UpdateCommandForWebcompatReporter();
  void UpdateCommandForHnsSync();
  void UpdateCommandForHnsWallet();
  void UpdateCommandForSidebar();
  void UpdateCommandForHnsVPN();
  void UpdateCommandForPlaylist();
  void UpdateCommandsFroGroups();

  bool ExecuteHnsCommandWithDisposition(int id,
                                          WindowOpenDisposition disposition,
                                          base::TimeTicks time_stamp);
#if BUILDFLAG(ENABLE_HNS_VPN)
  PrefChangeRegistrar hns_vpn_pref_change_registrar_;
#endif
  const raw_ref<Browser> browser_;

  CommandUpdaterImpl hns_command_updater_;
};

}   // namespace chrome

#endif  // HNS_BROWSER_UI_HNS_BROWSER_COMMAND_CONTROLLER_H_
