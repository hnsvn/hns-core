/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

function arrayBufsMatch(ab1, ab2) {
  const dataView1 = new DataView(ab1)
  const dataView2 = new DataView(ab2)
  console.log('dataView1 byte length: ', dataView1.byteLength)
  console.log('dataView2 byte length: ', dataView2.byteLength)
  if (dataView1.byteLength === 32 &&
      dataView1.byteLength === dataView2.byteLength) {
    for (let i = 0; i < dataView1.byteLength; i++) {
      if (dataView1.getUint8(i) !== dataView2.getUint8(i)) {
        console.log('Mismatch at position i: ', i)
        return false;
      }
    }
  }
  return true;
}

function testKnownSeedValuesEndToEnd() {
  chrome.test.runTests([
    function testKnownSeedValues() {
      const buf = new Uint8Array([
        196, 34, 104, 152, 91, 63, 78, 171,
        234, 163, 25, 221, 80, 73, 158, 89,
        52, 53, 227, 231, 152, 214, 61, 210,
        33, 54, 68, 171, 140, 239, 3, 158])
      const expectedResult = new Uint8Array([
        142, 147, 10, 180, 36, 89, 142, 110,
        52, 85, 216, 222, 83, 56, 38, 206,
        104, 133, 77, 246, 219, 90, 105, 35,
        52, 76, 223, 24, 183, 138, 244, 72])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const expectedResultArrayBuf =
          expectedResult.buffer.slice(expectedResult.byteOffset,
            expectedResult.byteLength + expectedResult.byteOffset)
      chrome.hnsWallet.getWalletSeed(buf, (resultArrayBuf) => {
        if (arrayBufsMatch(resultArrayBuf, expectedResultArrayBuf)) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsCryptoWallets() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.hnsWallet.getWeb3Provider((provider) => {
        if (provider === 'odbfpeeihdkbihmopkbjmoonfanlbfcl') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsHnsWalletPreferExtension() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.hnsWallet.getWeb3Provider((provider) => {
        // This is just a placeholder value to make CW think
        // it is set. But really it prefers extensions
        if (provider === 'odbfpeeihdkbihmopkbjmoonfanlbfcl') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsNone() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.hnsWallet.getWeb3Provider((provider) => {
        if (provider.length === 0) {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testProviderIsHnsWallet() {
  chrome.test.runTests([
    function CryptoWalletsIsProvider() {
      chrome.hnsWallet.getWeb3Provider((provider) => {
        if (provider === 'odbfpeeihdkbihmopkbjmoonfanlbfcl') {
          chrome.test.succeed();
        } else {
          chrome.test.fail();
        }
      })
    }
  ])
}

function testBasics() {
  chrome.test.runTests([
    function hnsWalletExtensionHasAccess() {
      if (chrome.hnsWallet &&
          chrome.hnsWallet.shouldPromptForSetup &&
          chrome.hnsWallet.notifyWalletUnlock &&
          chrome.hnsWallet.loadUI) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
    function hnsWalletExtensionSeedFunctionHasAccess() {
      if (chrome.hnsWallet && chrome.hnsWallet.getWalletSeed &&
          chrome.hnsWallet.getProjectID &&
          chrome.hnsWallet.getHnsKey &&
          chrome.hnsWallet.getWeb3Provider) {
        chrome.test.succeed();
      } else {
        chrome.test.fail();
      }
    },
    function hnsWalletWrongInputKeySizeFails() {
      const buf = new Uint8Array([...Array(3).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.hnsWallet.getWalletSeed(arraybuf, (seed) => {
        if (!seed) {
          chrome.test.succeed();
          return
        }
        chrome.test.fail();
      })
    },
    function hnsWalletExtensionGetWalletSeedSameSeedOnMultipleRequests() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      chrome.hnsWallet.getWalletSeed(arraybuf, (firstSeed) => {
        chrome.hnsWallet.getWalletSeed(arraybuf, (secondSeed) => {
          if (arrayBufsMatch(firstSeed, secondSeed)) {
            chrome.test.succeed();
            return
          }
          console.error('Seeds differ across calls!')
          chrome.test.fail();
        })
      })
    },
    function hnsWalletExtensionGetWalletSeedDifferentKeysDoesNotWork() {
      const buf = new Uint8Array([...Array(32).keys()])
      const arraybuf = buf.buffer.slice(buf.byteOffset,
          buf.byteLength + buf.byteOffset)
      const buf2 = new Uint8Array([11, ...Array(31).keys()])
      const arraybuf2 = buf2.buffer.slice(buf2.byteOffset, buf2.byteLength + buf2.byteOffset)
      chrome.hnsWallet.getWalletSeed(arraybuf, (firstSeed) => {
        chrome.hnsWallet.getWalletSeed(arraybuf2, (secondSeed) => {
          if (firstSeed && !secondSeed) {
            chrome.test.succeed();
            return
          }
          chrome.test.fail();
        })
      })
    },
    function hnsWalletExtensionGetProjectIDWorks() {
      chrome.hnsWallet.getProjectID((projectID) => {
        if (projectID === "test-project-id") {
          chrome.test.succeed();
          return
        }
        console.log('Failed project ID is: ' + projectID)
        chrome.test.fail();
      })
    },
    function hnsWalletExtensionGetHnsKeyWorks() {
      chrome.hnsWallet.getHnsKey((key) => {
        if (key === "test-hns-key") {
          chrome.test.succeed();
          return
        }
        console.log('Failure, hns key is: ' + key)
        chrome.test.fail();
      })
    }
  ]);
}
