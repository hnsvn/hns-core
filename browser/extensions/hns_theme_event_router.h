/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_HNS_THEME_EVENT_ROUTER_H_
#define HNS_BROWSER_EXTENSIONS_HNS_THEME_EVENT_ROUTER_H_

#include "base/memory/raw_ptr.h"
#include "base/scoped_observation.h"
#include "ui/native_theme/native_theme.h"
#include "ui/native_theme/native_theme_observer.h"

class Profile;

namespace extensions {

class HnsThemeEventRouter : public ui::NativeThemeObserver {
 public:
  explicit HnsThemeEventRouter(Profile* profile);
  HnsThemeEventRouter(const HnsThemeEventRouter&) = delete;
  HnsThemeEventRouter& operator=(const HnsThemeEventRouter&) = delete;
  ~HnsThemeEventRouter() override;

 private:
  friend class MockHnsThemeEventRouter;

  // ui::NativeThemeObserver overrides:
  void OnNativeThemeUpdated(ui::NativeTheme* observed_theme) override;

  // Make virtual for testing.
  virtual void Notify();

  raw_ptr<ui::NativeTheme> current_native_theme_for_testing_ = nullptr;
  raw_ptr<Profile> profile_ = nullptr;
  base::ScopedObservation<ui::NativeTheme, ui::NativeThemeObserver> observer_{
      this};
};

}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_HNS_THEME_EVENT_ROUTER_H_
