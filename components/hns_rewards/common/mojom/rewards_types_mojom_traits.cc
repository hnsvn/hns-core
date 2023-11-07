/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_rewards/common/mojom/rewards_types_mojom_traits.h"

#include <utility>

#include "hns/components/hns_rewards/common/mojom/rewards_types.mojom.h"

namespace mojo {

// static
hns_rewards::mojom::ConnectExternalWalletValuePtr UnionTraits<
    hns_rewards::mojom::ConnectExternalWalletResultDataView,
    base::expected<void, hns_rewards::mojom::ConnectExternalWalletError>>::
    value(
        const base::expected<void,
                             hns_rewards::mojom::ConnectExternalWalletError>&
            result) {
  DCHECK(result.has_value());
  return hns_rewards::mojom::ConnectExternalWalletValue::New();
}

// static
hns_rewards::mojom::ConnectExternalWalletError UnionTraits<
    hns_rewards::mojom::ConnectExternalWalletResultDataView,
    base::expected<void, hns_rewards::mojom::ConnectExternalWalletError>>::
    error(
        const base::expected<void,
                             hns_rewards::mojom::ConnectExternalWalletError>&
            result) {
  DCHECK(!result.has_value());
  return result.error();
}

// static
hns_rewards::mojom::ConnectExternalWalletResultDataView::Tag UnionTraits<
    hns_rewards::mojom::ConnectExternalWalletResultDataView,
    base::expected<void, hns_rewards::mojom::ConnectExternalWalletError>>::
    GetTag(
        const base::expected<void,
                             hns_rewards::mojom::ConnectExternalWalletError>&
            result) {
  return result.has_value()
             ? hns_rewards::mojom::ConnectExternalWalletResultDataView::Tag::
                   kValue
             : hns_rewards::mojom::ConnectExternalWalletResultDataView::Tag::
                   kError;
}

// static
bool UnionTraits<
    hns_rewards::mojom::ConnectExternalWalletResultDataView,
    base::expected<void, hns_rewards::mojom::ConnectExternalWalletError>>::
    Read(hns_rewards::mojom::ConnectExternalWalletResultDataView data,
         base::expected<void, hns_rewards::mojom::ConnectExternalWalletError>*
             out) {
  switch (data.tag()) {
    case hns_rewards::mojom::ConnectExternalWalletResultDataView::Tag::kValue:
      *out = {};
      return true;
    case hns_rewards::mojom::ConnectExternalWalletResultDataView::Tag::kError:
      hns_rewards::mojom::ConnectExternalWalletError error;
      if (data.ReadError(&error)) {
        *out = base::unexpected(error);
        return true;
      }

      break;
  }

  return false;
}

// static
hns_rewards::mojom::FetchBalanceValuePtr
UnionTraits<hns_rewards::mojom::FetchBalanceResultDataView,
            base::expected<hns_rewards::mojom::BalancePtr,
                           hns_rewards::mojom::FetchBalanceError>>::
    value(
        const base::expected<hns_rewards::mojom::BalancePtr,
                             hns_rewards::mojom::FetchBalanceError>& result) {
  DCHECK(result.has_value());
  return hns_rewards::mojom::FetchBalanceValue::New(result.value()->Clone());
}

// static
hns_rewards::mojom::FetchBalanceError
UnionTraits<hns_rewards::mojom::FetchBalanceResultDataView,
            base::expected<hns_rewards::mojom::BalancePtr,
                           hns_rewards::mojom::FetchBalanceError>>::
    error(
        const base::expected<hns_rewards::mojom::BalancePtr,
                             hns_rewards::mojom::FetchBalanceError>& result) {
  DCHECK(!result.has_value());
  return result.error();
}

// static
hns_rewards::mojom::FetchBalanceResultDataView::Tag
UnionTraits<hns_rewards::mojom::FetchBalanceResultDataView,
            base::expected<hns_rewards::mojom::BalancePtr,
                           hns_rewards::mojom::FetchBalanceError>>::
    GetTag(
        const base::expected<hns_rewards::mojom::BalancePtr,
                             hns_rewards::mojom::FetchBalanceError>& result) {
  return result.has_value()
             ? hns_rewards::mojom::FetchBalanceResultDataView::Tag::kValue
             : hns_rewards::mojom::FetchBalanceResultDataView::Tag::kError;
}

// static
bool UnionTraits<hns_rewards::mojom::FetchBalanceResultDataView,
                 base::expected<hns_rewards::mojom::BalancePtr,
                                hns_rewards::mojom::FetchBalanceError>>::
    Read(hns_rewards::mojom::FetchBalanceResultDataView data,
         base::expected<hns_rewards::mojom::BalancePtr,
                        hns_rewards::mojom::FetchBalanceError>* out) {
  switch (data.tag()) {
    case hns_rewards::mojom::FetchBalanceResultDataView::Tag::kValue: {
      hns_rewards::mojom::FetchBalanceValuePtr value;
      if (data.ReadValue(&value)) {
        *out = std::move(value->balance);
        return true;
      }

      break;
    }
    case hns_rewards::mojom::FetchBalanceResultDataView::Tag::kError: {
      hns_rewards::mojom::FetchBalanceError error;
      if (data.ReadError(&error)) {
        *out = base::unexpected(error);
        return true;
      }

      break;
    }
  }

  return false;
}

// static
hns_rewards::mojom::GetExternalWalletValuePtr
UnionTraits<hns_rewards::mojom::GetExternalWalletResultDataView,
            base::expected<hns_rewards::mojom::ExternalWalletPtr,
                           hns_rewards::mojom::GetExternalWalletError>>::
    value(const base::expected<hns_rewards::mojom::ExternalWalletPtr,
                               hns_rewards::mojom::GetExternalWalletError>&
              result) {
  DCHECK(result.has_value());
  return hns_rewards::mojom::GetExternalWalletValue::New(
      result.value()->Clone());
}

// static
hns_rewards::mojom::GetExternalWalletError
UnionTraits<hns_rewards::mojom::GetExternalWalletResultDataView,
            base::expected<hns_rewards::mojom::ExternalWalletPtr,
                           hns_rewards::mojom::GetExternalWalletError>>::
    error(const base::expected<hns_rewards::mojom::ExternalWalletPtr,
                               hns_rewards::mojom::GetExternalWalletError>&
              result) {
  DCHECK(!result.has_value());
  return result.error();
}

// static
hns_rewards::mojom::GetExternalWalletResultDataView::Tag
UnionTraits<hns_rewards::mojom::GetExternalWalletResultDataView,
            base::expected<hns_rewards::mojom::ExternalWalletPtr,
                           hns_rewards::mojom::GetExternalWalletError>>::
    GetTag(const base::expected<hns_rewards::mojom::ExternalWalletPtr,
                                hns_rewards::mojom::GetExternalWalletError>&
               result) {
  return result.has_value() ? hns_rewards::mojom::
                                  GetExternalWalletResultDataView::Tag::kValue
                            : hns_rewards::mojom::
                                  GetExternalWalletResultDataView::Tag::kError;
}

// static
bool UnionTraits<hns_rewards::mojom::GetExternalWalletResultDataView,
                 base::expected<hns_rewards::mojom::ExternalWalletPtr,
                                hns_rewards::mojom::GetExternalWalletError>>::
    Read(hns_rewards::mojom::GetExternalWalletResultDataView data,
         base::expected<hns_rewards::mojom::ExternalWalletPtr,
                        hns_rewards::mojom::GetExternalWalletError>* out) {
  switch (data.tag()) {
    case hns_rewards::mojom::GetExternalWalletResultDataView::Tag::kValue: {
      hns_rewards::mojom::GetExternalWalletValuePtr value;
      if (data.ReadValue(&value)) {
        *out = std::move(value->wallet);
        return true;
      }

      break;
    }
    case hns_rewards::mojom::GetExternalWalletResultDataView::Tag::kError: {
      hns_rewards::mojom::GetExternalWalletError error;
      if (data.ReadError(&error)) {
        *out = base::unexpected(error);
        return true;
      }

      break;
    }
  }

  return false;
}

}  // namespace mojo
