/* Copyright (c) 2020 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "hns/components/ipfs/pref_names.h"

// Used to determine which method should be used to resolve ipfs:// and ipns:///
// schemes, between:
// Ask: Uses a gateway but also prompts the user with an infobar.
// Gateway: Uses a gateway without prompting the user.
// Local: Uses a local node.
// Disabled: Disables all IPFS handling.
const char kIPFSResolveMethod[] = "hns.ipfs.resolve_method";
// Stores the location of the IPFS binary
const char kIPFSBinaryPath[] = "hns.ipfs.binary_path";

// Used to determine whether to automatically fallback to gateway when the
// local node is not available.
const char kIPFSAutoFallbackToGateway[] = "hns.ipfs.auto_fallback_to_gateway";

// Deprecated, use kIPFSAutoRedirectToConfiguredGateway instead
// Used to automatically redirect Gateway resources with x-ipfs-path
// header to the configured Hns IPFS gateway.
const char kIPFSAutoRedirectGateway[] = "hns.ipfs.auto_redirect_gateway";

// Deprecated, use kIPFSAutoRedirectToConfiguredGateway instead
// Used to automatically redirect for DNSLink resources
const char kIPFSAutoRedirectDNSLink[] = "hns.ipfs.auto_redirect_dnslink";

// This is a new setting which merges kIPFSAutoRedirectGateway and
// kIPFSAutoRedirectDNSLink
const char kIPFSAutoRedirectToConfiguredGateway[] =
    "hns.ipfs.auto_redirect_to_configured_gateway";

// The number of times the infobar is shown to ask the user to install IPFS
const char kIPFSInfobarCount[] = "hns.ipfs.infobar_count";

// The number of storage used by IPFS Node
const char kIpfsStorageMax[] = "hns.ipfs.storage_max";

// Used to enable/disable IPFS via admin policy.
const char kIPFSEnabled[] = "hns.ipfs.enabled";

// Used to determine if local node was ever used.
const char kIPFSLocalNodeUsed[] = "hns.ipfs.local_node_used";

// Stores IPFS public gateway address to be used when translating IPFS URLs.
const char kIPFSPublicGatewayAddress[] = "hns.ipfs.public_gateway_address";

// Stores IPFS public gateway address to be used when translating IPFS NFT URLs.
const char kIPFSPublicNFTGatewayAddress[] =
    "hns.ipfs.public_nft_gateway_address";

// Stores list of CIDs that are pinned localy
const char kIPFSPinnedCids[] = "hns.ipfs.local_pinned_cids";

// Stores info whether IPFS promo infobar was shown yet
const char kShowIPFSPromoInfobar[] = "hns.ipfs.show_ipfs_promo_infobar";
