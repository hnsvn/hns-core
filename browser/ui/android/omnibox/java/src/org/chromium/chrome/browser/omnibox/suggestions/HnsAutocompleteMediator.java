/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.omnibox.suggestions;

import android.content.Context;
import android.os.Handler;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import org.chromium.base.Callback;
import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.browser.omnibox.LocationBarDataProvider;
import org.chromium.chrome.browser.omnibox.UrlBarEditingTextStateProvider;
import org.chromium.chrome.browser.omnibox.suggestions.basic.BasicSuggestionProcessor.BookmarkState;
import org.chromium.chrome.browser.omnibox.suggestions.history_clusters.HistoryClustersProcessor.OpenHistoryClustersDelegate;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.profiles.ProfileManager;
import org.chromium.chrome.browser.share.ShareDelegate;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.chrome.browser.tabmodel.TabWindowManager;
import org.chromium.components.omnibox.action.OmniboxActionDelegate;
import org.chromium.components.user_prefs.UserPrefs;
import org.chromium.ui.modaldialog.ModalDialogManager;
import org.chromium.ui.modelutil.PropertyModel;

class HnsAutocompleteMediator extends AutocompleteMediator implements HnsSuggestionHost {
    private static final String AUTOCOMPLETE_ENABLED = "hns.autocomplete_enabled";

    private boolean mNativeInitialized;
    private DropdownItemViewInfoListManager mDropdownViewInfoListManager;

    public HnsAutocompleteMediator(@NonNull Context context,
            @NonNull AutocompleteControllerProvider controllerProvider,
            @NonNull AutocompleteDelegate delegate,
            @NonNull UrlBarEditingTextStateProvider textProvider,
            @NonNull PropertyModel listPropertyModel, @NonNull Handler handler,
            @NonNull Supplier<ModalDialogManager> modalDialogManagerSupplier,
            @NonNull Supplier<Tab> activityTabSupplier,
            @Nullable Supplier<ShareDelegate> shareDelegateSupplier,
            @NonNull LocationBarDataProvider locationBarDataProvider,
            @NonNull Callback<Tab> bringTabToFrontCallback,
            @NonNull Supplier<TabWindowManager> tabWindowManagerSupplier,
            @NonNull BookmarkState bookmarkState,
            @NonNull OmniboxActionDelegate omniboxActionDelegate,
            @NonNull OpenHistoryClustersDelegate openHistoryClustersDelegate) {
        super(context, controllerProvider, delegate, textProvider, listPropertyModel, handler,
                modalDialogManagerSupplier, activityTabSupplier, shareDelegateSupplier,
                locationBarDataProvider, bringTabToFrontCallback, tabWindowManagerSupplier,
                bookmarkState, omniboxActionDelegate, openHistoryClustersDelegate);
    }

    @Override
    public void onTextChanged(String textWithoutAutocomplete) {
        if (ProfileManager.isInitialized()
                && !UserPrefs.get(Profile.getLastUsedRegularProfile())
                            .getBoolean(AUTOCOMPLETE_ENABLED)) {
            return;
        }

        super.onTextChanged(textWithoutAutocomplete);
    }

    @Override
    public void onOmniboxSessionStateChange(boolean activated) {
        if (!mNativeInitialized) return;

        super.onOmniboxSessionStateChange(activated);
    }

    @Override
    public void removeHnsSearchSuggestion() {
        if (mDropdownViewInfoListManager instanceof HnsDropdownItemViewInfoListManager) {
            ((HnsDropdownItemViewInfoListManager) mDropdownViewInfoListManager)
                    .removeHnsSearchSuggestion();
        }
    }
}
