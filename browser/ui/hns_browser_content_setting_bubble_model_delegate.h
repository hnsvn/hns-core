/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_HNS_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_
#define HNS_BROWSER_UI_HNS_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_

#include "base/memory/raw_ptr.h"
#include "chrome/browser/ui/browser_content_setting_bubble_model_delegate.h"

class HnsBrowserContentSettingBubbleModelDelegate
    : public BrowserContentSettingBubbleModelDelegate {
 public:
  explicit HnsBrowserContentSettingBubbleModelDelegate(Browser* browser);
  HnsBrowserContentSettingBubbleModelDelegate(
      const HnsBrowserContentSettingBubbleModelDelegate&) = delete;
  HnsBrowserContentSettingBubbleModelDelegate& operator=(
      const HnsBrowserContentSettingBubbleModelDelegate&) = delete;
  ~HnsBrowserContentSettingBubbleModelDelegate() override;

  void ShowWidevineLearnMorePage();
  void ShowLearnMorePage(ContentSettingsType type) override;

 private:
  const raw_ptr<Browser> browser_;
};

#endif  // HNS_BROWSER_UI_HNS_BROWSER_CONTENT_SETTING_BUBBLE_MODEL_DELEGATE_H_
