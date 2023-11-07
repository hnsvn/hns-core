/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/components/hns_vpn/renderer/android/vpn_render_frame_observer.h"

#include "base/test/scoped_feature_list.h"
#include "hns/components/hns_vpn/common/features.h"
#include "hns/components/skus/common/features.h"
#include "content/public/common/isolated_world_ids.h"
#include "content/public/test/content_mock_cert_verifier.h"
#include "content/public/test/render_view_test.h"
#include "url/gurl.h"

namespace hns_vpn {

class VpnRenderFrameObserverBrowserTest : public content::RenderViewTest {
 public:
  VpnRenderFrameObserverBrowserTest() {
    scoped_feature_list_.InitWithFeatures(
        {skus::features::kSkusFeature, hns_vpn::features::kHnsVPN}, {});
  }
  ~VpnRenderFrameObserverBrowserTest() override = default;

 private:
  base::test::ScopedFeatureList scoped_feature_list_;
};

TEST_F(VpnRenderFrameObserverBrowserTest, IsAllowed) {
  VpnRenderFrameObserver observer(GetMainRenderFrame(),
                                  content::ISOLATED_WORLD_ID_GLOBAL);
  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "https://account.hns.com/?intent=connect-receipt&product=vpn");

  EXPECT_TRUE(observer.IsAllowed());
  // http
  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "http://account.hns.com/?intent=connect-receipt&product=vpn");

  EXPECT_FALSE(observer.IsAllowed());

  // https://account.hnssoftware.com
  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "https://account.hnssoftware.com/?intent=connect-receipt&product=vpn");

  EXPECT_TRUE(observer.IsAllowed());

  // https://account.hns.software
  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "https://account.hns.software/?intent=connect-receipt&product=vpn");

  EXPECT_TRUE(observer.IsAllowed());

  // no recepit
  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "https://account.hns.software/?intent=&product=vpn");

  EXPECT_FALSE(observer.IsAllowed());

  // wrong recepit
  LoadHTMLWithUrlOverride(R"(<html><body></body></html>)",
                          "https://account.hns.software/?product=vpn");

  EXPECT_FALSE(observer.IsAllowed());

  // wrong recepit
  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "https://account.hns.software/?intent=wrong&product=vpn");

  EXPECT_FALSE(observer.IsAllowed());

  // no product
  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "https://account.hns.software/?intent=connect-receipt&product=");

  EXPECT_FALSE(observer.IsAllowed());

  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "https://account.hns.software/?intent=connect-receipt");

  EXPECT_FALSE(observer.IsAllowed());

  // wrong product
  LoadHTMLWithUrlOverride(
      R"(<html><body></body></html>)",
      "https://account.hns.software/?intent=connect-receipt&product=wrong");

  EXPECT_FALSE(observer.IsAllowed());
}

}  // namespace hns_vpn
