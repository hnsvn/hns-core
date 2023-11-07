/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_DOWNLOAD_HNS_DOWNLOAD_ITEM_MODEL_H_
#define HNS_BROWSER_DOWNLOAD_HNS_DOWNLOAD_ITEM_MODEL_H_

#include <string>

#include "base/memory/raw_ptr.h"

// The purpose of this class is to extend DonwloadItemModel's class
// functionality by adding a method for the origin URL text and
// a method that returns tool tip text that includes origin URL.
// This class, however, doesn't inherit from DownloadItemModel because
// DownloadItemView has a member of DownloadItemModel type. To use this
// extended functionality, HnsDownloadItemView will have this model as
// a member.
class DownloadUIModel;

class HnsDownloadItemModel {
 public:
  // Constructs a HnsDownloadItemModel that encapsulates DownloadItemModel.
  explicit HnsDownloadItemModel(DownloadUIModel* model);
  HnsDownloadItemModel(const HnsDownloadItemModel&) = delete;
  HnsDownloadItemModel& operator=(const HnsDownloadItemModel&) = delete;
  ~HnsDownloadItemModel();

  // Method that returns a string suitable for use as a tooltip. For
  // a regular download, the tooltip is the filename and the origin URL with an
  // indicator if the URL is secure. For an interrupted download, falls back on
  // the base class behavior.
  std::u16string GetTooltipText();

  // Returns a string suitable for use as the origin URL. |is_secure| is set to
  // true if the url is considered secure.
  std::u16string GetOriginURLText(bool* is_secure);

  // Encapsulated model.
  raw_ptr<DownloadUIModel> model_ = nullptr;
};

#endif  // HNS_BROWSER_DOWNLOAD_HNS_DOWNLOAD_ITEM_MODEL_H_
