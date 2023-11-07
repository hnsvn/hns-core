/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.component_updater;

import org.json.JSONException;
import org.json.JSONObject;

import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

/**
 * Class for handling progress of component update in Java.
 * Basically this is just bridge to native component updater classes at
 * hns_component_updater_android.cc.
 */
@JNINamespace("chrome::android")
public class HnsComponentUpdater {
    private static final String TAG = "BCU";
    private long mNativeHnsComponentUpdaterAndroid;
    private static HnsComponentUpdater sHnsComponentUpdater;

    public static HnsComponentUpdater get() {
        ThreadUtils.assertOnUiThread();
        if (sHnsComponentUpdater == null) {
            sHnsComponentUpdater = new HnsComponentUpdater();
        }
        return sHnsComponentUpdater;
    }

    private HnsComponentUpdater() {
        init();
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeHnsComponentUpdaterAndroid == 0;
        mNativeHnsComponentUpdaterAndroid = nativePtr;
    }

    private void init() {
        if (mNativeHnsComponentUpdaterAndroid == 0) {
            HnsComponentUpdaterJni.get().init(HnsComponentUpdater.this);
        }
    }

    @Override
    protected void finalize() {
        destroy();
    }

    private void destroy() {
        if (mNativeHnsComponentUpdaterAndroid != 0) {
            HnsComponentUpdaterJni.get().destroy(mNativeHnsComponentUpdaterAndroid);
            mNativeHnsComponentUpdaterAndroid = 0;
        }
    }

    /**
     * Listener for the component update changes.
     */
    public interface ComponentUpdaterListener {
        // Invoked when component update info has changed.
        void onComponentUpdateEvent(int event, String id);
    }

    private final List<ComponentUpdaterListener> mComponentUpdaterListeners =
            new CopyOnWriteArrayList<>();

    public void addComponentUpdateEventListener(ComponentUpdaterListener listener) {
        ThreadUtils.assertOnUiThread();
        mComponentUpdaterListeners.add(listener);
    }

    public void removeComponentUpdateEventListener(ComponentUpdaterListener listener) {
        ThreadUtils.assertOnUiThread();
        mComponentUpdaterListeners.remove(listener);
    }

    @CalledByNative
    protected void componentStateUpdated(int event, String id) {
        for (ComponentUpdaterListener listener : mComponentUpdaterListeners) {
            listener.onComponentUpdateEvent(event, id);
        }
    }

    /**
     * Class describing the progress of component download.
     */
    public static class CrxUpdateItem {
        // From components/update_client/update_client.h
        public static final int STATUS_NEW = 0;
        public static final int STATUS_CHECKING = 1;
        public static final int STATUS_CAN_UPDATE = 2;
        public static final int STATUS_DOWNLOADING_DIFF = 3;
        public static final int STATUS_DOWNLOADING = 4;
        public static final int STATUS_DOWNLOADED = 5;
        public static final int STATUS_UPDATING_DIFF = 6;
        public static final int STATUS_UPDATING = 7;
        public static final int STATUS_UPDATED = 8;
        public static final int STATUS_UP_TO_DATE = 9;
        public static final int STATUS_UPDATE_ERROR = 10;
        public static final int STATUS_UNINSTALLED = 11;
        public static final int STATUS_REGISTRATION = 12;
        public static final int STATUS_RUN = 13;
        public static final int STATUS_LAST_STATUS = 14;

        public String mId;
        public long mDownloadedBytes = -1;
        public long mTotalBytes = -1;
        public int mState = -1;

        public boolean isInProgress() {
            return mState == STATUS_DOWNLOADING_DIFF || mState == STATUS_DOWNLOADING
                    || mState == STATUS_DOWNLOADED || mState == STATUS_UPDATING_DIFF
                    || mState == STATUS_UPDATING;
        }

        public void normalizeZeroProgress() {
            if (mState == STATUS_DOWNLOADING && mTotalBytes == -1 && mDownloadedBytes == -1) {
                mTotalBytes = 100;
                mDownloadedBytes = 0;
            }
        }
    }

    public CrxUpdateItem getUpdateState(String id) {
        String json = HnsComponentUpdaterJni.get().getUpdateState(
                mNativeHnsComponentUpdaterAndroid, id);

        CrxUpdateItem crxUpdateItem = new CrxUpdateItem();
        try {
            JSONObject result = new JSONObject(json);
            crxUpdateItem.mId = result.getString("id");
            crxUpdateItem.mDownloadedBytes = (long) result.getDouble("downloaded_bytes");
            crxUpdateItem.mTotalBytes = (long) result.getDouble("total_bytes");
            crxUpdateItem.mState = result.getInt("state");
        } catch (JSONException e) {
            Log.e(TAG, "getUpdateState JSONException error ", e);
        }

        return crxUpdateItem;
    }

    @NativeMethods
    interface Natives {
        void init(HnsComponentUpdater caller);
        void destroy(long nativeHnsComponentUpdaterAndroid);
        String getUpdateState(long nativeHnsComponentUpdaterAndroid, String id);
    }
}
