/* Copyright (c) 2018 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/hns_browser_content_setting_bubble_model_delegate.h"

#include "hns/components/constants/url_constants.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_tabstrip.h"

const char kHnsCommunitySupportUrl[] = "https://community.hns.com/";

HnsBrowserContentSettingBubbleModelDelegate::
HnsBrowserContentSettingBubbleModelDelegate(Browser* browser) :
    BrowserContentSettingBubbleModelDelegate(browser),
    browser_(browser) {
}

HnsBrowserContentSettingBubbleModelDelegate::
    ~HnsBrowserContentSettingBubbleModelDelegate() = default;

void
HnsBrowserContentSettingBubbleModelDelegate::ShowWidevineLearnMorePage() {
  GURL learn_more_url = GURL(kWidevineTOS);
  chrome::AddSelectedTabWithURL(browser_, learn_more_url,
                                ui::PAGE_TRANSITION_LINK);
}

void HnsBrowserContentSettingBubbleModelDelegate::ShowLearnMorePage(
    ContentSettingsType type) {
  // TODO(yrliou): Use specific support pages for each content setting type
  GURL learn_more_url(kHnsCommunitySupportUrl);
  chrome::AddSelectedTabWithURL(browser_, learn_more_url,
                                ui::PAGE_TRANSITION_LINK);
}
