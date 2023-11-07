/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "base/files/file_util.h"
#include "base/path_service.h"
#include "base/test/thread_test_helper.h"
#include "hns/browser/hns_browser_process.h"
#include "hns/browser/ipfs/ipfs_service_factory.h"
#include "hns/components/constants/hns_paths.h"
#include "hns/components/ipfs/hns_ipfs_client_updater.h"
#include "hns/components/ipfs/ipfs_service.h"
#include "build/build_config.h"
#include "chrome/browser/extensions/extension_browsertest.h"
#include "chrome/test/base/ui_test_utils.h"
#include "content/public/test/browser_test.h"

using extensions::ExtensionBrowserTest;

static const char kIpfsClientUpdaterComponentTestId[] =
    "ngicbhhaldfdgmjhilmnleppfpmkgbbk";
static const char kIpfsClientUpdaterComponentTestBase64PublicKey[] =
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAncFKJWCX6RqCRYOR0t5a"
    "js7HWIVP3Ne272HZs3MqiaNvo9IikbPd4JnUMeQjLhkXTwxg6Up9Tmrgo3M8T91D"
    "dggzpAG4OlhKj3l3N5kZnj/CxQ73YVd41jHAF97lZVoD5VTCGtEelzA5eHI4N4Hd"
    "cvMiMvr/Kj9pdlJ+kbg5UZIXAYLXUB/NfBjKlpCTZ+Ys/2nxRN27kUVnrE/gTitE"
    "Aj1PZGOxJd1ZeiYc29j0ETf3AmOsZyVrIs6HJzHEJLnYQFaa76dRwVabm1Zt/28T"
    "+NJdHcu+jj2LIEcxmZ8TjtbK9kfWORHhA/ELjTx4ScvKfVKJgdLpxy5QOBFFnTLR"
    "QQIDAQAB";

class HnsIpfsClientUpdaterTest : public ExtensionBrowserTest {
 public:
  HnsIpfsClientUpdaterTest() = default;

  void SetUp() override {
    InitEmbeddedTestServer();
    ExtensionBrowserTest::SetUp();
  }

  void InitEmbeddedTestServer() {
    hns::RegisterPathProvider();
    base::FilePath test_data_dir;
    base::PathService::Get(hns::DIR_TEST_DATA, &test_data_dir);
    embedded_test_server()->ServeFilesFromDirectory(test_data_dir);
    ASSERT_TRUE(embedded_test_server()->Start());
  }

  void GetTestDataDir(base::FilePath* test_data_dir) {
    base::ScopedAllowBlockingForTesting allow_blocking;
    base::PathService::Get(hns::DIR_TEST_DATA, test_data_dir);
  }

  bool PathExists(const base::FilePath& file_path) {
    base::ScopedAllowBlockingForTesting allow_blocking;
    return base::PathExists(file_path);
  }

  void SetComponentIdAndBase64PublicKeyForTest(
      const std::string& component_id,
      const std::string& component_base64_public_key) {
    ipfs::HnsIpfsClientUpdater::SetComponentIdAndBase64PublicKeyForTest(
        component_id, component_base64_public_key);
  }

  bool InstallIpfsClientUpdater() {
#if BUILDFLAG(IS_WIN)
    return InstallIpfsClientUpdater("ipfs-client-updater-win");
#elif BUILDFLAG(IS_MAC)
    return InstallIpfsClientUpdater("ipfs-client-updater-mac");
#elif BUILDFLAG(IS_LINUX)
    return InstallIpfsClientUpdater("ipfs-client-updater-linux");
#else
    return false;
#endif
  }

  bool InstallIpfsClientUpdater(const std::string& extension_dir) {
    base::FilePath test_data_dir;
    GetTestDataDir(&test_data_dir);
    const extensions::Extension* ipfs_client_updater =
        InstallExtension(test_data_dir.AppendASCII("ipfs-client-updater")
                             .AppendASCII(extension_dir),
                         1);
    if (!ipfs_client_updater)
      return false;

    g_hns_browser_process->ipfs_client_updater()->OnComponentReady(
        ipfs_client_updater->id(), ipfs_client_updater->path(), "");
    WaitForIpfsClientUpdaterThread();
    WaitForMainThreadTasksToFinish();
    return true;
  }

  void WaitForIpfsClientUpdaterThread() {
    scoped_refptr<base::ThreadTestHelper> io_helper(new base::ThreadTestHelper(
        g_hns_browser_process->ipfs_client_updater()->GetTaskRunner()));
    ASSERT_TRUE(io_helper->Run());
  }

  void WaitForMainThreadTasksToFinish() {
    base::RunLoop loop;
    loop.RunUntilIdle();
  }

  void SetIpfsExecutablePath(const base::FilePath& path) {
    g_hns_browser_process->ipfs_client_updater()->SetExecutablePath(path);
  }
};

// Load the Ipfs client updater extension and verify that it correctly
// installs the client.
IN_PROC_BROWSER_TEST_F(HnsIpfsClientUpdaterTest, IpfsClientInstalls) {
  SetComponentIdAndBase64PublicKeyForTest(
      kIpfsClientUpdaterComponentTestId,
      kIpfsClientUpdaterComponentTestBase64PublicKey);
  ASSERT_TRUE(InstallIpfsClientUpdater());
  base::FilePath executable_path =
      g_hns_browser_process->ipfs_client_updater()->GetExecutablePath();
  ASSERT_TRUE(PathExists(executable_path));
}

IN_PROC_BROWSER_TEST_F(HnsIpfsClientUpdaterTest, IpfsExecutableReady) {
  ipfs::IpfsService* ipfs_service =
      ipfs::IpfsServiceFactory::GetInstance()->GetForContext(profile());
  ASSERT_TRUE(ipfs_service);
  ASSERT_FALSE(ipfs_service->IsIPFSExecutableAvailable());
  ASSERT_TRUE(ipfs_service->GetIpfsExecutablePath().empty());
  SetComponentIdAndBase64PublicKeyForTest(
      kIpfsClientUpdaterComponentTestId,
      kIpfsClientUpdaterComponentTestBase64PublicKey);
  ASSERT_TRUE(InstallIpfsClientUpdater());
  base::FilePath executable_path =
      g_hns_browser_process->ipfs_client_updater()->GetExecutablePath();
  ASSERT_TRUE(PathExists(executable_path));

  EXPECT_EQ(ipfs_service->GetIpfsExecutablePath(), executable_path);
  ASSERT_TRUE(ipfs_service->IsIPFSExecutableAvailable());

  base::FilePath new_path(FILE_PATH_LITERAL("newpath"));
  SetIpfsExecutablePath(new_path);
  EXPECT_EQ(ipfs_service->GetIpfsExecutablePath(), new_path);
  ASSERT_TRUE(ipfs_service->IsIPFSExecutableAvailable());
}
