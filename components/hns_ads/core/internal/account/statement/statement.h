/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_H_

#include "base/functional/callback_forward.h"
#include "hns/components/hns_ads/core/mojom/hns_ads.mojom-forward.h"

namespace hns_ads {

using BuildStatementCallback =
    base::OnceCallback<void(mojom::StatementInfoPtr statement)>;

void BuildStatement(BuildStatementCallback callback);

}  // namespace hns_ads

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_ACCOUNT_STATEMENT_STATEMENT_H_
