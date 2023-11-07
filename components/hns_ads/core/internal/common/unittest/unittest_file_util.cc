/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_ads/core/internal/common/unittest/unittest_file_util.h"

#include "base/base_paths.h"
#include "base/files/file_util.h"
#include "base/path_service.h"
#include "hns/components/hns_ads/core/internal/common/unittest/unittest_tag_parser_util.h"

namespace hns_ads {

namespace {

absl::optional<std::string> ReadFileToString(const base::FilePath& path) {
  std::string content;
  if (!base::ReadFileToString(path, &content)) {
    return absl::nullopt;
  }

  return content;
}

}  // namespace

base::FilePath GetTestPath() {
  base::FilePath path;
  base::PathService::Get(base::DIR_SOURCE_ROOT, &path);
  return path.AppendASCII("hns")
      .AppendASCII("components")
      .AppendASCII("hns_ads")
      .AppendASCII("core")
      .AppendASCII("test")
      .AppendASCII("data");
}

absl::optional<std::string> ReadFileFromTestPathToString(
    const std::string& name) {
  const base::FilePath path = GetTestPath().AppendASCII(name);
  return ReadFileToString(path);
}

absl::optional<std::string> ReadFileFromTestPathAndParseTagsToString(
    const std::string& name) {
  absl::optional<std::string> content = ReadFileFromTestPathToString(name);
  if (!content) {
    return absl::nullopt;
  }

  ParseAndReplaceTags(*content);

  return *content;
}

base::FilePath GetFileResourcePath() {
  return GetTestPath().AppendASCII("resources");
}

base::FilePath GetDataResourcePath() {
  base::FilePath path;
  base::PathService::Get(base::DIR_SOURCE_ROOT, &path);
  return path.AppendASCII("hns")
      .AppendASCII("components")
      .AppendASCII("hns_ads")
      .AppendASCII("resources");
}

absl::optional<std::string> ReadFileFromDataResourcePathToString(
    const std::string& name) {
  const base::FilePath path = GetDataResourcePath().AppendASCII(name);
  return ReadFileToString(path);
}

}  // namespace hns_ads
