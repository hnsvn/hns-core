/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_AI_CHAT_COMMON_PREF_NAMES_H_
#define HNS_COMPONENTS_AI_CHAT_COMMON_PREF_NAMES_H_

class PrefRegistrySimple;

namespace ai_chat::prefs {

constexpr char kHnsChatHasSeenDisclaimer[] =
    "hns.ai_chat.has_seen_disclaimer";
constexpr char kHnsChatAutoGenerateQuestions[] =
    "hns.ai_chat.auto_generate_questions";
constexpr char kHnsChatP3AChatCountWeeklyStorage[] =
    "hns.ai_chat.p3a_chat_count";
constexpr char kHnsChatP3APromptCountWeeklyStorage[] =
    "hns.ai_chat.p3a_prompt_count";

void RegisterProfilePrefs(PrefRegistrySimple* registry);

}  // namespace ai_chat::prefs

#endif  // HNS_COMPONENTS_AI_CHAT_COMMON_PREF_NAMES_H_
