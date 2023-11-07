/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_UTILITY_IMPORTER_HNS_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_
#define HNS_UTILITY_IMPORTER_HNS_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_

#include <string>

#include "hns/common/importer/hns_importer_bridge.h"
#include "hns/common/importer/profile_import.mojom.h"
#include "chrome/utility/importer/external_process_importer_bridge.h"

class HnsExternalProcessImporterBridge : public ExternalProcessImporterBridge,
                                           public HnsImporterBridge {
 public:
  HnsExternalProcessImporterBridge(
      const base::flat_map<uint32_t, std::string>& localized_strings,
      mojo::SharedRemote<chrome::mojom::ProfileImportObserver> observer,
      mojo::SharedRemote<hns::mojom::ProfileImportObserver> hns_observer);

  HnsExternalProcessImporterBridge(
      const HnsExternalProcessImporterBridge&) = delete;
  HnsExternalProcessImporterBridge& operator=(
      const HnsExternalProcessImporterBridge&) = delete;

  void SetCreditCard(const std::u16string& name_on_card,
                     const std::u16string& expiration_month,
                     const std::u16string& expiration_year,
                     const std::u16string& decrypted_card_number,
                     const std::string& origin) override;

 private:
  ~HnsExternalProcessImporterBridge() override;

  mojo::SharedRemote<hns::mojom::ProfileImportObserver> hns_observer_;
};

#endif  // HNS_UTILITY_IMPORTER_HNS_EXTERNAL_PROCESS_IMPORTER_BRIDGE_H_
