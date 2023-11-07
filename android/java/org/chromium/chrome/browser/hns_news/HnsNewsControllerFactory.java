/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.hns_news;

import org.chromium.base.annotations.JNINamespace;
import org.chromium.base.annotations.NativeMethods;
import org.chromium.hns_news.mojom.HnsNewsController;
import org.chromium.mojo.bindings.ConnectionErrorHandler;
import org.chromium.mojo.bindings.Interface;
import org.chromium.mojo.bindings.Interface.Proxy.Handler;
import org.chromium.mojo.system.MessagePipeHandle;
import org.chromium.mojo.system.impl.CoreImpl;

@JNINamespace("chrome::android")
public class HnsNewsControllerFactory {
    private static final Object sLock = new Object();
    private static HnsNewsControllerFactory sInstance;

    public static HnsNewsControllerFactory getInstance() {
        synchronized (sLock) {
            if (sInstance == null) {
                sInstance = new HnsNewsControllerFactory();
            }
        }
        return sInstance;
    }

    private HnsNewsControllerFactory() {}

    public HnsNewsController getHnsNewsController(
            ConnectionErrorHandler connectionErrorHandler) {
        long nativeHandle = HnsNewsControllerFactoryJni.get().getInterfaceToHnsNewsController();
        MessagePipeHandle handle = wrapNativeHandle(nativeHandle);
        HnsNewsController hnsNewsController =
                HnsNewsController.MANAGER.attachProxy(handle, 0);
        Handler handler = ((Interface.Proxy) hnsNewsController).getProxyHandler();
        handler.setErrorHandler(connectionErrorHandler);

        return hnsNewsController;
    }

    private MessagePipeHandle wrapNativeHandle(long nativeHandle) {
        return CoreImpl.getInstance().acquireNativeHandle(nativeHandle).toMessagePipeHandle();
    }

    @NativeMethods
    interface Natives {
        long getInterfaceToHnsNewsController();
    }
}
