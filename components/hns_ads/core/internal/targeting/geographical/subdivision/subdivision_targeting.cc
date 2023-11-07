/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting.h"

#include "hns/components/hns_ads/core/internal/client/ads_client_helper.h"
#include "hns/components/hns_ads/core/internal/common/locale/locale_util.h"
#include "hns/components/hns_ads/core/internal/common/logging_util.h"
#include "hns/components/hns_ads/core/internal/common/subdivision/subdivision_util.h"
#include "hns/components/hns_ads/core/internal/settings/settings.h"
#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_targeting_util.h"
#include "hns/components/hns_ads/core/internal/targeting/geographical/subdivision/subdivision_url_request.h"
#include "hns/components/hns_ads/core/public/prefs/pref_names.h"
#include "hns/components/hns_news/common/pref_names.h"
#include "hns/components/hns_rewards/common/pref_names.h"
#include "hns/components/l10n/common/locale_util.h"

namespace hns_ads {

namespace {

constexpr char kAuto[] = "AUTO";
constexpr char kDisabled[] = "DISABLED";

bool DoesRequireResource() {
  return UserHasOptedInToHnsNewsAds() || UserHasOptedInToNotificationAds();
}

}  // namespace

SubdivisionTargeting::SubdivisionTargeting() {
  AdsClientHelper::AddObserver(this);
}

SubdivisionTargeting::~SubdivisionTargeting() {
  AdsClientHelper::RemoveObserver(this);
}

bool SubdivisionTargeting::IsDisabled() const {
  return GetLazySubdivision() == kDisabled;
}

bool SubdivisionTargeting::ShouldAutoDetect() const {
  return GetLazySubdivision() == kAuto;
}

// static
bool SubdivisionTargeting::ShouldAllow() {
  return AdsClientHelper::GetInstance()->GetBooleanPref(
      prefs::kShouldAllowSubdivisionTargeting);
}

const std::string& SubdivisionTargeting::GetSubdivision() const {
  return ShouldAutoDetect() ? GetLazyAutoDetectedSubdivision()
                            : GetLazySubdivision();
}

///////////////////////////////////////////////////////////////////////////////

void SubdivisionTargeting::Initialize() {
  MaybeRequireSubdivision();

  MaybeAllowAndFetchSubdivisionForLocale(GetLocale());
}

void SubdivisionTargeting::MaybeRequireSubdivision() {
  DoesRequireResource() ? InitializeSubdivisionUrlRequest()
                        : ShutdownSubdivisionUrlRequest();
}

void SubdivisionTargeting::InitializeSubdivisionUrlRequest() {
  if (!subdivision_url_request_) {
    BLOG(1, "Initialize subdivision URL request");
    subdivision_url_request_ = std::make_unique<SubdivisionUrlRequest>();
    subdivision_url_request_->SetDelegate(this);
  }
}

void SubdivisionTargeting::ShutdownSubdivisionUrlRequest() {
  if (subdivision_url_request_) {
    subdivision_url_request_.reset();
    BLOG(1, "Shutdown subdivision URL request");
  }
}

void SubdivisionTargeting::DisableSubdivision() {
  if (!IsDisabled()) {
    SetSubdivision(kDisabled);
  }
}

void SubdivisionTargeting::AutoDetectSubdivision() {
  if (!ShouldAutoDetect()) {
    SetSubdivision(kAuto);
  }
}

void SubdivisionTargeting::MaybeAllowForLocale(const std::string& locale) {
  if (!DoesRequireResource()) {
    return AdsClientHelper::GetInstance()->SetBooleanPref(
        prefs::kShouldAllowSubdivisionTargeting, false);
  }

  const std::string country_code = hns_l10n::GetISOCountryCode(locale);

  if (!ShouldTargetSubdivisionCountryCode(country_code)) {
    BLOG(1, "Subdivision targeting is unsupported for " << country_code
                                                        << " country code");

    return AdsClientHelper::GetInstance()->SetBooleanPref(
        prefs::kShouldAllowSubdivisionTargeting, false);
  }

  if (IsDisabled()) {
    return AdsClientHelper::GetInstance()->SetBooleanPref(
        prefs::kShouldAllowSubdivisionTargeting, true);
  }

  const std::string& subdivision = GetSubdivision();

  absl::optional<std::string> subdivision_country_code;
  if (!subdivision.empty()) {
    subdivision_country_code = GetSubdivisionCountryCode(subdivision);
  }

  if (country_code != subdivision_country_code) {
    AutoDetectSubdivision();

    return AdsClientHelper::GetInstance()->SetBooleanPref(
        prefs::kShouldAllowSubdivisionTargeting, false);
  }

  if (!ShouldTargetSubdivision(country_code, subdivision)) {
    BLOG(1, subdivision << " subdivision is unsupported for " << country_code
                        << " country code");

    DisableSubdivision();
  }

  AdsClientHelper::GetInstance()->SetBooleanPref(
      prefs::kShouldAllowSubdivisionTargeting, true);
}

bool SubdivisionTargeting::ShouldFetchSubdivision() {
  if (IsDisabled()) {
    BLOG(1, "Subdivision targeting is disabled");
    return false;
  }

  if (!ShouldAutoDetect()) {
    BLOG(1, "Subdivision targeting is set to " << GetLazySubdivision());
    return false;
  }

  return true;
}

void SubdivisionTargeting::MaybeFetchSubdivision() {
  if (subdivision_url_request_ && ShouldFetchSubdivision()) {
    subdivision_url_request_->PeriodicallyFetch();
  }
}

void SubdivisionTargeting::MaybeAllowAndFetchSubdivisionForLocale(
    const std::string& locale) {
  MaybeAllowForLocale(locale);
  MaybeFetchSubdivision();
}

void SubdivisionTargeting::SetAutoDetectedSubdivision(
    const std::string& subdivision) {
  CHECK(!subdivision.empty());

  if (auto_detected_subdivision_ != subdivision) {
    BLOG(1, "Automatically detected " << subdivision << " subdivision");

    auto_detected_subdivision_ = subdivision;
    AdsClientHelper::GetInstance()->SetStringPref(
        prefs::kSubdivisionTargetingAutoDetectedSubdivision, subdivision);
  }
}

void SubdivisionTargeting::UpdateAutoDetectedSubdivision() {
  const std::string auto_detected_subdivision =
      AdsClientHelper::GetInstance()->GetStringPref(
          prefs::kSubdivisionTargetingAutoDetectedSubdivision);

  if (auto_detected_subdivision_ != auto_detected_subdivision) {
    auto_detected_subdivision_ = auto_detected_subdivision;
    BLOG(1, "Changed to automatically detected " << auto_detected_subdivision
                                                 << " subdivision");
  }
}

const std::string& SubdivisionTargeting::GetLazyAutoDetectedSubdivision()
    const {
  if (!auto_detected_subdivision_) {
    auto_detected_subdivision_ = AdsClientHelper::GetInstance()->GetStringPref(
        prefs::kSubdivisionTargetingAutoDetectedSubdivision);
  }

  return *auto_detected_subdivision_;
}

void SubdivisionTargeting::SetSubdivision(const std::string& subdivision) {
  CHECK(!subdivision.empty());

  if (subdivision_ != subdivision) {
    subdivision_ = subdivision;
    AdsClientHelper::GetInstance()->SetStringPref(
        prefs::kSubdivisionTargetingSubdivision, *subdivision_);
  }
}

void SubdivisionTargeting::UpdateSubdivision() {
  const std::string subdivision = AdsClientHelper::GetInstance()->GetStringPref(
      prefs::kSubdivisionTargetingSubdivision);

  if (subdivision_ != subdivision) {
    subdivision_ = subdivision;
    BLOG(1, "Subdivision changed to " << subdivision);

    if (ShouldAutoDetect()) {
      MaybeFetchSubdivision();
    }
  }
}

const std::string& SubdivisionTargeting::GetLazySubdivision() const {
  if (!subdivision_) {
    subdivision_ = AdsClientHelper::GetInstance()->GetStringPref(
        prefs::kSubdivisionTargetingSubdivision);
  }

  return *subdivision_;
}

void SubdivisionTargeting::OnNotifyDidInitializeAds() {
  Initialize();
}

void SubdivisionTargeting::OnNotifyLocaleDidChange(const std::string& locale) {
  MaybeAllowAndFetchSubdivisionForLocale(locale);
}

void SubdivisionTargeting::OnNotifyPrefDidChange(const std::string& path) {
  if (path == hns_rewards::prefs::kEnabled ||
      path == prefs::kOptedInToNotificationAds ||
      path == hns_news::prefs::kHnsNewsOptedIn ||
      path == hns_news::prefs::kNewTabPageShowToday) {
    Initialize();
  } else if (path == prefs::kSubdivisionTargetingAutoDetectedSubdivision) {
    UpdateAutoDetectedSubdivision();
  } else if (path == prefs::kSubdivisionTargetingSubdivision) {
    UpdateSubdivision();
  }
}

void SubdivisionTargeting::OnDidFetchSubdivision(
    const std::string& subdivision) {
  SetAutoDetectedSubdivision(subdivision);
  MaybeAllowForLocale(GetLocale());
}

}  // namespace hns_ads
