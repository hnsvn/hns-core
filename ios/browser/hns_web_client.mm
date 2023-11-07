/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#import "hns/ios/browser/hns_web_client.h"

#include <string>

#include "base/functional/bind.h"
#include "hns/ios/browser/hns_web_main_parts.h"
#include "ios/chrome/browser/shared/model/url/chrome_url_constants.h"
#include "ios/components/webui/web_ui_url_constants.h"
#include "ui/base/resource/resource_bundle.h"
#include "url/gurl.h"

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

HnsWebClient::HnsWebClient() : web_main_parts_(nullptr) {}

HnsWebClient::~HnsWebClient() {
}

void HnsWebClient::AddAdditionalSchemes(Schemes* schemes) const {
  schemes->standard_schemes.push_back(kChromeUIScheme);
  schemes->secure_schemes.push_back(kChromeUIScheme);
}

bool HnsWebClient::IsAppSpecificURL(const GURL& url) const {
  return url.SchemeIs(kChromeUIScheme);
}

std::unique_ptr<web::WebMainParts> HnsWebClient::CreateWebMainParts() {
  auto web_main_parts = std::make_unique<HnsWebMainParts>();
  web_main_parts_ = web_main_parts.get();
  return web_main_parts;
}

void HnsWebClient::SetUserAgent(const std::string& user_agent) {
  user_agent_ = user_agent;
}

std::string HnsWebClient::GetUserAgent(web::UserAgentType type) const {
  return user_agent_;
}

base::StringPiece HnsWebClient::GetDataResource(
    int resource_id,
    ui::ResourceScaleFactor scale_factor) const {
  return ui::ResourceBundle::GetSharedInstance().GetRawDataResourceForScale(
      resource_id, scale_factor);
}

base::RefCountedMemory* HnsWebClient::GetDataResourceBytes(
    int resource_id) const {
  return ui::ResourceBundle::GetSharedInstance().LoadDataResourceBytes(
      resource_id);
}

void HnsWebClient::GetAdditionalWebUISchemes(
    std::vector<std::string>* additional_schemes) {}
