/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_REWARDS_COMMON_MOJOM_REWARDS_TYPES_MOJOM_TRAITS_H_
#define HNS_COMPONENTS_HNS_REWARDS_COMMON_MOJOM_REWARDS_TYPES_MOJOM_TRAITS_H_

#include "base/types/expected.h"
#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom-forward.h"
#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom-shared.h"
#include "mojo/public/cpp/bindings/union_traits.h"

namespace mojo {

template <>
struct UnionTraits<
    hns_rewards::mojom::ConnectExternalWalletResultDataView,
    base::expected<void, hns_rewards::mojom::ConnectExternalWalletError>> {
  static hns_rewards::mojom::ConnectExternalWalletValuePtr value(
      const base::expected<void,
                           hns_rewards::mojom::ConnectExternalWalletError>&
          result);

  static hns_rewards::mojom::ConnectExternalWalletError error(
      const base::expected<void,
                           hns_rewards::mojom::ConnectExternalWalletError>&
          result);

  static hns_rewards::mojom::ConnectExternalWalletResultDataView::Tag GetTag(
      const base::expected<void,
                           hns_rewards::mojom::ConnectExternalWalletError>&
          result);

  static bool Read(
      hns_rewards::mojom::ConnectExternalWalletResultDataView data,
      base::expected<void, hns_rewards::mojom::ConnectExternalWalletError>*
          out);
};

template <>
struct UnionTraits<hns_rewards::mojom::FetchBalanceResultDataView,
                   base::expected<hns_rewards::mojom::BalancePtr,
                                  hns_rewards::mojom::FetchBalanceError>> {
  static hns_rewards::mojom::FetchBalanceValuePtr value(
      const base::expected<hns_rewards::mojom::BalancePtr,
                           hns_rewards::mojom::FetchBalanceError>& result);

  static hns_rewards::mojom::FetchBalanceError error(
      const base::expected<hns_rewards::mojom::BalancePtr,
                           hns_rewards::mojom::FetchBalanceError>& result);

  static hns_rewards::mojom::FetchBalanceResultDataView::Tag GetTag(
      const base::expected<hns_rewards::mojom::BalancePtr,
                           hns_rewards::mojom::FetchBalanceError>& result);

  static bool Read(
      hns_rewards::mojom::FetchBalanceResultDataView data,
      base::expected<hns_rewards::mojom::BalancePtr,
                     hns_rewards::mojom::FetchBalanceError>* out);
};

template <>
struct UnionTraits<
    hns_rewards::mojom::GetExternalWalletResultDataView,
    base::expected<hns_rewards::mojom::ExternalWalletPtr,
                   hns_rewards::mojom::GetExternalWalletError>> {
  static hns_rewards::mojom::GetExternalWalletValuePtr value(
      const base::expected<hns_rewards::mojom::ExternalWalletPtr,
                           hns_rewards::mojom::GetExternalWalletError>&
          result);

  static hns_rewards::mojom::GetExternalWalletError error(
      const base::expected<hns_rewards::mojom::ExternalWalletPtr,
                           hns_rewards::mojom::GetExternalWalletError>&
          result);

  static hns_rewards::mojom::GetExternalWalletResultDataView::Tag GetTag(
      const base::expected<hns_rewards::mojom::ExternalWalletPtr,
                           hns_rewards::mojom::GetExternalWalletError>&
          result);

  static bool Read(
      hns_rewards::mojom::GetExternalWalletResultDataView data,
      base::expected<hns_rewards::mojom::ExternalWalletPtr,
                     hns_rewards::mojom::GetExternalWalletError>* out);
};

}  // namespace mojo

#endif  // HNS_COMPONENTS_HNS_REWARDS_COMMON_MOJOM_REWARDS_TYPES_MOJOM_TRAITS_H_
