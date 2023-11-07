/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser;

import android.content.Context;
import android.content.SharedPreferences;

import org.chromium.base.Callback;
import org.chromium.base.ContextUtils;
import org.chromium.base.Log;
import org.chromium.base.ThreadUtils;
import org.chromium.base.annotations.CalledByNative;
import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.base.task.PostTask;
import org.chromium.base.task.TaskTraits;

@JNINamespace("chrome::android")
public class HnsSyncWorker {
    private static final String TAG = "SYNC";

    private Context mContext;
    private String mDebug = "true";

    private long mNativeHnsSyncWorker;

    private static HnsSyncWorker sHnsSyncWorker;
    private static boolean sInitialized;

    public static HnsSyncWorker get() {
        if (!sInitialized) {
            sHnsSyncWorker = new HnsSyncWorker();
            sInitialized = true;
        }
        return sHnsSyncWorker;
    }

    @CalledByNative
    private void setNativePtr(long nativePtr) {
        assert mNativeHnsSyncWorker == 0;
        mNativeHnsSyncWorker = nativePtr;
    }

    private void Init() {
        if (mNativeHnsSyncWorker == 0) {
            HnsSyncWorkerJni.get().init(HnsSyncWorker.this);
        }
    }

    @Override
    protected void finalize() {
        Destroy();
    }

    private void Destroy() {
        if (mNativeHnsSyncWorker != 0) {
            HnsSyncWorkerJni.get().destroy(mNativeHnsSyncWorker);
            mNativeHnsSyncWorker = 0;
        }
    }

    public HnsSyncWorker() {
        mContext = ContextUtils.getApplicationContext();
        Init();
        (new MigrationFromV1()).MigrateFromSyncV1();
    }

    private class MigrationFromV1 {
        // Deprecated
        public static final String PREF_NAME = "SyncPreferences";
        private static final String PREF_LAST_FETCH_NAME = "TimeLastFetch";
        private static final String PREF_LATEST_DEVICE_RECORD_TIMESTAMPT_NAME =
                "LatestDeviceRecordTime";
        private static final String PREF_LAST_TIME_SEND_NOT_SYNCED_NAME = "TimeLastSendNotSynced";
        public static final String PREF_DEVICE_ID = "DeviceId";
        public static final String PREF_BASE_ORDER = "BaseOrder";
        public static final String PREF_LAST_ORDER = "LastOrder";
        public static final String PREF_SEED = "Seed";
        public static final String PREF_SYNC_DEVICE_NAME = "SyncDeviceName";
        private static final String PREF_SYNC_SWITCH = "sync_switch";
        private static final String PREF_SYNC_BOOKMARKS = "hns_sync_bookmarks";
        public static final String PREF_SYNC_TABS = "hns_sync_tabs"; // never used
        public static final String PREF_SYNC_HISTORY = "hns_sync_history"; // never used
        public static final String PREF_SYNC_AUTOFILL_PASSWORDS =
                "hns_sync_autofill_passwords"; // never used
        public static final String PREF_SYNC_PAYMENT_SETTINGS =
                "hns_sync_payment_settings"; // never used

        private boolean HaveSyncV1Prefs() {
            SharedPreferences sharedPref = mContext.getSharedPreferences(PREF_NAME, 0);

            String deviceId = sharedPref.getString(PREF_DEVICE_ID, null);
            if (null == deviceId) {
                return false;
            }
            return true;
        }

        private void DeleteSyncV1Prefs() {
            SharedPreferences sharedPref = mContext.getSharedPreferences(PREF_NAME, 0);
            SharedPreferences.Editor editor = sharedPref.edit();
            editor.clear().apply();
        }

        private void DeleteSyncV1LevelDb() {
            HnsSyncWorkerJni.get().destroyV1LevelDb();
        }

        public void MigrateFromSyncV1() {
            // Do all migration work in file IO thread because we may need to
            // read shared preferences and delete level db
            PostTask.postTask(TaskTraits.BEST_EFFORT_MAY_BLOCK, () -> {
                if (HaveSyncV1Prefs()) {
                    Log.i(TAG, "Found sync v1 data, doing migration");
                    DeleteSyncV1Prefs();
                    DeleteSyncV1LevelDb();
                    // Mark sync v1 was enabled to trigger informers
                    ThreadUtils.runOnUiThreadBlocking(new Runnable() {
                        @Override
                        public void run() {
                            HnsSyncWorkerJni.get().markSyncV1WasEnabledAndMigrated();
                            HnsSyncInformers.show();
                        }
                    });
                }
            });
        }
    };

    public String GetPureWords() {
        return HnsSyncWorkerJni.get().getSyncCodeWords(mNativeHnsSyncWorker);
    }

    public String GetTimeLimitedWordsFromPure(String pureWords) {
        return HnsSyncWorkerJni.get().getTimeLimitedWordsFromPure(pureWords);
    }

    public void SaveCodephrase(String codephrase) {
        HnsSyncWorkerJni.get().saveCodeWords(mNativeHnsSyncWorker, codephrase);
    }

