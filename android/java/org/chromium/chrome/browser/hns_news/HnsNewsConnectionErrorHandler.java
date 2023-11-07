/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.hns_news;

import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.system.MojoException;

/**
 * This is a handler for mojo connection failure for Hns News
 */
public class HnsNewsConnectionErrorHandler implements ConnectionErrorHandler {
    /**
     * This is a delegate that is implemented in the object where the connection is created
     */
    public interface HnsNewsConnectionErrorHandlerDelegate {
        default void initHnsNewsController() {}
        default void cleanUpHnsNewsController() {}
    }

    private HnsNewsConnectionErrorHandlerDelegate mHnsNewsConnectionErrorHandlerDelegate;
    private static final Object sLock = new Object();
    private static volatile HnsNewsConnectionErrorHandler sInstance;

    public static HnsNewsConnectionErrorHandler getInstance() {
        synchronized (sLock) {
            if (sInstance == null) {
                sInstance = new HnsNewsConnectionErrorHandler();
            }
        }
        return sInstance;
    }

    public void setDelegate(
            HnsNewsConnectionErrorHandlerDelegate hnsNewsConnectionErrorHandlerDelegate) {
        mHnsNewsConnectionErrorHandlerDelegate = hnsNewsConnectionErrorHandlerDelegate;
        assert mHnsNewsConnectionErrorHandlerDelegate
                != null : "mHnsNewsConnectionErrorHandlerDelegate has to be initialized";
    }

    @Override
    public void onConnectionError(MojoException e) {
        if (mHnsNewsConnectionErrorHandlerDelegate == null) {
            return;
        }

        mHnsNewsConnectionErrorHandlerDelegate.cleanUpHnsNewsController();
        mHnsNewsConnectionErrorHandlerDelegate.initHnsNewsController();
    }
}
