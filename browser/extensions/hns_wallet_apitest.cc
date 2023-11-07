/* Copyright (c) 2019 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "base/environment.h"
#include "base/path_service.h"
#include "hns/browser/ethereum_remote_client/ethereum_remote_client_constants.h"
#include "hns/browser/ethereum_remote_client/pref_names.h"
#include "hns/components/hns_wallet/browser/hns_wallet_utils.h"
#include "hns/components/hns_wallet/browser/pref_names.h"
#include "hns/components/hns_wallet/common/hns_wallet.mojom.h"
#include "hns/components/constants/hns_paths.h"
#include "hns/components/constants/pref_names.h"
#include "chrome/browser/extensions/extension_apitest.h"
#include "chrome/browser/extensions/extension_service.h"
#include "chrome/test/base/ui_test_utils.h"
#include "components/prefs/pref_service.h"
#include "content/public/test/browser_test.h"
#include "extensions/common/constants.h"
#include "extensions/test/result_catcher.h"

namespace extensions {
namespace {

class HnsWalletExtensionApiTest : public ExtensionApiTest {
 public:
  void SetUp() override {
    hns::RegisterPathProvider();
    base::PathService::Get(hns::DIR_TEST_DATA, &extension_dir_);
    extension_dir_ = extension_dir_.AppendASCII("extensions/api_test");
    ExtensionApiTest::SetUp();
  }
  void TearDown() override { ExtensionApiTest::TearDown(); }
  PrefService* GetPrefs() { return browser()->profile()->GetPrefs(); }
  base::FilePath extension_dir_;
};

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       HnsExtensionWithWalletHasAccess) {
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsShieldsWithWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), hns_extension_id, "testBasics()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest, HnsWalletAPIAvailable) {
  std::unique_ptr<base::Environment> env(base::Environment::Create());
  env->SetVar("HNS_INFURA_PROJECT_ID", "test-project-id");
  env->SetVar("HNS_SERVICES_KEY", "test-hns-key");
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testBasics()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       HnsWalletAPIKnownValuesTest) {
  GetPrefs()->SetString(kERCAES256GCMSivNonce, "yJngKDr5nCGYz7EM");
  GetPrefs()->SetString(
      kERCEncryptedSeed,
      "IQu5fUMbXG6E7v8ITwcIKL3TI3rst0LU1US7ZxCKpgAGgLNAN6DbCN7nMF2Eg7Kx");
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testKnownSeedValuesEndToEnd()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       HnsWalletWeb3ProviderCryptoWallets) {
  hns_wallet::SetDefaultEthereumWallet(
      GetPrefs(), hns_wallet::mojom::DefaultWallet::CryptoWallets);
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsCryptoWallets()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       HnsWalletWeb3ProviderIsHnsWalletPreferExtension) {
  hns_wallet::SetDefaultEthereumWallet(
      GetPrefs(),
      hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension);
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsHnsWalletPreferExtension()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       HnsWalletWeb3ProviderNone) {
  hns_wallet::SetDefaultEthereumWallet(
      GetPrefs(), hns_wallet::mojom::DefaultWallet::None);
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsNone()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       HnsWalletWeb3ProviderHnsWallet) {
  hns_wallet::SetDefaultEthereumWallet(
      GetPrefs(), hns_wallet::mojom::DefaultWallet::HnsWallet);
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(browsertest_util::ExecuteScriptInBackgroundPageNoWait(
      browser()->profile(), ethereum_remote_client_extension_id,
      "testProviderIsHnsWallet()"));
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       HnsWalletAPINotAvailable) {
  ResultCatcher catcher;
  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("notHnsWallet"));
  ASSERT_TRUE(extension);
  ASSERT_TRUE(catcher.GetNextResult()) << message_;
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       ChangeTypeCryptoWalletsToHnsWallet) {
  hns_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      hns_wallet::mojom::DefaultWallet::CryptoWallets);
  ASSERT_TRUE(
      ui_test_utils::NavigateToURL(browser(), GURL("hns://settings/")));

  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsWallet"));
  ASSERT_TRUE(extension);
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(browser()->profile())
          ->extension_service();
  ASSERT_TRUE(service->IsExtensionEnabled(ethereum_remote_client_extension_id));
  hns_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      hns_wallet::mojom::DefaultWallet::HnsWallet);
  ASSERT_FALSE(
      service->IsExtensionEnabled(ethereum_remote_client_extension_id));
}

IN_PROC_BROWSER_TEST_F(HnsWalletExtensionApiTest,
                       ChangeTypeCryptoWalletsToHnsWalletPreferExtension) {
  hns_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      hns_wallet::mojom::DefaultWallet::CryptoWallets);
  ASSERT_TRUE(
      ui_test_utils::NavigateToURL(browser(), GURL("hns://settings/")));

  const Extension* extension =
      LoadExtension(extension_dir_.AppendASCII("hnsWallet"));
  ASSERT_TRUE(extension);
  extensions::ExtensionService* service =
      extensions::ExtensionSystem::Get(browser()->profile())
          ->extension_service();
  ASSERT_TRUE(service->IsExtensionEnabled(ethereum_remote_client_extension_id));
  hns_wallet::SetDefaultEthereumWallet(
      browser()->profile()->GetPrefs(),
      hns_wallet::mojom::DefaultWallet::HnsWalletPreferExtension);
  ASSERT_FALSE(
      service->IsExtensionEnabled(ethereum_remote_client_extension_id));
}

}  // namespace
}  // namespace extensions
