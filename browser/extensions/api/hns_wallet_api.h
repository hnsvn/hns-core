/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_BROWSER_EXTENSIONS_API_HNS_WALLET_API_H_
#define HNS_BROWSER_EXTENSIONS_API_HNS_WALLET_API_H_

#include "extensions/browser/extension_function.h"

class Profile;

namespace extensions {
namespace api {

class HnsWalletReadyFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.ready", UNKNOWN)

 protected:
  ~HnsWalletReadyFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletNotifyWalletUnlockFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.notifyWalletUnlock", UNKNOWN)

 protected:
  ~HnsWalletNotifyWalletUnlockFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletLoadUIFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.loadUI", UNKNOWN)
  void OnLoaded();

 protected:
  ~HnsWalletLoadUIFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletShouldPromptForSetupFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.shouldPromptForSetup", UNKNOWN)

 protected:
  ~HnsWalletShouldPromptForSetupFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletGetWalletSeedFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.getWalletSeed", UNKNOWN)

 protected:
  ~HnsWalletGetWalletSeedFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletGetProjectIDFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.getProjectID", UNKNOWN)

 protected:
  ~HnsWalletGetProjectIDFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletGetHnsKeyFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.getHnsKey", UNKNOWN)

 protected:
  ~HnsWalletGetHnsKeyFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletResetWalletFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.resetWallet", UNKNOWN)

 protected:
  ~HnsWalletResetWalletFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletGetWeb3ProviderFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.getWeb3Provider", UNKNOWN)

 protected:
  ~HnsWalletGetWeb3ProviderFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletGetWeb3ProviderListFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.getWeb3ProviderList", UNKNOWN)

 protected:
  ~HnsWalletGetWeb3ProviderListFunction() override {}
  ResponseAction Run() override;
};

class HnsWalletIsNativeWalletEnabledFunction : public ExtensionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("hnsWallet.isNativeWalletEnabled", UNKNOWN)

 protected:
  ~HnsWalletIsNativeWalletEnabledFunction() override {}
  ResponseAction Run() override;
};

}  // namespace api
}  // namespace extensions

#endif  // HNS_BROWSER_EXTENSIONS_API_HNS_WALLET_API_H_
