/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import android.app.Activity;
import android.app.SearchManager;
import android.content.Intent;
import android.net.Uri;
import android.text.TextUtils;

import org.chromium.base.IntentUtils;
import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.chrome.browser.search_engines.TemplateUrlServiceFactory;
import org.chromium.chrome.browser.util.HnsConstants;
import org.chromium.content_public.browser.BrowserStartupController;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;

public class HnsIntentHandler extends IntentHandler {
    private static final String TAG = "HnsIntentHandler";

    private static final String CONNECTION_INFO_HELP_URL =
            "https://support.google.com/chrome?p=android_connection_info";
    private static final String HNS_CONNECTION_INFO_HELP_URL =
            "https://support.hns.com/hc/en-us/articles/360018185871-How-do-I-check-if-a-site-s-connection-is-secure-";

    public HnsIntentHandler(Activity activity, IntentHandlerDelegate delegate) {
        super(activity, delegate);
    }

    @Override
    public boolean onNewIntent(Intent intent) {
        // Redirect requests if necessary
        String url = getUrlFromIntent(intent);
        if (url != null && url.equals(CONNECTION_INFO_HELP_URL)) {
            intent.setData(Uri.parse(HNS_CONNECTION_INFO_HELP_URL));
        }
        String appLinkAction = intent.getAction();
        Uri appLinkData = intent.getData();

        if (Intent.ACTION_VIEW.equals(appLinkAction) && appLinkData != null) {
            String lastPathSegment = appLinkData.getLastPathSegment();
            if (lastPathSegment != null
                    && (lastPathSegment.equalsIgnoreCase(HnsConstants.DEEPLINK_ANDROID_PLAYLIST)
                            || lastPathSegment.equalsIgnoreCase(
                                    HnsConstants.DEEPLINK_ANDROID_VPN))) {
                return false;
            }
        }
        return super.onNewIntent(intent);
    }

    /**
     * Helper method to extract the raw URL from the intent, without further processing.
     * The URL may be in multiple locations.
     * @param intent Intent to examine.
     * @return Raw URL from the intent, or null if raw URL could't be found.
     */
    protected static String extractUrlFromIntent(Intent intent) {
        if (intent == null) return null;
        String url = getUrlFromVoiceSearchResult(intent);
        if (url == null) url = getUrlForCustomTab(intent);
        if (url == null) url = getUrlForWebapp(intent);
        if (url == null) url = intent.getDataString();
        if (url == null) url = getUrlFromText(intent);
        if (url == null) url = getWebSearchUrl(intent);
        if (url == null) return null;
        url = url.trim();
        return TextUtils.isEmpty(url) ? null : url;
    }

    protected static String getUrlFromText(Intent intent) {
        if (intent == null) return null;
        String text = IntentUtils.safeGetStringExtra(intent, Intent.EXTRA_TEXT);
        return (text == null || isJavascriptSchemeOrInvalidUrl(text)) ? null : text;
    }

    protected static String getWebSearchUrl(Intent intent) {
        final String action = intent.getAction();
        if (!Intent.ACTION_WEB_SEARCH.equals(action)) {
            return null;
        }

        String query = IntentUtils.safeGetStringExtra(intent, SearchManager.QUERY);
        if (TextUtils.isEmpty(query)
                || !BrowserStartupController.getInstance().isFullBrowserStarted()) {
            return null;
        }

        try {
            return ThreadUtils.runOnUiThreadBlocking(new Callable<String>() {
                @Override
                public String call() {
                    return TemplateUrlServiceFactory
                            .getForProfile(Profile.getLastUsedRegularProfile())
                            .getUrlForSearchQuery(query);
                }
            });
        } catch (ExecutionException e) {
            Log.e(TAG, "Could not retrieve search query: " + e);
        }
        return null;
    }

    private static String getUrlForCustomTab(Intent intent) {
        assert (false);
        return null;
    }

    private static String getUrlForWebapp(Intent intent) {
        assert (false);
        return null;
    }

    private static boolean isJavascriptSchemeOrInvalidUrl(String url) {
        assert (false);
        return false;
    }
}
