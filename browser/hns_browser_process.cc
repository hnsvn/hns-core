/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/hns_browser_process.h"

#include <cstddef>

HnsBrowserProcess* g_hns_browser_process = nullptr;

HnsBrowserProcess::HnsBrowserProcess() = default;

HnsBrowserProcess::~HnsBrowserProcess() = default;
