/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/browser/extensions/api/hns_theme_api.h"
#include "hns/browser/themes/hns_dark_mode_utils.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/test/base/in_process_browser_test.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "extensions/browser/api_test_utils.h"
#include "extensions/common/extension_builder.h"

using extensions::api::HnsThemeGetHnsThemeTypeFunction;
using extensions::api_test_utils::RunFunctionAndReturnSingleResult;

class HnsThemeAPIBrowserTest : public InProcessBrowserTest {
 public:
  void SetUpOnMainThread() override {
    InProcessBrowserTest::SetUpOnMainThread();
    extension_ = extensions::ExtensionBuilder("Test").Build();
  }

  scoped_refptr<const extensions::Extension> extension() {
    return extension_;
  }

 private:
  scoped_refptr<const extensions::Extension> extension_;
};

IN_PROC_BROWSER_TEST_F(HnsThemeAPIBrowserTest,
                       HnsThemeGetHnsThemeTypeTest) {
  // Change to Light type and check it from api.
  dark_mode::SetHnsDarkModeType(
      dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT);
  EXPECT_EQ(dark_mode::HnsDarkModeType::HNS_DARK_MODE_TYPE_LIGHT,
            dark_mode::GetActiveHnsDarkModeType());

  scoped_refptr<HnsThemeGetHnsThemeTypeFunction> get_function(
      new HnsThemeGetHnsThemeTypeFunction());
  get_function->set_extension(extension().get());
  auto value = RunFunctionAndReturnSingleResult(
      get_function.get(), std::string("[]"), browser()->profile());
  ASSERT_TRUE(value);
  EXPECT_EQ(value->GetString(), "Light");
}
