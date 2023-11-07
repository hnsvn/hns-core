/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.hns.bytecode;

import org.objectweb.asm.ClassVisitor;

public class HnsHelpAndFeedbackLauncherImplClassAdapter extends HnsClassVisitor {
    static String sHelpAndFeedbackLauncherImplClassName =
            "org/chromium/chrome/browser/feedback/HelpAndFeedbackLauncherImpl";

    static String sHnsHelpAndFeedbackLauncherImplClassName =
            "org/chromium/chrome/browser/feedback/HnsHelpAndFeedbackLauncherImpl";

    public HnsHelpAndFeedbackLauncherImplClassAdapter(ClassVisitor visitor) {
        super(visitor);

        redirectConstructor(
                sHelpAndFeedbackLauncherImplClassName, sHnsHelpAndFeedbackLauncherImplClassName);
    }
}
