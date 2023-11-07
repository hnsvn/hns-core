/**
 * Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.omnibox.suggestions.hns_search;

import android.content.Context;

import org.chromium.chrome.browser.omnibox.R;
import org.chromium.chrome.browser.omnibox.UrlBarEditingTextStateProvider;
import org.chromium.chrome.browser.omnibox.suggestions.AutocompleteDelegate;
import org.chromium.chrome.browser.omnibox.suggestions.HnsOmniboxSuggestionUiType;
import org.chromium.chrome.browser.omnibox.suggestions.HnsSuggestionHost;
import org.chromium.chrome.browser.omnibox.suggestions.DropdownItemProcessor;
import org.chromium.ui.base.PageTransition;
import org.chromium.ui.modelutil.PropertyModel;

/** A class that handles model and view creation for the suggestion hns search banner. */
public class HnsSearchBannerProcessor implements DropdownItemProcessor {
    private final HnsSuggestionHost mSuggestionHost;
    private final int mMinimumHeight;
    private final UrlBarEditingTextStateProvider mUrlBarEditingTextProvider;
    private final AutocompleteDelegate mUrlBarDelegate;

    /**
     * @param context An Android context.
     * @param suggestionHost A handle to the object using the suggestions.
     */
    public HnsSearchBannerProcessor(Context context, HnsSuggestionHost suggestionHost,
            UrlBarEditingTextStateProvider editingTextProvider, AutocompleteDelegate urlDelegate) {
        mSuggestionHost = suggestionHost;
        mUrlBarEditingTextProvider = editingTextProvider;
        mUrlBarDelegate = urlDelegate;
        mMinimumHeight = context.getResources().getDimensionPixelSize(
                R.dimen.omnibox_hns_search_banner_height);
    }

    public void populateModel(final PropertyModel model) {
        model.set(HnsSearchBannerProperties.DELEGATE, new HnsSearchBannerProperties.Delegate() {
            @Override
            public void onPositiveClicked() {
                mUrlBarDelegate.loadUrl("https://search.hns.com/search?q="
                                + mUrlBarEditingTextProvider.getTextWithoutAutocomplete()
                                + "&action=makeDefault",
                        PageTransition.LINK, System.currentTimeMillis(), /*openInNewTab=*/false);
            }

            @Override
            public void onNegativeClicked() {
                mSuggestionHost.removeHnsSearchSuggestion();
            }
        });
    }

    @Override
    public int getViewTypeId() {
        return HnsOmniboxSuggestionUiType.HNS_SEARCH_PROMO_BANNER;
    }

    @Override
    public int getMinimumViewHeight() {
        return mMinimumHeight;
    }

    @Override
    public PropertyModel createModel() {
        return new PropertyModel(HnsSearchBannerProperties.ALL_KEYS);
    }

    @Override
    public void onOmniboxSessionStateChange(boolean activated) {}

    @Override
    public void onNativeInitialized() {}

    @Override
    public boolean allowBackgroundRounding() {
        return false;
    }
}
