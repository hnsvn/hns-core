/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/l10n/common/un_m49_code_util.h"

#include "base/containers/contains.h"
#include "base/strings/string_piece.h"
#include "hns/components/l10n/common/un_m49_code_constants.h"

namespace hns_l10n {

bool IsUNM49Code(const base::StringPiece code) {
  return base::Contains(kUnM49Codes, code);
}

}  // namespace hns_l10n
