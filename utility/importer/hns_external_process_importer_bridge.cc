/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/utility/importer/hns_external_process_importer_bridge.h"

#include <utility>

HnsExternalProcessImporterBridge::HnsExternalProcessImporterBridge(
    const base::flat_map<uint32_t, std::string>& localized_strings,
    mojo::SharedRemote<chrome::mojom::ProfileImportObserver> observer,
    mojo::SharedRemote<hns::mojom::ProfileImportObserver> hns_observer)
    : ExternalProcessImporterBridge(std::move(localized_strings),
                                         std::move(observer)),
      hns_observer_(std::move(hns_observer)) {}

HnsExternalProcessImporterBridge::
    ~HnsExternalProcessImporterBridge() = default;

void HnsExternalProcessImporterBridge::SetCreditCard(
    const std::u16string& name_on_card,
    const std::u16string& expiration_month,
    const std::u16string& expiration_year,
    const std::u16string& decrypted_card_number,
    const std::string& origin) {
  hns_observer_->OnCreditCardImportReady(
      name_on_card, expiration_month,
      expiration_year, decrypted_card_number,
      origin);
}
