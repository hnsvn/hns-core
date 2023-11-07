/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_REWARDS_SOURCE_H_
#define HNS_BROWSER_UI_WEBUI_HNS_REWARDS_SOURCE_H_

#include <string>
#include <vector>

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/bitmap_fetcher/bitmap_fetcher_service.h"
#include "content/public/browser/url_data_source.h"

class GURL;
class Profile;
class SkBitmap;

class HnsRewardsSource : public content::URLDataSource {
 public:
  explicit HnsRewardsSource(Profile* profile);
  HnsRewardsSource(const HnsRewardsSource&) = delete;
  HnsRewardsSource& operator=(const HnsRewardsSource&) = delete;

  ~HnsRewardsSource() override;

  // content::URLDataSource implementation.
  std::string GetSource() override;
  void StartDataRequest(
      const GURL& url,
      const content::WebContents::Getter& wc_getter,
      content::URLDataSource::GotDataCallback got_data_callback)
      override;
  std::string GetMimeType(const GURL& url) override;
  bool AllowCaching() override;
  bool ShouldReplaceExistingSource() override;
  bool ShouldServiceRequest(const GURL& url,
                            content::BrowserContext* browser_context,
                            int render_process_id) override;

 private:
  void OnBitmapFetched(
      content::URLDataSource::GotDataCallback got_data_callback,
      const GURL& url,
      const SkBitmap& bitmap);

  raw_ptr<Profile> profile_ = nullptr;
  std::vector<GURL> resource_fetchers_;

  base::WeakPtrFactory<HnsRewardsSource> weak_factory_{this};
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_REWARDS_SOURCE_H_
