/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.autofill;

import android.app.Activity;
import android.view.View;
import android.view.ViewTreeObserver;

import androidx.annotation.NonNull;

import org.chromium.base.annotations.JNINamespace;
import org.chromium.chrome.browser.app.HnsActivity;
import org.chromium.ui.base.WindowAndroid;

@JNINamespace("autofill")
public class HnsAutofillPopupBridge
        extends AutofillPopupBridge implements ViewTreeObserver.OnPreDrawListener {
    private View mView;
    private HnsActivity mActivity;
    private ViewTreeObserver mViewTreeObserver;

    public HnsAutofillPopupBridge(@NonNull View anchorView, long nativeAutofillPopupViewAndroid,
            @NonNull WindowAndroid windowAndroid) {
        super(anchorView, nativeAutofillPopupViewAndroid, windowAndroid);

        Activity activity = windowAndroid != null ? windowAndroid.getActivity().get() : null;
        assert activity instanceof HnsActivity : "Wrong activity type!";
        if (activity instanceof HnsActivity) {
            mActivity = (HnsActivity) activity;
        }

        mView = anchorView;
        mViewTreeObserver = mView.getViewTreeObserver();
        mViewTreeObserver.addOnPreDrawListener(this);
    }

    // ViewTreeObserver.OnPreDrawListener implementation.
    @Override
    public boolean onPreDraw() {
        if (mView.isShown() && mActivity != null && !mActivity.isViewBelowToolbar(mView)) {
            mView.setY(mActivity.getToolbarBottom());
        }

        // We need to make an adjustment only once.
        if (mViewTreeObserver != null && mViewTreeObserver.isAlive()) {
            mViewTreeObserver.removeOnPreDrawListener(this);
        }

        return true;
    }
}
