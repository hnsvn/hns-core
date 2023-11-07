/* Copyright (c) 2021 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef HNS_COMPONENTS_DECENTRALIZED_DNS_CORE_UTILS_H_
#define HNS_COMPONENTS_DECENTRALIZED_DNS_CORE_UTILS_H_

#include "base/strings/string_piece.h"
#include "hns/components/decentralized_dns/core/constants.h"

class GURL;
class PrefService;
class PrefRegistrySimple;

namespace decentralized_dns {

void RegisterLocalStatePrefs(PrefRegistrySimple* registry);
void MigrateObsoleteLocalStatePrefs(PrefService* local_state);

bool IsUnstoppableDomainsTLD(const base::StringPiece& host);
void SetUnstoppableDomainsResolveMethod(PrefService* local_state,
                                        ResolveMethodTypes method);
ResolveMethodTypes GetUnstoppableDomainsResolveMethod(PrefService* local_state);
bool IsUnstoppableDomainsResolveMethodAsk(PrefService* local_state);
bool IsUnstoppableDomainsResolveMethodEnabled(PrefService* local_state);

bool IsENSTLD(const base::StringPiece& host);
void SetENSResolveMethod(PrefService* local_state, ResolveMethodTypes method);
ResolveMethodTypes GetENSResolveMethod(PrefService* local_state);
bool IsENSResolveMethodAsk(PrefService* local_state);
bool IsENSResolveMethodEnabled(PrefService* local_state);

void SetEnsOffchainResolveMethod(PrefService* local_state,
                                 EnsOffchainResolveMethod method);
EnsOffchainResolveMethod GetEnsOffchainResolveMethod(PrefService* local_state);

bool IsSnsTLD(const base::StringPiece& host);
void SetSnsResolveMethod(PrefService* local_state, ResolveMethodTypes method);
ResolveMethodTypes GetSnsResolveMethod(PrefService* local_state);
bool IsSnsResolveMethodAsk(PrefService* local_state);
bool IsSnsResolveMethodEnabled(PrefService* local_state);

}  // namespace decentralized_dns

#endif  // HNS_COMPONENTS_DECENTRALIZED_DNS_CORE_UTILS_H_
