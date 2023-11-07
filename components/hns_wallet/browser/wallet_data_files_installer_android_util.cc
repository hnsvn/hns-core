/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_wallet/browser/wallet_data_files_installer_android_util.h"

#include <jni.h>

#include "base/android/jni_android.h"
#include "hns/build/android/jni_headers/WalletDataFilesInstallerUtil_jni.h"

namespace hns_wallet {

bool IsHnsWalletConfiguredOnAndroid() {
  JNIEnv* env = base::android::AttachCurrentThread();
  return Java_WalletDataFilesInstallerUtil_isHnsWalletConfiguredOnAndroid(
      env);
}

}  // namespace hns_wallet
