/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.vpn.models;

public class HnsVpnServerRegion {
    private String mContinent;
    private String mName;
    private String mNamePretty;

    public HnsVpnServerRegion(String continent, String name, String namePretty) {
        this.mContinent = continent;
        this.mName = name;
        this.mNamePretty = namePretty;
    }

    public String getContinent() {
        return mContinent;
    }

    public String getName() {
        return mName;
    }

    public String getNamePretty() {
        return mNamePretty;
    }
}
