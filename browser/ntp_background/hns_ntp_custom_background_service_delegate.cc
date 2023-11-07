// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

#include "hns/browser/ntp_background/hns_ntp_custom_background_service_delegate.h"

#include <utility>

#include "base/files/file_path.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/ntp_background/constants.h"
#include "hns/browser/ntp_background/custom_background_file_manager.h"
#include "hns/browser/ntp_background/ntp_background_prefs.h"
#include "hns/components/constants/pref_names.h"
#include "hns/components/ntp_background_images/browser/ntp_background_images_data.h"
#include "hns/components/ntp_background_images/browser/ntp_background_images_service.h"
#include "hns/components/ntp_background_images/browser/url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/pref_names.h"
#include "chrome/common/url_constants.h"
#include "components/prefs/pref_service.h"
#include "url/gurl.h"

HnsNTPCustomBackgroundServiceDelegate::
    HnsNTPCustomBackgroundServiceDelegate(Profile* profile)
    : profile_(profile),
      file_manager_(std::make_unique<CustomBackgroundFileManager>(profile_)) {
  if (ShouldMigrateCustomImagePref()) {
    DVLOG(2) << "Migrate old custom background image";

    MigrateCustomImage();
  }
}

HnsNTPCustomBackgroundServiceDelegate::
    ~HnsNTPCustomBackgroundServiceDelegate() = default;

bool HnsNTPCustomBackgroundServiceDelegate::ShouldMigrateCustomImagePref()
    const {
  auto prefs = NTPBackgroundPrefs(profile_->GetPrefs());
  return prefs.IsCustomImageType() && prefs.GetCustomImageList().empty();
}

void HnsNTPCustomBackgroundServiceDelegate::MigrateCustomImage(
    base::OnceCallback<void(bool)> callback) {
  auto prefs = NTPBackgroundPrefs(profile_->GetPrefs());
  file_manager_->MoveImage(
      base::FilePath(profile_->GetPath().AppendASCII(
          ntp_background_images::kSanitizedImageFileNameDeprecated)),
      base::BindOnce(
          [](Profile* profile, bool result) {
            auto prefs = NTPBackgroundPrefs(profile->GetPrefs());
            if (!result) {
              LOG(ERROR) << "Failed to migrate Custom background image. "
                            "Resets to default background";
              prefs.SetType(NTPBackgroundPrefs::Type::kHns);
              prefs.SetShouldUseRandomValue(true);
              prefs.SetSelectedValue(std::string());
              return false;
            }

            prefs.SetSelectedValue(
                ntp_background_images::kSanitizedImageFileNameDeprecated);
            prefs.AddCustomImageToList(
                ntp_background_images::kSanitizedImageFileNameDeprecated);
            return true;
          },
          profile_)
          .Then(std::move(callback)));
}

bool HnsNTPCustomBackgroundServiceDelegate::IsCustomImageBackgroundEnabled()
    const {
  if (profile_->GetPrefs()->IsManagedPreference(
          prefs::kNtpCustomBackgroundDict)) {
    return false;
  }

  return NTPBackgroundPrefs(profile_->GetPrefs()).IsCustomImageType();
}

base::FilePath
HnsNTPCustomBackgroundServiceDelegate::GetCustomBackgroundImageLocalFilePath(
    const GURL& url) const {
  return CustomBackgroundFileManager::Converter(url, file_manager_.get())
      .To<base::FilePath>();
}

GURL HnsNTPCustomBackgroundServiceDelegate::GetCustomBackgroundImageURL()
    const {
  DCHECK(IsCustomImageBackgroundEnabled());

  auto prefs = NTPBackgroundPrefs(profile_->GetPrefs());
  auto name = absl::get<std::string>(prefs.GetSelectedValue());
  return CustomBackgroundFileManager::Converter(name).To<GURL>();
}

bool HnsNTPCustomBackgroundServiceDelegate::IsColorBackgroundEnabled() const {
  return NTPBackgroundPrefs(profile_->GetPrefs()).IsColorType();
}

std::string HnsNTPCustomBackgroundServiceDelegate::GetColor() const {
  if (!IsColorBackgroundEnabled()) {
    return {};
  }

  const auto selected_value =
      NTPBackgroundPrefs(profile_->GetPrefs()).GetSelectedValue();
  DCHECK(absl::holds_alternative<std::string>(selected_value));
  return absl::get<std::string>(selected_value);
}

bool HnsNTPCustomBackgroundServiceDelegate::ShouldUseRandomValue() const {
  return NTPBackgroundPrefs(profile_->GetPrefs()).ShouldUseRandomValue();
}

bool HnsNTPCustomBackgroundServiceDelegate::HasPreferredHnsBackground()
    const {
  const auto pref = NTPBackgroundPrefs(profile_->GetPrefs());
  if (!pref.IsHnsType() || pref.ShouldUseRandomValue()) {
    return false;
  }

  auto selected_value = pref.GetSelectedValue();
  if (auto* selected_url = absl::get_if<GURL>(&selected_value)) {
    return selected_url->is_valid();
  }

  return false;
}

base::Value::Dict
HnsNTPCustomBackgroundServiceDelegate::GetPreferredHnsBackground() const {
  DCHECK(HasPreferredHnsBackground());

  auto pref = NTPBackgroundPrefs(profile_->GetPrefs());
  const auto selected_value = pref.GetSelectedValue();
  const auto image_url = absl::get<GURL>(selected_value);

  const auto* service =
      g_hns_browser_process->ntp_background_images_service();
  DCHECK(service);

  auto* image_data = service->GetBackgroundImagesData();
  if (!image_data) {
    // This can happen when the image data is not downloaded yet.
    return {};
  }

  auto iter = base::ranges::find_if(
      image_data->backgrounds, [image_data, &image_url](const auto& data) {
        return image_data->url_prefix +
                   data.image_file.BaseName().AsUTF8Unsafe() ==
               image_url.spec();
      });

  if (iter == image_data->backgrounds.end()) {
    // Due to version update, the data could have been invalidated.
    // Try fixing up the data and return empty value.
    pref.SetShouldUseRandomValue(true);
    pref.SetSelectedValue(base::EmptyString());
    return {};
  }

  return image_data->GetBackgroundAt(
      std::distance(image_data->backgrounds.begin(), iter));
}
