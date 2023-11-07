/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

function testBasics() {
  chrome.test.runTests([
    function hnsShieldsHasWalletAccessButNotSeed() {
      if (chrome.hnsWallet && !chrome.hnsWallet.getWalletSeed &&
          !chrome.hnsWallet.getProjectID &&
          !chrome.hnsWallet.getHnsKey) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
  ]);
}
