/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.fragments.dapps;

import android.app.Activity;

import androidx.fragment.app.Fragment;

import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.hns_wallet.mojom.JsonRpcService;
import org.chromium.hns_wallet.mojom.KeyringService;
import org.chromium.chrome.browser.crypto_wallet.activities.HnsWalletBaseActivity;

public class BaseDAppsFragment extends Fragment {
    public HnsWalletService getHnsWalletService() {
        Activity activity = getActivity();
        if (activity instanceof HnsWalletBaseActivity) {
            return ((HnsWalletBaseActivity) activity).getHnsWalletService();
        }

        return null;
    }

    public KeyringService getKeyringService() {
        Activity activity = getActivity();
        if (activity instanceof HnsWalletBaseActivity) {
            return ((HnsWalletBaseActivity) activity).getKeyringService();
        }

        return null;
    }

    public JsonRpcService getJsonRpcService() {
        Activity activity = getActivity();
        if (activity instanceof HnsWalletBaseActivity) {
            return ((HnsWalletBaseActivity) activity).getJsonRpcService();
        }

        return null;
    }
}