    public String GetSeedHexFromWords(String codephrase) {
        return HnsSyncWorkerJni.get().getSeedHexFromWords(codephrase);
    }

    public String GetWordsFromSeedHex(String seedHex) {
        return HnsSyncWorkerJni.get().getWordsFromSeedHex(seedHex);
    }

    public String GetQrDataJson(String seedHex) {
        return HnsSyncWorkerJni.get().getQrDataJson(seedHex);
    }

    public int GetQrCodeValidationResult(String jsonQr) {
        return HnsSyncWorkerJni.get().getQrCodeValidationResult(jsonQr);
    }

    public String GetSeedHexFromQrJson(String jsonQr) {
        return HnsSyncWorkerJni.get().getSeedHexFromQrJson(jsonQr);
    }

    public int GetWordsValidationResult(String timeLimitedWords) {
        return HnsSyncWorkerJni.get().getWordsValidationResult(timeLimitedWords);
    }

    public String GetPureWordsFromTimeLimited(String timeLimitedWords) {
        return HnsSyncWorkerJni.get().getPureWordsFromTimeLimited(timeLimitedWords);
    }

    public void RequestSync() {
        HnsSyncWorkerJni.get().requestSync(mNativeHnsSyncWorker);
    }

    public boolean IsInitialSyncFeatureSetupComplete() {
        return HnsSyncWorkerJni.get().isInitialSyncFeatureSetupComplete(mNativeHnsSyncWorker);
    }

    public void FinalizeSyncSetup() {
        HnsSyncWorkerJni.get().finalizeSyncSetup(mNativeHnsSyncWorker);
    }

    public void ResetSync() {
        HnsSyncWorkerJni.get().resetSync(mNativeHnsSyncWorker);
    }

    public boolean getSyncV1WasEnabled() {
        return HnsSyncWorkerJni.get().getSyncV1WasEnabled(mNativeHnsSyncWorker);
    }

    public boolean getSyncV2MigrateNoticeDismissed() {
        return HnsSyncWorkerJni.get().getSyncV2MigrateNoticeDismissed(mNativeHnsSyncWorker);
    }

    public void setSyncV2MigrateNoticeDismissed(boolean isDismissed) {
        HnsSyncWorkerJni.get().setSyncV2MigrateNoticeDismissed(
                mNativeHnsSyncWorker, isDismissed);
    }

    @CalledByNative
    private static void onPermanentlyDeleteAccountResult(Callback<String> callback, String result) {
        callback.onResult(result);
    }

    public void permanentlyDeleteAccount(Callback<String> callback) {
        HnsSyncWorkerJni.get().permanentlyDeleteAccount(mNativeHnsSyncWorker, callback);
    }

    public void clearAccountDeletedNoticePending() {
        HnsSyncWorkerJni.get().clearAccountDeletedNoticePending(mNativeHnsSyncWorker);
    }

    public boolean isAccountDeletedNoticePending() {
        return HnsSyncWorkerJni.get().isAccountDeletedNoticePending(mNativeHnsSyncWorker);
    }

    @CalledByNative
    private static void onJoinSyncChainResult(Callback<Boolean> callback, Boolean result) {
        callback.onResult(result);
    }

    public void setJoinSyncChainCallback(Callback<Boolean> callback) {
        HnsSyncWorkerJni.get().setJoinSyncChainCallback(mNativeHnsSyncWorker, callback);
    }

    @NativeMethods
    interface Natives {
        void init(HnsSyncWorker caller);
        void destroy(long nativeHnsSyncWorker);

        void destroyV1LevelDb();
        void markSyncV1WasEnabledAndMigrated();

        String getSyncCodeWords(long nativeHnsSyncWorker);
        void requestSync(long nativeHnsSyncWorker);

        String getSeedHexFromWords(String passphrase);
        String getWordsFromSeedHex(String seedHex);
        String getQrDataJson(String seedHex);
        int getQrCodeValidationResult(String jsonQr);
        String getSeedHexFromQrJson(String jsonQr);
        int getWordsValidationResult(String timeLimitedWords);
        String getPureWordsFromTimeLimited(String timeLimitedWords);
        String getTimeLimitedWordsFromPure(String pureWords);

        void saveCodeWords(long nativeHnsSyncWorker, String passphrase);

        void finalizeSyncSetup(long nativeHnsSyncWorker);

        boolean isInitialSyncFeatureSetupComplete(long nativeHnsSyncWorker);

        void resetSync(long nativeHnsSyncWorker);

        boolean getSyncV1WasEnabled(long nativeHnsSyncWorker);
        boolean getSyncV2MigrateNoticeDismissed(long nativeHnsSyncWorker);
        void setSyncV2MigrateNoticeDismissed(long nativeHnsSyncWorker, boolean isDismissed);
        void permanentlyDeleteAccount(long nativeHnsSyncWorker, Callback<String> callback);
        void clearAccountDeletedNoticePending(long nativeHnsSyncWorker);
        boolean isAccountDeletedNoticePending(long nativeHnsSyncWorker);
        void setJoinSyncChainCallback(long nativeHnsSyncWorker, Callback<Boolean> callback);
    }
}
