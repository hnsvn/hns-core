/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/hns_shields/cookie_list_opt_in_page_handler.h"

#include <utility>

#include "base/metrics/histogram_functions.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/components/hns_shields/browser/ad_block_regional_service_manager.h"
#include "hns/components/hns_shields/browser/ad_block_service.h"
#include "hns/components/hns_shields/common/hns_shield_constants.h"

CookieListOptInPageHandler::CookieListOptInPageHandler(
    mojo::PendingReceiver<hns_shields::mojom::CookieListOptInPageHandler>
        receiver,
    base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder,
    Profile* profile)
    : receiver_(this, std::move(receiver)),
      embedder_(embedder),
      profile_(profile) {
  DCHECK(embedder_);
  DCHECK(profile_);
}

CookieListOptInPageHandler::~CookieListOptInPageHandler() = default;

void CookieListOptInPageHandler::ShowUI() {
  base::UmaHistogramExactLinear(hns_shields::kCookieListPromptHistogram, 1,
                                4);
  if (embedder_) {
    embedder_->ShowUI();
  }
}

void CookieListOptInPageHandler::CloseUI() {
  if (embedder_) {
    embedder_->CloseUI();
  }
}

void CookieListOptInPageHandler::EnableFilter() {
  g_hns_browser_process->ad_block_service()
      ->regional_service_manager()
      ->EnableFilterList(hns_shields::kCookieListUuid, true);
}

void CookieListOptInPageHandler::OnUINoClicked() {
  base::UmaHistogramExactLinear(hns_shields::kCookieListPromptHistogram, 2,
                                4);
}

void CookieListOptInPageHandler::OnUIYesClicked() {
  base::UmaHistogramExactLinear(hns_shields::kCookieListPromptHistogram, 3,
                                4);
}
