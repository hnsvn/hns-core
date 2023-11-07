/* Copyright 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_PROFILES_HNS_BOOKMARK_MODEL_LOADED_OBSERVER_H_
#define HNS_BROWSER_PROFILES_HNS_BOOKMARK_MODEL_LOADED_OBSERVER_H_

#include "chrome/browser/profiles/bookmark_model_loaded_observer.h"

class HnsBookmarkModelLoadedObserver
    : public BookmarkModelLoadedObserver {
 public:
  explicit HnsBookmarkModelLoadedObserver(Profile* profile);
  HnsBookmarkModelLoadedObserver(const HnsBookmarkModelLoadedObserver&) =
      delete;
  HnsBookmarkModelLoadedObserver& operator=(
      const HnsBookmarkModelLoadedObserver&) = delete;

 private:
  void BookmarkModelLoaded(bookmarks::BookmarkModel* model,
                           bool ids_reassigned) override;
};

#endif  // HNS_BROWSER_PROFILES_HNS_BOOKMARK_MODEL_LOADED_OBSERVER_H_
