/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_UI_WEBUI_HNS_WEB_UI_CONTROLLER_FACTORY_H_
#define HNS_BROWSER_UI_WEBUI_HNS_WEB_UI_CONTROLLER_FACTORY_H_

#include <memory>

#include "chrome/browser/ui/webui/chrome_web_ui_controller_factory.h"

namespace base {
template <typename T>
class NoDestructor;
class RefCountedMemory;
}  // namespace base

class HnsWebUIControllerFactory : public ChromeWebUIControllerFactory {
 public:
  HnsWebUIControllerFactory(const HnsWebUIControllerFactory&) = delete;
  HnsWebUIControllerFactory& operator=(const HnsWebUIControllerFactory&) =
      delete;

  content::WebUI::TypeID GetWebUIType(content::BrowserContext* browser_context,
                                      const GURL& url) override;
  std::unique_ptr<content::WebUIController> CreateWebUIControllerForURL(
      content::WebUI* web_ui,
      const GURL& url) override;

  static HnsWebUIControllerFactory* GetInstance();

 protected:
  friend base::NoDestructor<HnsWebUIControllerFactory>;

  HnsWebUIControllerFactory();
  ~HnsWebUIControllerFactory() override;
};

#endif  // HNS_BROWSER_UI_WEBUI_HNS_WEB_UI_CONTROLLER_FACTORY_H_
