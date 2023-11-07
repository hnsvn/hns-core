/**
 * Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.omnibox.suggestions;

import android.content.Context;
import android.view.LayoutInflater;

import org.chromium.base.HnsReflectionUtil;
import org.chromium.base.Callback;
import org.chromium.chrome.browser.omnibox.R;
import org.chromium.chrome.browser.omnibox.suggestions.SuggestionListViewBinder.SuggestionListViewHolder;
import org.chromium.chrome.browser.omnibox.suggestions.hns_search.HnsSearchBannerViewBinder;
import org.chromium.ui.ViewProvider;
import org.chromium.ui.modelutil.MVCListAdapter;

import java.util.ArrayList;
import java.util.List;

public class HnsAutocompleteCoordinator {
    public ViewProvider<SuggestionListViewHolder> createViewProvider(
            Context context, MVCListAdapter.ModelList modelList) {
        ViewProvider<SuggestionListViewHolder> provider =
                (ViewProvider<SuggestionListViewHolder>) HnsReflectionUtil.InvokeMethod(
                        AutocompleteCoordinator.class, this, "createViewProvider", Context.class,
                        context, MVCListAdapter.ModelList.class, modelList);

        return new ViewProvider<SuggestionListViewHolder>() {
            private List<Callback<SuggestionListViewHolder>> mCallbacks = new ArrayList<>();
            private SuggestionListViewHolder mHolder;
            @Override
            public void inflate() {
                provider.whenLoaded((holder) -> {
                    OmniboxSuggestionsDropdown dropdown = holder.dropdown;
                    if (dropdown != null && dropdown.getAdapter() != null
                            && dropdown.getAdapter() instanceof OmniboxSuggestionsDropdownAdapter) {
                        ((OmniboxSuggestionsDropdownAdapter) dropdown.getAdapter())
                                .registerType(
                                        HnsOmniboxSuggestionUiType.HNS_SEARCH_PROMO_BANNER,
                                        parent
                                        -> LayoutInflater.from(parent.getContext())
                                                   .inflate(R.layout.omnibox_hns_search_banner,
                                                           null),
                                        HnsSearchBannerViewBinder::bind);

                        mHolder = holder;
                        for (int i = 0; i < mCallbacks.size(); i++) {
                            mCallbacks.get(i).onResult(holder);
                        }
                        mCallbacks = null;
                    }
                });
                provider.inflate();
            }

            @Override
            public void whenLoaded(Callback<SuggestionListViewHolder> callback) {
                if (mHolder != null) {
                    callback.onResult(mHolder);
                    return;
                }
                mCallbacks.add(callback);
            }
        };
    }
}
