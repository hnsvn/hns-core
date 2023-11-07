/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_HNS_SHELL_INTEGRATION_H_
#define HNS_BROWSER_HNS_SHELL_INTEGRATION_H_

#include "base/functional/callback.h"
#include "base/functional/callback_helpers.h"
#include "chrome/browser/shell_integration.h"

class Profile;

namespace shell_integration {

void PinShortcut(
    Profile* profile = nullptr,
    base::OnceCallback<void(bool)> result_callback = base::DoNothing());
void IsShortcutPinned(
    base::OnceCallback<void(bool)> result_callback = base::DoNothing());

class HnsDefaultBrowserWorker : public DefaultBrowserWorker {
 public:
  HnsDefaultBrowserWorker();

  HnsDefaultBrowserWorker(const HnsDefaultBrowserWorker&) = delete;
  HnsDefaultBrowserWorker& operator=(const HnsDefaultBrowserWorker&) =
      delete;

 protected:
  ~HnsDefaultBrowserWorker() override;

  void SetAsDefaultImpl(base::OnceClosure on_finished_callback) override;
};

}  // namespace shell_integration

#endif  // HNS_BROWSER_HNS_SHELL_INTEGRATION_H_
