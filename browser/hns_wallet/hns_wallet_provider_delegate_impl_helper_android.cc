/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/android/callback_android.h"
#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "base/notreached.h"
#include "hns/browser/hns_wallet/hns_wallet_provider_delegate_impl_helper.h"
#include "hns/browser/hns_wallet/hns_wallet_tab_helper.h"
#include "hns/build/android/jni_headers/HnsWalletProviderDelegateImplHelper_jni.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "content/public/browser/render_frame_host.h"
#include "content/public/browser/web_contents.h"

using base::android::JavaParamRef;

namespace hns_wallet {

void ShowPanel(content::WebContents*) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsWalletProviderDelegateImplHelper_showPanel(env);
}

void ShowWalletOnboarding(content::WebContents*) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsWalletProviderDelegateImplHelper_showWalletOnboarding(env);
}

void ShowAccountCreation(content::WebContents*,
                         hns_wallet::mojom::CoinType coin_type) {
  JNIEnv* env = base::android::AttachCurrentThread();
  Java_HnsWalletProviderDelegateImplHelper_ShowAccountCreation(
      env, static_cast<int>(coin_type));
}

void WalletInteractionDetected(content::WebContents* web_contents) {
  if (!web_contents) {
    return;
  }
  Java_HnsWalletProviderDelegateImplHelper_walletInteractionDetected(
      base::android::AttachCurrentThread(), web_contents->GetJavaWebContents());
}

bool IsWeb3NotificationAllowed() {
  JNIEnv* env = base::android::AttachCurrentThread();

  return Java_HnsWalletProviderDelegateImplHelper_isWeb3NotificationAllowed(
      env);
}

static void JNI_HnsWalletProviderDelegateImplHelper_IsSolanaConnected(
    JNIEnv* env,
    const JavaParamRef<jobject>& jweb_contents,
    const base::android::JavaParamRef<jstring>& jaccount,
    const JavaParamRef<jobject>& jcallback) {
  content::RenderFrameHost* rfh = nullptr;
  content::WebContents* web_contents =
      content::WebContents::FromJavaWebContents(jweb_contents);
  base::android::ScopedJavaGlobalRef callback =
      base::android::ScopedJavaGlobalRef<jobject>(jcallback);
  const std::string account = ConvertJavaStringToUTF8(env, jaccount);
  if (!(rfh = web_contents->GetPrimaryMainFrame())) {
    base::android::RunBooleanCallbackAndroid(callback, false);
    return;
  }

  auto* tab_helper =
      hns_wallet::HnsWalletTabHelper::FromWebContents(web_contents);
  if (!tab_helper) {
    base::android::RunBooleanCallbackAndroid(callback, false);
    return;
  }

  base::android::RunBooleanCallbackAndroid(
      callback,
      tab_helper->IsSolanaAccountConnected(rfh->GetGlobalId(), account));
}

}  // namespace hns_wallet
