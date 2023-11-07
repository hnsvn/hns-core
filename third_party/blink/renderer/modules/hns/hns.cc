/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/third_party/blink/renderer/modules/hns/hns.h"

#include "third_party/blink/renderer/bindings/core/v8/script_promise.h"
#include "third_party/blink/renderer/bindings/core/v8/script_promise_resolver.h"

namespace blink {

const char Hns::kSupplementName[] = "Hns";

Hns::Hns(NavigatorBase& navigator) : Supplement<NavigatorBase>(navigator) {}

Hns* Hns::hns(NavigatorBase& navigator) {
  auto* supplement = Supplement<NavigatorBase>::From<Hns>(navigator);
  if (!supplement) {
    supplement = MakeGarbageCollected<Hns>(navigator);
    Supplement<NavigatorBase>::ProvideTo(navigator, supplement);
  }
  return supplement;
}

ScriptPromise Hns::isHns(ScriptState* script_state) {
  auto* resolver = MakeGarbageCollected<ScriptPromiseResolver>(script_state);
  ScriptPromise promise = resolver->Promise();
  resolver->Resolve(true);
  return promise;
}

void Hns::Trace(Visitor* visitor) const {
  ScriptWrappable::Trace(visitor);
  Supplement<NavigatorBase>::Trace(visitor);
}

}  // namespace blink
