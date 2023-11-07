/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/android/jni_android.h"
#include "hns/browser/hns_news/hns_news_controller_factory.h"
#include "hns/build/android/jni_headers/HnsNewsControllerFactory_jni.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/profiles/profile_manager.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"

namespace chrome {
namespace android {
static jlong JNI_HnsNewsControllerFactory_GetInterfaceToHnsNewsController(
    JNIEnv* env) {
  auto* profile = ProfileManager::GetActiveUserProfile();
  auto pending =
      hns_news::HnsNewsControllerFactory::GetInstance()->GetRemoteService(
          profile);
  auto passPipe = pending.PassPipe();

  return static_cast<jlong>(passPipe.release().value());
}

}  // namespace android
}  // namespace chrome
