/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
#define HNS_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_

// To prevent re-defining Start() in timer.h by ours overriding.
#include "base/timer/timer.h"

#define HNS_EXTERNAL_PROCESS_IMPORTER_CLIENT_H \
    friend class HnsExternalProcessImporterClient;

#define Start virtual Start
#define Cancel virtual Cancel
#define CloseMojoHandles virtual CloseMojoHandles

#include "src/chrome/browser/importer/external_process_importer_client.h"  // IWYU pragma: export

#undef HNS_EXTERNAL_PROCESS_IMPORTER_CLIENT_H
#undef Start
#undef Cancel
#undef CloseMojoHandles

#endif  // HNS_CHROMIUM_SRC_CHROME_BROWSER_IMPORTER_EXTERNAL_PROCESS_IMPORTER_CLIENT_H_
