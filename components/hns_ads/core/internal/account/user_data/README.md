# User Data

**Optional** user data may not be included in the confirmation payload.

**Mutable** user data can change when a confirmation redemption fails and is retried later.

## User Data Submitted when Redeeming Anonymous Confirmations

Included in the confirmation payload for non-Hns Rewards and Hns Rewards users. See [anonymous confirmation redemption](../tokens/redeem_confirmation/README.md).

| user data  | optional  | mutable  | description  |
|---|---|---|---|---|
| conversion  | yes  | no  | Conversion action and encrypted verifiable envelope.  |

### Additional User Data Submitted for Hns Rewards Users

Should only be included in the confirmation payload for users who have joined Hns Rewards.

| user data  | optional  | mutable  | description  |
|---|---|---|---|---|
| buildChannel  | no  | no  | Browser build channel.  |
| catalog  | no  | no  |  Catalog identifier.  |
| createdAtTimestamp  | no  | no  | Privacy-preserving [ISO 8601](https://en.wikipedia.org/wiki/ISO_8601) timestamp.  |
| diagnosticId  | yes  | yes  | Diagnostic id from [hns://rewards-internals](hns://rewards-internals) to help diagnose issues.  |
| locale  | yes  | no  | Privacy-preserving operating system locale  |
| platform  | no  | no  | Operating system.  |
| rotating_hash  | yes  | no  | Privacy-preserving time-based rotating hash.  |
| segment  | yes  | no  | Advertising segment taxonomy for the chosen ad.  |
| studies  | no  | no  | User studies. See [Griffin](https://github.com/hnsvn/hns-browser/wiki/Hns-Variations-(Griffin)).  |
| systemTimestamp  | no  | yes  | Privacy-preserving [ISO 8601](https://en.wikipedia.org/wiki/ISO_8601) timestamp.  |
| versionNumber  | no  | no  | Browser version number.  |

## User Data Submitted when Redeeming Identifiable Payment Tokens

See [identifiable payment token redemption](../tokens/redeem_payment_tokens/README.md).

| user data  | optional  | mutable  | description  |
|---|---|---|---|
| platform  | no  | no  | Operating system.  |
| summary  | no  | no  | Summary of confirmation tokens for each creative.  |

Please add to it!
