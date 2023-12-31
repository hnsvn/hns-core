/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/android/jni_android.h"
#include "base/android/jni_string.h"
#include "base/containers/contains.h"
#include "base/functional/bind.h"
#include "base/task/sequenced_task_runner.h"
#include "hns/build/android/jni_headers/WalletDataFilesInstaller_jni.h"
#include "hns/components/hns_wallet/browser/wallet_data_files_installer.h"
#include "build/build_config.h"
#include "chrome/browser/browser_process.h"
#include "components/component_updater/component_updater_service.h"

static_assert BUILDFLAG(IS_ANDROID);

namespace chrome {
namespace android {

namespace {

bool IsHnsWalletDataFilesComponentRegistered() {
  std::vector<std::string> registered_ids =
      g_browser_process->component_updater()->GetComponentIDs();
  return base::Contains(
      registered_ids,
      std::string(hns_wallet::GetWalletDataFilesComponentId()));
}

}  // namespace

static void
JNI_WalletDataFilesInstaller_RegisterWalletDataFilesComponentOnDemand(
    JNIEnv* env) {
  if (IsHnsWalletDataFilesComponentRegistered()) {
    return;
  }

  component_updater::ComponentUpdateService* cus =
      g_browser_process->component_updater();

  ::hns_wallet::RegisterWalletDataFilesComponentOnDemand(cus);
}

static base::android::ScopedJavaLocalRef<jstring>
JNI_WalletDataFilesInstaller_GetWalletDataFilesComponentId(JNIEnv* env) {
  return base::android::ConvertUTF8ToJavaString(
      env, ::hns_wallet::GetWalletDataFilesComponentId());
}

}  // namespace android
}  // namespace chrome
