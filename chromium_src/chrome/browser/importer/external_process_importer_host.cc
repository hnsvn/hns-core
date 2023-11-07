/* Copyright 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/importer/hns_external_process_importer_client.h"
#include "hns/browser/importer/hns_in_process_importer_bridge.h"

#define ExternalProcessImporterClient HnsExternalProcessImporterClient
#define InProcessImporterBridge HnsInProcessImporterBridge
#include "src/chrome/browser/importer/external_process_importer_host.cc"
#undef ExternalProcessImporterClient
#undef InProcessImporterBridge
