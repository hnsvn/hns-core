// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_SHIELDS_PANEL_DATA_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_SHIELDS_PANEL_DATA_HANDLER_H_

#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "hns/browser/ui/hns_shields_data_controller.h"
#include "hns/components/hns_shields/common/hns_shields_panel.mojom.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"

class TabStripModel;

namespace ui {
class MojoBubbleWebUIController;
}  // namespace ui

class ShieldsPanelDataHandler
    : public hns_shields::mojom::DataHandler,
      public hns_shields::HnsShieldsDataController::Observer,
      public TabStripModelObserver {
 public:
  ShieldsPanelDataHandler(
      mojo::PendingReceiver<hns_shields::mojom::DataHandler>
          data_handler_receiver,
      ui::MojoBubbleWebUIController* webui_controller,
      TabStripModel* browser);

  ShieldsPanelDataHandler(const ShieldsPanelDataHandler&) = delete;
  ShieldsPanelDataHandler& operator=(const ShieldsPanelDataHandler&) = delete;
  ~ShieldsPanelDataHandler() override;

  // mojom::DataHandler
  void RegisterUIHandler(mojo::PendingRemote<hns_shields::mojom::UIHandler>
                             ui_handler_receiver) override;
  void GetSiteBlockInfo(GetSiteBlockInfoCallback callback) override;
  void GetSiteSettings(GetSiteSettingsCallback callback) override;
  void SetAdBlockMode(AdBlockMode callback) override;
  void SetFingerprintMode(FingerprintMode mode) override;
  void SetCookieBlockMode(CookieBlockMode mode) override;
  void SetHttpsUpgradeMode(HttpsUpgradeMode mode) override;
  void SetIsNoScriptsEnabled(bool is_enabled) override;
  void SetHTTPSEverywhereEnabled(bool is_enabled) override;
  void SetHnsShieldsEnabled(bool is_enabled) override;
  void SetForgetFirstPartyStorageEnabled(bool is_enabled) override;
  void OpenWebCompatWindow() override;
  void UpdateFavicon() override;
  void AllowScriptsOnce(const std::vector<std::string>& origins) override;
  void BlockAllowedScripts(const std::vector<std::string>& origins) override;

 private:
  void UpdateSiteBlockInfo();

  // HnsShieldsDataController::Observer
  void OnResourcesChanged() override;
  void OnFaviconUpdated() override;

  // TabStripModelObserver
  void OnTabStripModelChanged(
      TabStripModel* tab_strip_model,
      const TabStripModelChange& change,
      const TabStripSelectionChange& selection) override;

  mojo::Receiver<hns_shields::mojom::DataHandler> data_handler_receiver_;
  mojo::Remote<hns_shields::mojom::UIHandler> ui_handler_remote_;
  raw_ptr<ui::MojoBubbleWebUIController> const webui_controller_ = nullptr;
  raw_ptr<hns_shields::HnsShieldsDataController>
      active_shields_data_controller_ = nullptr;

  hns_shields::mojom::SiteBlockInfo site_block_info_;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_SHIELDS_SHIELDS_PANEL_DATA_HANDLER_H_
