/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_IMPORTER_HNS_IN_PROCESS_IMPORTER_BRIDGE_H_
#define HNS_BROWSER_IMPORTER_HNS_IN_PROCESS_IMPORTER_BRIDGE_H_

#include <string>

#include "hns/common/importer/hns_importer_bridge.h"
#include "chrome/browser/importer/in_process_importer_bridge.h"

class HnsInProcessImporterBridge : public InProcessImporterBridge,
                                     public HnsImporterBridge {
 public:
  using InProcessImporterBridge::InProcessImporterBridge;

  HnsInProcessImporterBridge(const HnsInProcessImporterBridge&) = delete;
  HnsInProcessImporterBridge operator=(
      const HnsInProcessImporterBridge&) = delete;

  // HnsImporterBridge overrides:
  void SetCreditCard(const std::u16string& name_on_card,
                     const std::u16string& expiration_month,
                     const std::u16string& expiration_year,
                     const std::u16string& decrypted_card_number,
                     const std::string& origin) override;

 private:
  ~HnsInProcessImporterBridge() override;
};

#endif  // HNS_BROWSER_IMPORTER_HNS_IN_PROCESS_IMPORTER_BRIDGE_H_
