/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/browser/ui/webui/settings/hns_import_data_handler.h"
#include "hns/browser/ui/webui/settings/hns_search_engines_handler.h"
#include "hns/browser/ui/webui/settings/hns_site_settings_handler.h"
#include "hns/browser/ui/webui/settings/settings_cookies_view_handler.h"
#include "hns/components/hns_vpn/common/buildflags/buildflags.h"
#include "chrome/browser/ui/webui/settings/hats_handler.h"
#include "chrome/browser/ui/webui/settings/settings_secure_dns_handler.h"
#include "chrome/browser/ui/webui/settings/site_settings_handler.h"

#if BUILDFLAG(IS_WIN) && BUILDFLAG(ENABLE_HNS_VPN)
#include "hns/browser/ui/webui/settings/hns_settings_secure_dns_handler.h"

#define SecureDnsHandler HnsSecureDnsHandler
#endif
#define SiteSettingsHandler HnsSiteSettingsHandler
#define ImportDataHandler HnsImportDataHandler
#define SearchEnginesHandler HnsSearchEnginesHandler
#define HatsHandler HatsHandler>());AddSettingsPageUIHandler(std::make_unique<CookiesViewHandler
#include "src/chrome/browser/ui/webui/settings/settings_ui.cc"
#undef HatsHandler
#undef ImportDataHandler
#undef SearchEnginesHandler
#undef SiteSettingsHandler
