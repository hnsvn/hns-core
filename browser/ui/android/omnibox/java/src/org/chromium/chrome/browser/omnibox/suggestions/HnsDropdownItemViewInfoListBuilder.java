/**
 * Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/.
 */

package org.chromium.chrome.browser.omnibox.suggestions;

import android.content.Context;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.annotation.Px;

import org.chromium.base.HnsFeatureList;
import org.chromium.base.supplier.Supplier;
import org.chromium.chrome.browser.flags.ChromeFeatureList;
import org.chromium.chrome.browser.omnibox.OmniboxPrefManager;
import org.chromium.chrome.browser.omnibox.UrlBarEditingTextStateProvider;
import org.chromium.chrome.browser.omnibox.suggestions.basic.BasicSuggestionProcessor.BookmarkState;
import org.chromium.chrome.browser.omnibox.suggestions.hns_search.HnsSearchBannerProcessor;
import org.chromium.chrome.browser.omnibox.suggestions.history_clusters.HistoryClustersProcessor.OpenHistoryClustersDelegate;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.search_engines.settings.HnsSearchEngineAdapter;
import org.chromium.chrome.browser.tab.Tab;
import org.chromium.components.omnibox.AutocompleteResult;
import org.chromium.components.omnibox.GroupsProto.GroupConfig;
import org.chromium.ui.modelutil.PropertyModel;

import java.util.Arrays;
import java.util.List;
import java.util.Locale;

class HnsDropdownItemViewInfoListBuilder extends DropdownItemViewInfoListBuilder {
    private @Nullable HnsSearchBannerProcessor mHnsSearchBannerProcessor;
    private UrlBarEditingTextStateProvider mUrlBarEditingTextProvider;
    private @NonNull Supplier<Tab> mActivityTabSupplier;
    private static final List<String> mHnsSearchEngineDefaultRegions =
            Arrays.asList("CA", "DE", "FR", "GB", "US", "AT", "ES", "MX");
    @Px
    private static final int DROPDOWN_HEIGHT_UNKNOWN = -1;
    private static final int DEFAULT_SIZE_OF_VISIBLE_GROUP = 5;
    private Context mContext;

    HnsDropdownItemViewInfoListBuilder(@NonNull Supplier<Tab> tabSupplier,
            BookmarkState bookmarkState, OpenHistoryClustersDelegate openHistoryClustersDelegate) {
        super(tabSupplier, bookmarkState, openHistoryClustersDelegate);

        mActivityTabSupplier = tabSupplier;
    }

    @Override
    void initDefaultProcessors(Context context, SuggestionHost host, AutocompleteDelegate delegate,
            UrlBarEditingTextStateProvider textProvider) {
        mContext = context;
        mUrlBarEditingTextProvider = textProvider;
        super.initDefaultProcessors(context, host, delegate, textProvider);
        if (host instanceof HnsSuggestionHost) {
            mHnsSearchBannerProcessor = new HnsSearchBannerProcessor(
                    context, (HnsSuggestionHost) host, textProvider, delegate);
        }
    }

    @Override
    void onOmniboxSessionStateChange(boolean activated) {
        super.onOmniboxSessionStateChange(activated);
        mHnsSearchBannerProcessor.onOmniboxSessionStateChange(activated);
    }

    @Override
    void onNativeInitialized() {
        super.onNativeInitialized();
        mHnsSearchBannerProcessor.onNativeInitialized();
    }

    @Override
    @NonNull
    List<DropdownItemViewInfo> buildDropdownViewInfoList(AutocompleteResult autocompleteResult) {
        mHnsSearchBannerProcessor.onSuggestionsReceived();
        List<DropdownItemViewInfo> viewInfoList =
                super.buildDropdownViewInfoList(autocompleteResult);

        if (isHnsSearchPromoBanner()) {
            final PropertyModel model = mHnsSearchBannerProcessor.createModel();
            mHnsSearchBannerProcessor.populateModel(model);
            viewInfoList.add(new DropdownItemViewInfo(
                    mHnsSearchBannerProcessor, model, GroupConfig.getDefaultInstance()));
        }

        return viewInfoList;
    }

    private boolean isHnsSearchPromoBanner() {
        Tab activeTab = mActivityTabSupplier.get();
        if (ChromeFeatureList.isEnabled(HnsFeatureList.HNS_SEARCH_OMNIBOX_BANNER)
                && mUrlBarEditingTextProvider != null
                && mUrlBarEditingTextProvider.getTextWithoutAutocomplete().length() > 0
                && activeTab != null && !activeTab.isIncognito()
                && mHnsSearchEngineDefaultRegions.contains(Locale.getDefault().getCountry())
                && !HnsSearchEngineAdapter
                            .getDSEShortName(
                                    Profile.fromWebContents(activeTab.getWebContents()), false)
                            .equals("Hns")
                && !OmniboxPrefManager.getInstance().isHnsSearchPromoBannerDismissed()
                && !OmniboxPrefManager.getInstance()
                            .isHnsSearchPromoBannerDismissedCurrentSession()) {
            long expiredDate =
                    OmniboxPrefManager.getInstance().getHnsSearchPromoBannerExpiredDate();

            if (expiredDate == 0) {
                OmniboxPrefManager.getInstance().setHnsSearchPromoBannerExpiredDate();
                return true;
            } else if (expiredDate > System.currentTimeMillis()) {
                return true;
            }
            return false;
        } else {
            return false;
        }
    }
}
