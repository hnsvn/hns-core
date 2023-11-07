/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet;

import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.hns_wallet.mojom.HnsWalletP3a;
import org.chromium.hns_wallet.mojom.HnsWalletService;
import org.chromium.chrome.browser.crypto_wallet.util.Utils;
import org.chromium.chrome.browser.profiles.Profile;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.bindings.Interface;
import org.chromium.mojo.bindings.Interface.Proxy.Handler;
import org.chromium.mojo.system.MessagePipeHandle;
import org.chromium.mojo.system.impl.CoreImpl;

@JNINamespace("chrome::android")
public class HnsWalletServiceFactory {
    private static final Object sLock = new Object();
    private static HnsWalletServiceFactory sInstance;

    public static HnsWalletServiceFactory getInstance() {
        synchronized (sLock) {
            if (sInstance == null) {
                sInstance = new HnsWalletServiceFactory();
            }
        }
        return sInstance;
    }

    private HnsWalletServiceFactory() {}

    public HnsWalletService getHnsWalletService(ConnectionErrorHandler connectionErrorHandler) {
        Profile profile = Utils.getProfile(false); // always use regular profile
        long nativeHandle =
                HnsWalletServiceFactoryJni.get().getInterfaceToHnsWalletService(profile);
        MessagePipeHandle handle = wrapNativeHandle(nativeHandle);
        HnsWalletService hnsWalletService = HnsWalletService.MANAGER.attachProxy(handle, 0);
        Handler handler = ((Interface.Proxy) hnsWalletService).getProxyHandler();
        handler.setErrorHandler(connectionErrorHandler);

        return hnsWalletService;
    }

    public HnsWalletP3a getHnsWalletP3A(ConnectionErrorHandler connectionErrorHandler) {
        Profile profile = Utils.getProfile(false); // always use regular profile
        long nativeHandle =
                HnsWalletServiceFactoryJni.get().getInterfaceToHnsWalletP3A(profile);
        MessagePipeHandle handle = wrapNativeHandle(nativeHandle);
        HnsWalletP3a hnsWalletP3A = HnsWalletP3a.MANAGER.attachProxy(handle, 0);
        Handler handler = ((Interface.Proxy) hnsWalletP3A).getProxyHandler();
        handler.setErrorHandler(connectionErrorHandler);

        return hnsWalletP3A;
    }

    private MessagePipeHandle wrapNativeHandle(long nativeHandle) {
        return CoreImpl.getInstance().acquireNativeHandle(nativeHandle).toMessagePipeHandle();
    }

    @NativeMethods
    interface Natives {
        long getInterfaceToHnsWalletService(Profile profile);
        long getInterfaceToHnsWalletP3A(Profile profile);
    }
}
