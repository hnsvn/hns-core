// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef HNS_COMPONENTS_SPEEDREADER_COMMON_CONSTANTS_H_
#define HNS_COMPONENTS_SPEEDREADER_COMMON_CONSTANTS_H_

#include "components/grit/hns_components_strings.h"
#include "ui/base/webui/web_ui_util.h"

namespace speedreader {

constexpr webui::LocalizedString kLocalizedStrings[] = {
    {"hnsReaderModeCaption", IDS_READER_MODE_CAPTION},
    {"hnsReaderModeClose", IDS_READER_MODE_CLOSE},
    {"hnsReaderModeExit", IDS_READER_MODE_EXIT},
    {"hnsReaderModeVoice", IDS_READER_MODE_VOICE},
    {"hnsReaderModeTune", IDS_READER_MODE_TUNE},
    {"hnsReaderModeAppearance", IDS_READER_MODE_APPEARANCE},
    {"hnsReaderModeAppearanceThemeLight",
     IDS_READER_MODE_APPEARANCE_THEME_LIGHT},
    {"hnsReaderModeAppearanceThemeSepia",
     IDS_READER_MODE_APPEARANCE_THEME_SEPIA},
    {"hnsReaderModeAppearanceThemeDark",
     IDS_READER_MODE_APPEARANCE_THEME_DARK},
    {"hnsReaderModeAppearanceThemeSystem",
     IDS_READER_MODE_APPEARANCE_THEME_SYSTEM},
    {"hnsReaderModeAppearanceFontSans", IDS_READER_MODE_APPEARANCE_FONT_SANS},
    {"hnsReaderModeAppearanceFontSerif",
     IDS_READER_MODE_APPEARANCE_FONT_SERIF},
    {"hnsReaderModeAppearanceFontMono", IDS_READER_MODE_APPEARANCE_FONT_MONO},
    {"hnsReaderModeAppearanceFontDyslexic",
     IDS_READER_MODE_APPEARANCE_FONT_DYSLEXIC},
    {"hnsReaderModeTextToSpeech", IDS_READER_MODE_TEXT_TO_SPEECH},
    {"hnsReaderModeAI", IDS_READER_MODE_AI},
    {"hnsReaderModeFontSizeDecrease",
     IDS_READER_MODE_APPEARANCE_FONT_SIZE_DECREASE},
    {"hnsReaderModeFontSizeIncrease",
     IDS_READER_MODE_APPEARANCE_FONT_SIZE_INCREASE},
    {"hnsReaderModeTtsRewind", IDS_READER_MODE_TEXT_TO_SPEECH_REWIND},
    {"hnsReaderModeTtsPlayPause", IDS_READER_MODE_TEXT_TO_SPEECH_PLAY_PAUSE},
    {"hnsReaderModeTtsForward", IDS_READER_MODE_TEXT_TO_SPEECH_FORWARD},
    {"hnsReaderModeTtsSpeedDecrease",
     IDS_READER_MODE_TEXT_TO_SPEECH_SPEED_DECREASE},
    {"hnsReaderModeTtsSpeedIncrease",
     IDS_READER_MODE_TEXT_TO_SPEECH_SPEED_INCREASE},
};

}  // namespace speedreader

#endif  // HNS_COMPONENTS_SPEEDREADER_COMMON_CONSTANTS_H_
