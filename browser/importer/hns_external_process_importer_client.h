/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_IMPORTER_HNS_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
#define HNS_BROWSER_IMPORTER_HNS_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "hns/common/importer/profile_import.mojom.h"
#include "chrome/browser/importer/external_process_importer_client.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "mojo/public/cpp/bindings/remote.h"

class HnsExternalProcessImporterClient
    : public ExternalProcessImporterClient,
      public hns::mojom::ProfileImportObserver {
 public:
  HnsExternalProcessImporterClient(
      base::WeakPtr<ExternalProcessImporterHost> importer_host,
      const importer::SourceProfile& source_profile,
      uint16_t items,
      InProcessImporterBridge* bridge);

  HnsExternalProcessImporterClient(
      const HnsExternalProcessImporterClient&) = delete;
  HnsExternalProcessImporterClient& operator=(
      const HnsExternalProcessImporterClient&) = delete;

  // ExternalProcessImportClient overrides:
  void Start() override;
  void Cancel() override;
  void CloseMojoHandles() override;
  void OnImportItemFinished(importer::ImportItem import_item) override;

  // hns::mojom::ProfileImportObserver overrides:
  void OnCreditCardImportReady(const std::u16string& name_on_card,
                               const std::u16string& expiration_month,
                               const std::u16string& expiration_year,
                               const std::u16string& decrypted_card_number,
                               const std::string& origin) override;

 protected:
  ~HnsExternalProcessImporterClient() override;

 private:
  // Used to start and stop the actual hns importer running in a different
  // process.
  mojo::Remote<hns::mojom::ProfileImport> hns_profile_import_;

  // Used to receive progress updates from the hns importer.
  mojo::Receiver<hns::mojom::ProfileImportObserver> hns_receiver_{this};
};

#endif  // HNS_BROWSER_IMPORTER_HNS_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
