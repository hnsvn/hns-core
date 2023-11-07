/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsVariationsSeedFetcherClassAdapter extends HnsClassVisitor {
    static String sVariationsSeedFetcherClassName =
            "org/chromium/components/variations/firstrun/VariationsSeedFetcher";
    static String sHnsVariationsSeedFetcherClassName =
            "org/chromium/components/variations/firstrun/HnsVariationsSeedFetcher";

    public HnsVariationsSeedFetcherClassAdapter(ClassVisitor visitor) {
        super(visitor);

        changeMethodOwner(
                sVariationsSeedFetcherClassName, "get", sHnsVariationsSeedFetcherClassName);
    }
}
