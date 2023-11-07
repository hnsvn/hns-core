/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/sandbox/win/src/module_file_name_interception.h"

#include <string.h>
#include <algorithm>
#include <string>

#include "base/strings/string_piece.h"
#include "base/strings/string_util.h"
#include "base/win/windows_types.h"
#include "third_party/abseil-cpp/absl/types/optional.h"

namespace {

void ReplaceAt(char* dest, size_t dest_size, base::StringPiece src) {
  ::strncpy_s(dest, dest_size, src.data(),
              std::min(dest_size - 1, src.length()));
}

void ReplaceAt(wchar_t* dest, size_t dest_size, base::WStringPiece src) {
  ::wcsncpy_s(dest, dest_size, src.data(),
              std::min(dest_size - 1, src.length()));
}

template <typename CharT>
struct HnsToChrome;

template <>
struct HnsToChrome<char> {
  static constexpr const base::StringPiece kHns = "hns.exe";
  static constexpr const base::StringPiece kChrome = "chrome.exe";
};

template <>
struct HnsToChrome<wchar_t> {
  static constexpr const base::WStringPiece kHns = L"hns.exe";
  static constexpr const base::WStringPiece kChrome = L"chrome.exe";
};

template <typename CharT>
struct TestHnsToChrome;

template <>
struct TestHnsToChrome<char> {
  static constexpr const base::StringPiece kHns = "hns_browser_tests.exe";
  static constexpr const base::StringPiece kChrome = "chrome_browser_tests.exe";
};

template <>
struct TestHnsToChrome<wchar_t> {
  static constexpr const base::WStringPiece kHns = L"hns_browser_tests.exe";
  static constexpr const base::WStringPiece kChrome =
      L"chrome_browser_tests.exe";
};

template <template <class T> class FromTo, typename CharT>
absl::optional<DWORD> PatchFilenameImpl(CharT* filename,
                                        DWORD length,
                                        DWORD size) {
  if (!base::EndsWith(base::BasicStringPiece<CharT>(filename, length),
                      FromTo<CharT>::kHns,
                      base::CompareCase::INSENSITIVE_ASCII)) {
    return absl::nullopt;
  }

  constexpr DWORD kHnsLen = FromTo<CharT>::kHns.length();
  constexpr DWORD kChromeLen = FromTo<CharT>::kChrome.length();
  static_assert(kHnsLen <= kChromeLen);
  constexpr DWORD kLenDiff = kChromeLen - kHnsLen;

  --size;  // space for null-terminator

  const size_t hns_pos = length - kHnsLen;
  ReplaceAt(filename + hns_pos, size - hns_pos, FromTo<CharT>::kChrome);
  if (size < length + kLenDiff) {
    ::SetLastError(ERROR_INSUFFICIENT_BUFFER);
  }
  length = std::min(size, length + kLenDiff);
  filename[length] = 0;
  return length;
}

template <typename CharT>
DWORD PatchFilename(CharT* filename, DWORD length, DWORD size) {
  if (auto r = PatchFilenameImpl<HnsToChrome>(filename, length, size)) {
    return *r;
  }
  if (auto r = PatchFilenameImpl<TestHnsToChrome>(filename, length, size)) {
    return *r;
  }
  return length;
}

}  // namespace

namespace sandbox {

SANDBOX_INTERCEPT DWORD WINAPI
TargetGetModuleFileNameA(GetModuleFileNameAFunction orig,
                         HMODULE hModule,
                         LPSTR lpFilename,
                         DWORD nSize) {
  const auto result = orig(hModule, lpFilename, nSize);
  if (result != 0) {
    return PatchFilename(lpFilename, result, nSize);
  }
  return result;
}

SANDBOX_INTERCEPT DWORD WINAPI
TargetGetModuleFileNameW(GetModuleFileNameWFunction orig,
                         HMODULE hModule,
                         LPWSTR lpFilename,
                         DWORD nSize) {
  const auto result = orig(hModule, lpFilename, nSize);
  if (result != 0) {
    return PatchFilename(lpFilename, result, nSize);
  }
  return result;
}

SANDBOX_INTERCEPT DWORD WINAPI
TargetGetModuleFileNameExA(GetModuleFileNameExAFunction orig,
                           HANDLE hProcess,
                           HMODULE hModule,
                           LPSTR lpFilename,
                           DWORD nSize) {
  const auto result = orig(hProcess, hModule, lpFilename, nSize);
  if (result != 0) {
    return PatchFilename(lpFilename, result, nSize);
  }
  return result;
}

SANDBOX_INTERCEPT DWORD WINAPI
TargetGetModuleFileNameExW(GetModuleFileNameExWFunction orig,
                           HANDLE hProcess,
                           HMODULE hModule,
                           LPWSTR lpFilename,
                           DWORD nSize) {
  const auto result = orig(hProcess, hModule, lpFilename, nSize);
  if (result != 0) {
    return PatchFilename(lpFilename, result, nSize);
  }
  return result;
}

}  // namespace sandbox
