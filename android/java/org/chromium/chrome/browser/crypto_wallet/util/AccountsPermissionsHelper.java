/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

package org.chromium.chrome.browser.crypto_wallet.util;

import org.chromium.hns_wallet.mojom.AccountId;
import org.chromium.hns_wallet.mojom.AccountInfo;
import org.chromium.hns_wallet.mojom.HnsWalletService;

import java.util.Arrays;
import java.util.HashSet;
import java.util.stream.Collectors;

public class AccountsPermissionsHelper {
    private HnsWalletService mHnsWalletService;
    private AccountInfo[] mAccounts;
    private HashSet<AccountInfo> mAccountsWithPermissions;

    public AccountsPermissionsHelper(
            HnsWalletService hnsWalletService, AccountInfo[] accounts) {
        assert hnsWalletService != null;
        assert accounts != null;
        mHnsWalletService = hnsWalletService;
        mAccounts = accounts;
        mAccountsWithPermissions = new HashSet<AccountInfo>();
    }

    public HashSet<AccountInfo> getAccountsWithPermissions() {
        return mAccountsWithPermissions;
    }

    private static boolean containsAccount(AccountId[] accounts, AccountId searchFor) {
        return Arrays.stream(accounts).anyMatch(
                acc -> { return WalletUtils.accountIdsEqual(acc, searchFor); });
    }

    public void checkAccounts(Runnable runWhenDone) {
        AccountId[] allAccountIds =
                Arrays.stream(mAccounts).map(acc -> acc.accountId).toArray(AccountId[] ::new);
        mHnsWalletService.hasPermission(allAccountIds, (success, filteredAccounts) -> {
            mAccountsWithPermissions =
                    Arrays.stream(mAccounts)
                            .filter(acc -> containsAccount(filteredAccounts, acc.accountId))
                            .collect(Collectors.toCollection(HashSet::new));

            runWhenDone.run();
        });
    }
}
