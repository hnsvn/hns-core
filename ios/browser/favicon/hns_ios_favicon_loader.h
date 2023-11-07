/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_FAVICON_HNS_IOS_FAVICON_LOADER_H_
#define HNS_IOS_BROWSER_FAVICON_HNS_IOS_FAVICON_LOADER_H_

#import <Foundation/Foundation.h>

#include "base/memory/raw_ptr.h"
#include "base/task/cancelable_task_tracker.h"
#include "components/keyed_service/core/keyed_service.h"

namespace favicon {
class FaviconService;
}  // namespace favicon

class GURL;
@class FaviconAttributes;

namespace hns_favicon {

class HnsFaviconLoader : public KeyedService {
 public:
  // Type for completion block for FaviconForURL().
  typedef void (^FaviconAttributesCompletionBlock)(FaviconAttributes*);

  explicit HnsFaviconLoader(favicon::FaviconService* favicon_service);

  HnsFaviconLoader(const HnsFaviconLoader&) = delete;
  HnsFaviconLoader& operator=(const HnsFaviconLoader&) = delete;

  ~HnsFaviconLoader() override;

  void FaviconForPageUrlOrHost(
      const GURL& page_url,
      float size_in_points,
      float min_size_in_points,
      FaviconAttributesCompletionBlock faviconBlockHandler);

  // Cancel all incomplete requests.
  void CancellAllRequests();

  // Return a weak pointer to the current object.
  base::WeakPtr<HnsFaviconLoader> AsWeakPtr();

 private:
  base::raw_ptr<favicon::FaviconService> favicon_service_;

  // Tracks tasks sent to HistoryService.
  base::CancelableTaskTracker cancelable_task_tracker_;

  base::WeakPtrFactory<HnsFaviconLoader> weak_ptr_factory_{this};
};

}  // namespace hns_favicon

#endif  // HNS_IOS_BROWSER_FAVICON_HNS_IOS_FAVICON_LOADER_H_
