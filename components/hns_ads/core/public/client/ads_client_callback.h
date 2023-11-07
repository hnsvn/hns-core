/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_CLIENT_ADS_CLIENT_CALLBACK_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_CLIENT_ADS_CLIENT_CALLBACK_H_

#include <string>
#include <vector>

#include "base/files/file.h"
#include "base/functional/callback.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "url/gurl.h"

namespace hns_ads {

using ResultCallback = base::OnceCallback<void(bool success)>;

using SaveCallback = base::OnceCallback<void(bool success)>;

using LoadCallback =
    base::OnceCallback<void(const absl::optional<std::string>& value)>;

using LoadFileCallback = base::OnceCallback<void(base::File file)>;

using UrlRequestCallback =
    base::OnceCallback<void(const mojom::UrlResponseInfo& url_response)>;

using RunDBTransactionCallback =
    base::OnceCallback<void(mojom::DBCommandResponseInfoPtr command_response)>;

using GetBrowsingHistoryCallback =
    base::OnceCallback<void(const std::vector<GURL>& browsing_history)>;

using GetScheduledCaptchaCallback =
    base::OnceCallback<void(const std::string& captcha_id)>;

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_PUBLIC_CLIENT_ADS_CLIENT_CALLBACK_H_
