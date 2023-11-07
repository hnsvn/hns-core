/**
 * Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.omnibox.suggestions;

import androidx.annotation.IntDef;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

@IntDef({HnsOmniboxSuggestionUiType.HNS_SEARCH_PROMO_BANNER})
@Retention(RetentionPolicy.SOURCE)
public @interface HnsOmniboxSuggestionUiType {
    int HNS_SEARCH_PROMO_BANNER = 10;
}
