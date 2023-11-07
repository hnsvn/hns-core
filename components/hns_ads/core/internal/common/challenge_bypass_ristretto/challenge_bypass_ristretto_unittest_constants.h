/* Copyright (c) 2022 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_CHALLENGE_BYPASS_RISTRETTO_UNITTEST_CONSTANTS_H_
#define HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_CHALLENGE_BYPASS_RISTRETTO_UNITTEST_CONSTANTS_H_

namespace hns_ads::cbr {

constexpr char kInvalidBase64[] = "INVALID";

constexpr char kSigningKeyBase64[] =
    R"(qsA+Hif/fQ3fNLMa37qbCLJjlzauLaqbjFNff/PkFAY=)";

constexpr char kPublicKeyBase64[] =
    R"(QnShwT9vRebch3WDu28nqlTaNCU5MaOF1n4VV4Q3K1g=)";

constexpr char kTokenBase64[] =
    R"(IXDCnZnVEJ0orkbZfr2ut2NQPQ0ofdervKBmQ2hyjcClGCjA3/ExbBumO0ua5cxwo//nN0uKQ60kknru8hRXx0FMYHu7ahizDiX18HVBawWBzA46pyrBtJDfiomK6HQI)";

constexpr char kBlindedTokenBase64[] =
    R"(mNRViqFD8ZPpjRZi4Xwj1UEsU1j9qPNc4R/BoiWsVi0=)";

constexpr char kSignedTokenBase64[] =
    R"(jGZR7JREp+zoqxgsMOa32F+zhhBw/0d/HeVhl9iPVWU=)";

constexpr char kDLEQProofBase64[] =
    R"(8vp0QItdO24oqOZB8m8rCB85VUftBhnpZ8kDovYP9AvvlaEpwDFbTi72B1ZEJmumS5TazlWlLBlI4HrWDCMvDg==)";

constexpr char kBatchDLEQProofBase64[] =
    R"(y0a409PTX6g97xC0Xq8cCuY7ElLPaP+QJ6DaHNfqlQWizBYCSWdaleakKatkyNswfPmkQuhL7awmzQ0ygEUGDw==)";

constexpr char kUnblindedTokenBase64[] =
    R"(IXDCnZnVEJ0orkbZfr2ut2NQPQ0ofdervKBmQ2hyjcClGCjA3/ExbBumO0ua5cxwo//nN0uKQ60kknru8hRXx0DWhwHwuFlxmot8WgVbnQ0XtPx7q9BG0jbI00AJStwN)";

constexpr char kVerificationSignatureBase64[] =
    R"(V7Gilxl5TNv7pTqq8Sftmu+O+HgJ44Byn8PhDkDIwnsgncGiCduuoRMNagUnN7AXalaQdy1GedKj5thKFeyUcQ==)";

constexpr char kTokenPreimageBase64[] =
    R"(IXDCnZnVEJ0orkbZfr2ut2NQPQ0ofdervKBmQ2hyjcClGCjA3/ExbBumO0ua5cxwo//nN0uKQ60kknru8hRXxw==)";

}  // namespace hns_ads::cbr

#endif  // HNS_COMPONENTS_HNS_ADS_CORE_INTERNAL_COMMON_CHALLENGE_BYPASS_RISTRETTO_CHALLENGE_BYPASS_RISTRETTO_UNITTEST_CONSTANTS_H_
