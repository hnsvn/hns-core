/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_SPEEDREADER_SPEEDREADER_PREF_NAMES_H_
#define HNS_COMPONENTS_SPEEDREADER_SPEEDREADER_PREF_NAMES_H_

namespace speedreader {

// Is Speedreader currently enabled
// java_cpp_string.py doesn't work when the variable is constexpr
const char kSpeedreaderPrefEnabled[] = "hns.speedreader.enabled";

// Set if Speedreader was enabled at least once
constexpr char kSpeedreaderPrefEverEnabled[] = "hns.speedreader.ever_enabled";

// Number of times the user has toggled Speedreader
constexpr char kSpeedreaderPrefToggleCount[] = "hns.speedreader.toggle_count";

// Number of times the "Enable Speedreader" button was shown automatically
constexpr char kSpeedreaderPrefPromptCount[] = "hns.speedreader.prompt_count";

// The theme selected by the user. If it has a default value then system theme
// should be used.
constexpr char kSpeedreaderPrefTheme[] = "hns.speedreader.theme";

constexpr char kSpeedreaderPrefFontSize[] = "hns.speedreader.font_size";

constexpr char kSpeedreaderPrefFontFamily[] = "hns.speedreader.font_family";

constexpr char kSpeedreaderPrefTtsVoice[] = "hns.speedreader.tts_voice";

constexpr char kSpeedreaderPrefTtsSpeed[] = "hns.speedreader.tts_speed";

}  // namespace speedreader

#endif  // HNS_COMPONENTS_SPEEDREADER_SPEEDREADER_PREF_NAMES_H_
