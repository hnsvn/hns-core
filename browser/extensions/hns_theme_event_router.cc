/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/hns_theme_event_router.h"

#include <memory>
#include <string>
#include <utility>

#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/common/extensions/api/hns_theme.h"
#include "chrome/browser/profiles/profile.h"
#include "extensions/browser/event_router.h"
#include "ui/native_theme/native_theme.h"

namespace extensions {

HnsThemeEventRouter::HnsThemeEventRouter(Profile* profile)
    : profile_(profile) {
  observer_.Observe(ui::NativeTheme::GetInstanceForNativeUi());
}

HnsThemeEventRouter::~HnsThemeEventRouter() = default;

void HnsThemeEventRouter::OnNativeThemeUpdated(
    ui::NativeTheme* observed_theme) {
  DCHECK(observer_.IsObservingSource(observed_theme));
  Notify();
}

void HnsThemeEventRouter::Notify() {
  const std::string theme_type =
      dark_mode::GetStringFromHnsDarkModeType(
          dark_mode::GetActiveHnsDarkModeType());

  auto event = std::make_unique<extensions::Event>(
      extensions::events::HNS_ON_HNS_THEME_TYPE_CHANGED,
      api::hns_theme::OnHnsThemeTypeChanged::kEventName,
      api::hns_theme::OnHnsThemeTypeChanged::Create(theme_type),
      profile_);

  if (EventRouter* event_router = EventRouter::Get(profile_))
    event_router->BroadcastEvent(std::move(event));
}

}  // namespace extensions
