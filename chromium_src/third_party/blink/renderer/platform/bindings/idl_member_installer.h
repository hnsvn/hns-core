/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_BINDINGS_IDL_MEMBER_INSTALLER_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_BINDINGS_IDL_MEMBER_INSTALLER_H_

struct HnsNavigatorAttributeInstallerTrait;

#define HNS_IDL_MEMBER_INSTALLER_H_                     \
  template <typename T>                                   \
  static void HnsInstallAttributes(                     \
      v8::Isolate* isolate, const DOMWrapperWorld& world, \
      v8::Local<v8::Template> instance_template,          \
      v8::Local<v8::Template> prototype_template,         \
      v8::Local<v8::Template> interface_template,         \
      v8::Local<v8::Signature> signature,                 \
      base::span<const AttributeConfig> configs);         \
  template <typename T>                                   \
  static void HnsInstallAttributes(                     \
      v8::Isolate* isolate, const DOMWrapperWorld& world, \
      v8::Local<v8::Object> instance_object,              \
      v8::Local<v8::Object> prototype_object,             \
      v8::Local<v8::Object> interface_object,             \
      v8::Local<v8::Signature> signature,                 \
      base::span<const AttributeConfig> configs);

#include "src/third_party/blink/renderer/platform/bindings/idl_member_installer.h"  // IWYU pragma: export
#undef HNS_IDL_MEMBER_INSTALLER_H_

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_PLATFORM_BINDINGS_IDL_MEMBER_INSTALLER_H_
