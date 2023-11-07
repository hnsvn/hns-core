/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_CONTENT_SETTINGS_HNS_AUTOPLAY_CONTENT_SETTING_BUBBLE_MODEL_H_
#define HNS_BROWSER_UI_CONTENT_SETTINGS_HNS_AUTOPLAY_CONTENT_SETTING_BUBBLE_MODEL_H_

#include "chrome/browser/ui/content_settings/content_setting_bubble_model.h"

class Profile;

using content::WebContents;

class HnsAutoplayContentSettingBubbleModel
    : public ContentSettingSimpleBubbleModel {
 public:
  HnsAutoplayContentSettingBubbleModel(Delegate* delegate,
                                         WebContents* web_contents);
  HnsAutoplayContentSettingBubbleModel(
      const HnsAutoplayContentSettingBubbleModel&) = delete;
  HnsAutoplayContentSettingBubbleModel& operator=(
      const HnsAutoplayContentSettingBubbleModel&) = delete;
  ~HnsAutoplayContentSettingBubbleModel() override;

  // ContentSettingSimpleBubbleModel:
  void CommitChanges() override;

 protected:
  bool settings_changed() const;

 private:
  void SetTitle();
  void SetRadioGroup();
  void SetNarrowestContentSetting(ContentSetting setting);

  ContentSetting block_setting_;
};

#endif  // HNS_BROWSER_UI_CONTENT_SETTINGS_HNS_AUTOPLAY_CONTENT_SETTING_BUBBLE_MODEL_H_
