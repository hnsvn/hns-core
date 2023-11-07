/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_IMPORT_BULK_DATA_HANDLER_H_
#define HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_IMPORT_BULK_DATA_HANDLER_H_

#include <memory>

#include "base/containers/flat_set.h"
#include "base/functional/callback.h"
#include "base/memory/weak_ptr.h"
#include "hns/browser/ui/webui/settings/hns_import_data_handler.h"
#include "hns/browser/ui/webui/settings/hns_importer_observer.h"
#include "build/build_config.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace settings {

// This class handles bulk requests to import multiple profiles to
// new target Hns profiles.
class HnsImportBulkDataHandler : public HnsImportDataHandler {
 public:
  HnsImportBulkDataHandler();
  ~HnsImportBulkDataHandler() override;

  using ProfileReadyCallback = base::OnceCallback<void(Profile* profile)>;

  HnsImportBulkDataHandler(const HnsImportBulkDataHandler&) = delete;
  HnsImportBulkDataHandler& operator=(const HnsImportBulkDataHandler&) =
      delete;

 protected:
  void HandleImportDataBulk(const base::Value::List& args);

  absl::optional<int> GetProfileIndex(
      const importer::SourceProfile& source_profile);

  void PrepareProfile(const std::u16string& name,
                      ProfileReadyCallback callback);

  void ProfileReadyForImport(const importer::SourceProfile& source_profile,
                             uint16_t imported_items,
                             Profile* profile);
  // HnsImportDataHandler
  void NotifyImportProgress(const importer::SourceProfile& source_profile,
                            const base::Value::Dict& info) override;
  void OnImportEnded(const importer::SourceProfile& source_profile) override;

  // SettingsPageUIHandler
  void RegisterMessages() override;

  // ImportDataHandler overrides:
  void StartImport(const importer::SourceProfile& source_profile,
                   uint16_t imported_items) override;

 private:
  base::flat_set<int> importing_profiles_;
  base::WeakPtrFactory<HnsImportBulkDataHandler> weak_factory_{this};
};

}  // namespace settings

#endif  // HNS_BROWSER_UI_WEBUI_SETTINGS_HNS_IMPORT_BULK_DATA_HANDLER_H_
