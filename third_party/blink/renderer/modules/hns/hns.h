// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef HNS_THIRD_PARTY_BLINK_RENDERER_MODULES_HNS_HNS_H_
#define HNS_THIRD_PARTY_BLINK_RENDERER_MODULES_HNS_HNS_H_

#include "third_party/blink/renderer/core/execution_context/navigator_base.h"
#include "third_party/blink/renderer/modules/modules_export.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"

namespace blink {

class ScriptPromise;
class ScriptState;

class MODULES_EXPORT Hns final : public ScriptWrappable,
                                   public Supplement<NavigatorBase> {
  DEFINE_WRAPPERTYPEINFO();

 public:
  static const char kSupplementName[];

  static Hns* hns(NavigatorBase& navigator);

  explicit Hns(NavigatorBase& navigator);
  ~Hns() override = default;

  void Trace(Visitor*) const override;

  ScriptPromise isHns(ScriptState*);
};

}  // namespace blink

#endif  // HNS_THIRD_PARTY_BLINK_RENDERER_MODULES_HNS_HNS_H_
