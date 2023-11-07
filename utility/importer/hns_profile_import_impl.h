/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_UTILITY_IMPORTER_HNS_PROFILE_IMPORT_IMPL_H_
#define HNS_UTILITY_IMPORTER_HNS_PROFILE_IMPORT_IMPL_H_

#include <memory>
#include <string>

#include "hns/common/importer/profile_import.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/pending_remote.h"
#include "mojo/public/cpp/bindings/receiver.h"

class HnsExternalProcessImporterBridge;
class Importer;

namespace base {
class Thread;
}  // namespace base

namespace importer {
struct SourceProfile;
}  // namespace importer

class HnsProfileImportImpl : public hns::mojom::ProfileImport {
 public:
  explicit HnsProfileImportImpl(
      mojo::PendingReceiver<hns::mojom::ProfileImport> receiver);
  ~HnsProfileImportImpl() override;

  HnsProfileImportImpl(const HnsProfileImportImpl&) = delete;
  HnsProfileImportImpl& operator=(const HnsProfileImportImpl&) = delete;

 private:
  // hns::mojom::ProfileImport overrides:
  void StartImport(
      const importer::SourceProfile& source_profile,
      uint16_t items,
      const base::flat_map<uint32_t, std::string>& localized_strings,
      mojo::PendingRemote<chrome::mojom::ProfileImportObserver> observer,
      mojo::PendingRemote<hns::mojom::ProfileImportObserver> hns_observer)
      override;
  void CancelImport() override;
  void ReportImportItemFinished(importer::ImportItem item) override;

  void ImporterCleanup();

  mojo::Receiver<hns::mojom::ProfileImport> receiver_;
  std::unique_ptr<base::Thread> import_thread_;

  // Bridge object is passed to importer, so that it can send IPC calls
  // directly back to the ProfileImportProcessHost.
  scoped_refptr<HnsExternalProcessImporterBridge> bridge_;

  // A bitmask of importer::ImportItem.
  uint16_t items_to_import_ = 0;

  // Importer of the appropriate type (Firefox, Safari, IE, etc.)
  scoped_refptr<Importer> importer_;
};

#endif  // HNS_UTILITY_IMPORTER_HNS_PROFILE_IMPORT_IMPL_H_
