/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/content_settings/hns_autoplay_blocked_image_model.h"

#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/tabs/tab_strip_model.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/content_settings/browser/page_specific_content_settings.h"
#include "components/content_settings/core/common/content_settings_types.h"
#include "content/public/test/browser_test.h"

using content::WebContents;
using ImageType = ContentSettingImageModel::ImageType;

typedef InProcessBrowserTest HnsAutoplayBlockedImageModelTest;

IN_PROC_BROWSER_TEST_F(HnsAutoplayBlockedImageModelTest, CreateBubbleModel) {
  WebContents* web_contents =
      browser()->tab_strip_model()->GetActiveWebContents();
  content_settings::PageSpecificContentSettings* content_settings =
      content_settings::PageSpecificContentSettings::GetForFrame(
          web_contents->GetPrimaryMainFrame());
  content_settings->BlockAllContentForTesting();

  auto model = std::make_unique<HnsAutoplayBlockedImageModel>();
  std::unique_ptr<ContentSettingBubbleModel> bubble(
    model->CreateBubbleModel(nullptr, web_contents));

  ContentSettingSimpleBubbleModel* simple_bubble =
    bubble->AsSimpleBubbleModel();
  ASSERT_TRUE(simple_bubble);
  EXPECT_EQ(static_cast<ContentSettingSimpleImageModel*>(model.get())
            ->content_type(),
            simple_bubble->content_type());
  EXPECT_EQ(ImageType::MEDIASTREAM, model->image_type());
  EXPECT_EQ(ContentSettingsType::AUTOPLAY, model->content_type());
}
