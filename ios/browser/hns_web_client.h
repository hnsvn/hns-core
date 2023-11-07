/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_IOS_BROWSER_HNS_WEB_CLIENT_H_
#define HNS_IOS_BROWSER_HNS_WEB_CLIENT_H_

#include <memory>
#include <string>
#include <vector>

#include "base/compiler_specific.h"
#import "ios/web/public/web_client.h"

class HnsWebMainParts;

class HnsWebClient : public web::WebClient {
 public:
  HnsWebClient();
  HnsWebClient(const HnsWebClient&) = delete;
  HnsWebClient& operator=(const HnsWebClient&) = delete;
  ~HnsWebClient() override;

  void SetUserAgent(const std::string& user_agent);

  // WebClient implementation.
  void AddAdditionalSchemes(Schemes* schemes) const override;
  bool IsAppSpecificURL(const GURL& url) const override;

  std::unique_ptr<web::WebMainParts> CreateWebMainParts() override;
  std::string GetUserAgent(web::UserAgentType type) const override;
  base::StringPiece GetDataResource(
      int resource_id,
      ui::ResourceScaleFactor scale_factor) const override;
  base::RefCountedMemory* GetDataResourceBytes(int resource_id) const override;

  void GetAdditionalWebUISchemes(
      std::vector<std::string>* additional_schemes) override;

 private:
  HnsWebMainParts* web_main_parts_;
  std::string user_agent_;
};

#endif  // HNS_IOS_BROWSER_HNS_WEB_CLIENT_H_
