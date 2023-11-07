/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/version_info/version_info.h"
#include "components/version_info/version_info.h"

#define GetVersionNumber GetHnsVersionNumberForDisplay
#include "src/chrome/browser/ui/webui/crashes_ui.cc"
#undef GetVersionNumber
