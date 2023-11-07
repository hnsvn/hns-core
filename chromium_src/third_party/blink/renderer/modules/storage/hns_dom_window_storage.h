/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_STORAGE_HNS_DOM_WINDOW_STORAGE_H_
#define HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_STORAGE_HNS_DOM_WINDOW_STORAGE_H_

#include "third_party/blink/renderer/platform/supplementable.h"

namespace blink {

class ExceptionState;
class LocalDOMWindow;
class SecurityOrigin;
class StorageArea;

const SecurityOrigin* GetEphemeralStorageOrigin(LocalDOMWindow* window);

class HnsDOMWindowStorage final
    : public GarbageCollected<HnsDOMWindowStorage>,
      public Supplement<LocalDOMWindow> {
 public:
  static const char kSupplementName[];

  static HnsDOMWindowStorage& From(LocalDOMWindow&);
  static StorageArea* sessionStorage(LocalDOMWindow&, ExceptionState&);
  static StorageArea* localStorage(LocalDOMWindow&, ExceptionState&);

  StorageArea* sessionStorage(ExceptionState&);
  StorageArea* localStorage(ExceptionState&);

  explicit HnsDOMWindowStorage(LocalDOMWindow&);

  void Trace(Visitor*) const override;

 private:
  StorageArea* ephemeralSessionStorage();

  mutable Member<StorageArea> ephemeral_session_storage_;
  mutable Member<StorageArea> ephemeral_local_storage_;
};

}  // namespace blink

#endif  // HNS_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_MODULES_STORAGE_HNS_DOM_WINDOW_STORAGE_H_
