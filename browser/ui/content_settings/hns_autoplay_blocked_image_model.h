/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_CONTENT_SETTINGS_HNS_AUTOPLAY_BLOCKED_IMAGE_MODEL_H_
#define HNS_BROWSER_UI_CONTENT_SETTINGS_HNS_AUTOPLAY_BLOCKED_IMAGE_MODEL_H_

#include <memory>

#include "chrome/browser/ui/content_settings/content_setting_image_model.h"

class HnsAutoplayBlockedImageModel : public ContentSettingSimpleImageModel {
 public:
  HnsAutoplayBlockedImageModel();
  HnsAutoplayBlockedImageModel(const HnsAutoplayBlockedImageModel&) =
      delete;
  HnsAutoplayBlockedImageModel& operator=(
      const HnsAutoplayBlockedImageModel&) = delete;
  bool UpdateAndGetVisibility(content::WebContents* web_contents) override;
  std::unique_ptr<ContentSettingBubbleModel> CreateBubbleModelImpl(
      ContentSettingBubbleModel::Delegate* delegate,
      content::WebContents* web_contents) override;
};

#endif  // HNS_BROWSER_UI_CONTENT_SETTINGS_HNS_AUTOPLAY_BLOCKED_IMAGE_MODEL_H_
