/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/ethereum_remote_client/ethereum_remote_client_ui.h"

#include <memory>

#include "hns/browser/ui/webui/hns_webui_source.h"
#include "hns/components/ethereum_remote_client_page/resources/grit/ethereum_remote_client_page_generated_map.h"
#include "ui/resources/grit/webui_resources.h"

EthereumRemoteClientUI::EthereumRemoteClientUI(content::WebUI* web_ui,
                                               const std::string& name)
    : WebUIController(web_ui) {
  CreateAndAddWebUIDataSource(web_ui, name, kEthereumRemoteClientPageGenerated,
                              kEthereumRemoteClientPageGeneratedSize,
                              IDR_HNS_WEBUI_ETHEREUM_REMOTE_CLIENT_PAGE_HTML,
                              true /*disable_trusted_types_csp*/);
}

EthereumRemoteClientUI::~EthereumRemoteClientUI() = default;
