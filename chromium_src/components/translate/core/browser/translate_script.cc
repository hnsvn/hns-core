/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "components/translate/core/browser/translate_script.h"

#include "base/functional/callback.h"
#include "base/strings/strcat.h"
#include "base/strings/stringprintf.h"
#include "base/task/sequenced_task_runner.h"
#include "hns/components/constants/hns_services_key.h"
#include "hns/components/translate/core/common/hns_translate_constants.h"
#include "hns/components/translate/core/common/hns_translate_features.h"
#include "components/grit/hns_components_resources.h"
#include "ui/base/resource/resource_bundle.h"

namespace translate {
namespace google_apis {
std::string GetAPIKey() {
  return BUILDFLAG(HNS_SERVICES_KEY);
}
}  // namespace google_apis
}  // namespace translate

#define TranslateScript ChromiumTranslateScript
#include "src/components/translate/core/browser/translate_script.cc"
#undef TranslateScript

namespace translate {

// Redirect the translate script request to the Hns endpoints.
GURL ChromiumTranslateScript::AddHostLocaleToUrl(const GURL& url) {
  GURL result = ::translate::AddHostLocaleToUrl(url);
  const GURL google_translate_script(kScriptURL);
  if (result.host_piece() == google_translate_script.host_piece()) {
    const GURL hns_translate_script(kHnsTranslateScriptURL);
    GURL::Replacements replaces;
    replaces.SetHostStr(hns_translate_script.host_piece());
    replaces.SetPathStr(hns_translate_script.path_piece());
    return result.ReplaceComponents(replaces);
  }
  return result;
}

void TranslateScript::Request(RequestCallback callback, bool is_incognito) {
  if (!IsHnsTranslateGoAvailable()) {
    base::SequencedTaskRunner::GetCurrentDefault()->PostTask(
        FROM_HERE, base::BindOnce(std::move(callback), false));
    return;
  }
  ChromiumTranslateScript::Request(std::move(callback), is_incognito);
}

void TranslateScript::OnScriptFetchComplete(bool success,
                                            const std::string& data) {
  const std::string new_data = base::StrCat(
      {base::StringPrintf(
           "const useGoogleTranslateEndpoint = %s;",
           translate::UseGoogleTranslateEndpoint() ? "true" : "false"),
       base::StringPrintf("const hnsTranslateStaticPath = '%s';",
                          kHnsTranslateStaticPath),
       ui::ResourceBundle::GetSharedInstance().LoadDataResourceString(
           IDR_HNS_TRANSLATE_JS),
       data});
  ChromiumTranslateScript::OnScriptFetchComplete(success, new_data);
}

}  // namespace translate
