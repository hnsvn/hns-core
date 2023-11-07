/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_SERVICE_CALLBACK_H_
#define HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_SERVICE_CALLBACK_H_

#include <string>

#include "base/functional/callback.h"
#include "base/values.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace hns_ads {

using GetDiagnosticsCallback =
    base::OnceCallback<void(absl::optional<base::Value::List> diagnostics)>;

using GetStatementOfAccountsCallback =
    base::OnceCallback<void(mojom::StatementInfoPtr statement)>;

using MaybeServeInlineContentAdAsDictCallback =
    base::OnceCallback<void(const std::string& dimensions,
                            absl::optional<base::Value::Dict> ads)>;

using PurgeOrphanedAdEventsForTypeCallback =
    base::OnceCallback<void(bool success)>;

using GetHistoryCallback = base::OnceCallback<void(base::Value::List history)>;

using ToggleLikeAdCallback =
    base::OnceCallback<void(base::Value::Dict ad_content)>;
using ToggleDislikeAdCallback =
    base::OnceCallback<void(base::Value::Dict ad_content)>;
using ToggleLikeCategoryCallback =
    base::OnceCallback<void(base::Value::Dict category_content)>;
using ToggleDislikeCategoryCallback =
    base::OnceCallback<void(base::Value::Dict category_content)>;
using ToggleSaveAdCallback =
    base::OnceCallback<void(base::Value::Dict ad_content)>;
using ToggleMarkAdAsInappropriateCallback =
    base::OnceCallback<void(base::Value::Dict ad_content)>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_BROWSER_ADS_SERVICE_CALLBACK_H_
