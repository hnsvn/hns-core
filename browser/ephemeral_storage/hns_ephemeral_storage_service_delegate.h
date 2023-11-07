/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EPHEMERAL_STORAGE_HNS_EPHEMERAL_STORAGE_SERVICE_DELEGATE_H_
#define HNS_BROWSER_EPHEMERAL_STORAGE_HNS_EPHEMERAL_STORAGE_SERVICE_DELEGATE_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "base/memory/scoped_refptr.h"
#include "hns/components/ephemeral_storage/ephemeral_storage_service_delegate.h"
#include "components/content_settings/core/browser/cookie_settings.h"

namespace content {
class BrowserContext;
}

namespace ephemeral_storage {

class HnsEphemeralStorageServiceDelegate
    : public EphemeralStorageServiceDelegate {
 public:
  HnsEphemeralStorageServiceDelegate(
      content::BrowserContext* context,
      scoped_refptr<content_settings::CookieSettings> cookie_settings);
  ~HnsEphemeralStorageServiceDelegate() override;

  void CleanupTLDEphemeralArea(const TLDEphemeralAreaKey& key) override;
  void CleanupFirstPartyStorageArea(
      const std::string& registerable_domain) override;

 private:
  raw_ptr<content::BrowserContext> context_ = nullptr;
  scoped_refptr<content_settings::CookieSettings> cookie_settings_;
};

}  // namespace ephemeral_storage

#endif  // HNS_BROWSER_EPHEMERAL_STORAGE_HNS_EPHEMERAL_STORAGE_SERVICE_DELEGATE_H_
