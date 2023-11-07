/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.externalnav;

import android.content.Intent;

import org.chromium.base.ContextUtils;
import org.chromium.chrome.browser.HnsWalletProvider;
import org.chromium.chrome.browser.privacy.settings.HnsPrivacySettings;
import org.chromium.components.external_intents.ExternalNavigationDelegate;
import org.chromium.components.external_intents.ExternalNavigationHandler;
import org.chromium.components.external_intents.ExternalNavigationHandler.OverrideUrlLoadingResult;
import org.chromium.components.external_intents.ExternalNavigationParams;
import org.chromium.url.GURL;

/**
 * Extends Chromium's ExternalNavigationHandler
 */
public class HnsExternalNavigationHandler extends ExternalNavigationHandler {
    // private static final String TAG = "HnsUrlHandler";
    private HnsWalletProvider mHnsWalletProvider;

    public HnsExternalNavigationHandler(ExternalNavigationDelegate delegate) {
        super(delegate);
    }

    @Override
    public OverrideUrlLoadingResult shouldOverrideUrlLoading(ExternalNavigationParams params) {
        if (isWalletProviderOverride(params)) {
            String originalUrl = params.getUrl().getSpec();
            String url = originalUrl.replaceFirst("^rewards://", "hns://rewards/");
            GURL browserFallbackGURL = new GURL(url);
            if (params.getRedirectHandler() != null) {
                params.getRedirectHandler().setShouldNotOverrideUrlLoadingOnCurrentRedirectChain();
            }
            return OverrideUrlLoadingResult.forNavigateTab(browserFallbackGURL, params);
        }
        // TODO: Once we have a ready for https://github.com/hnsvn/hns-browser/issues/33015, We'll
        // use this code
        /*else if (originalUrl.equalsIgnoreCase("chrome://adblock/")) {
            try {
                HnsActivity.getHnsActivity().openHnsContentFilteringSettings();
            } catch (HnsActivity.HnsActivityNotFoundException e) {
                Log.e(TAG, "adblock url " + e);
            }
            return OverrideUrlLoadingResult.forExternalIntent();
        }*/
        return super.shouldOverrideUrlLoading(params);
    }

    private boolean isWalletProviderOverride(ExternalNavigationParams params) {
        if (params.getUrl().getSpec().startsWith(HnsWalletProvider.UPHOLD_REDIRECT_URL)) {
            return true;
        }

        if (params.getUrl().getSpec().startsWith(HnsWalletProvider.BITFLYER_REDIRECT_URL)) {
            return true;
        }

        if (params.getUrl().getSpec().startsWith(HnsWalletProvider.GEMINI_REDIRECT_URL)) {
            return true;
        }

        if (params.getUrl().getSpec().startsWith(HnsWalletProvider.ZEBPAY_REDIRECT_URL)) {
            return true;
        }

        return false;
    }

    @Override
    protected OverrideUrlLoadingResult startActivity(Intent intent, boolean requiresIntentChooser,
            QueryIntentActivitiesSupplier resolvingInfos, ResolveActivitySupplier resolveActivity,
            GURL browserFallbackUrl, GURL intentDataUrl, ExternalNavigationParams params) {
        if (ContextUtils.getAppSharedPreferences().getBoolean(
                    HnsPrivacySettings.PREF_APP_LINKS, true)) {
            return super.startActivity(intent, requiresIntentChooser, resolvingInfos,
                    resolveActivity, browserFallbackUrl, intentDataUrl, params);
        } else {
            return OverrideUrlLoadingResult.forNoOverride();
        }
    }
}
