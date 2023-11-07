// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import { provideStrings } from '../../../.storybook/locale'

provideStrings({
  // App Categories
  hnsWalletDefiCategory: 'Defi apps',
  hnsWalletNftCategory: 'NFT marketplaces',
  hnsWalletSearchCategory: 'Search results',

  // App Category Button Text
  hnsWalletDefiButtonText: 'Browse more Defi',
  hnsWalletNftButtonText: 'Browse more NFT',

  // Compound App
  hnsWalletCompoundName: 'Compound',
  hnsWalletCompoundDescription: 'Unlock a universe of open financial applications.',

  // Maker App
  hnsWalletMakerName: 'MakerDAO',
  hnsWalletMakerDescription: 'Maker - stablecoin, loans and governance...',

  // Aave App
  hnsWalletAaveName: 'Aave',
  hnsWalletAaveDescription: 'Protocol to earn on deposits & borrow assets.',

  // OpenSea App
  hnsWalletOpenSeaName: 'OpenSea',
  hnsWalletOpenSeaDescription: 'The largest NFT marketplace. Buy, sell, and discover rare digital items',

  // Rarible App
  hnsWalletRaribleName: 'Rarible',
  hnsWalletRaribleDescription: 'Create and sell digital artworks',

  // Search Text
  hnsWalletSearchText: 'Search',

  // Side Nav Buttons
  hnsWalletSideNavCrypto: 'Crypto',
  hnsWalletSideNavRewards: 'Rewards',
  hnsWalletSideNavCards: 'Cards',

  // Top Nav Tab Buttons
  hnsWalletTopNavPortfolio: 'Portfolio',
  hnsWalletTopTabPrices: 'Prices',
  hnsWalletTopTabApps: 'Apps',
  hnsWalletTopNavNFTS: 'NFTs',
  hnsWalletTopNavAccounts: 'Accounts',
  hnsWalletTopNavMarket: 'Market',

  // Chart Timeline Buttons
  hnsWalletChartOneHour: '1 Hour',
  hnsWalletChartOneDay: '1 Day',
  hnsWalletChartOneWeek: '1 Week',
  hnsWalletChartOneMonth: '1 Month',
  hnsWalletChartThreeMonths: '3 Months',
  hnsWalletChartOneYear: '1 Year',
  hnsWalletChartAllTime: 'All time',
  hnsWalletChartOneHourAbbr: '1H',
  hnsWalletChartOneDayAbbr: '1D',
  hnsWalletChartOneWeekAbbr: '1W',
  hnsWalletChartOneMonthAbbr: '1M',
  hnsWalletChartThreeMonthsAbbr: '3M',
  hnsWalletChartOneYearAbbr: '1Y',
  hnsWalletChartAllTimeAbbr: 'All',

  // Portfolio View
  hnsWalletAddCoin: 'Add Coin',
  hnsWalletBalance: 'Balance',
  hnsWalletPortfolioAssetNetworkDescription: '$1 on $2',

  // Portfolio SubView
  hnsWalletAccounts: 'Accounts',
  hnsWalletAccount: 'Account',
  hnsWalletOwner: 'Owner',
  hnsWalletActivity: 'Activity',
  hnsWalletTransactions: 'Transactions',
  hnsWalletPrice: 'Price',
  hnsWalletBack: 'Back',
  hnsWalletAddAccount: 'Add Account',
  hnsWalletPoweredByCoinGecko: 'Price data powered by CoinGecko',

  // Actions
  hnsWalletClickToSwitch: 'Click to switch',
  hnsWalletEnterYourPassword: 'Enter your password',
  hnsWalletEnterAPassswordToContinue: 'Enter a password to continue',
  hnsWalletEnterYourPasswordToStartBackup: 'Enter your Hns Wallet password to start backing up wallet.',

  // BuySendSwap
  hnsWalletBuy: 'Buy',
  hnsWalletSend: 'Send',
  hnsWalletSwap: 'Swap',
  hnsWalletReset: 'Reset',
  hnsWalletSell: 'Sell',
  hnsWalletNotEnoughBalance: 'You don’t have enough $1 in this account.',
  hnsWalletBuyNotSupportedTooltip: 'Buy not supported',
  hnsWalletSwapNotSupportedTooltip: 'Swap not supported',
  hnsWalletSlippageToleranceWarning: 'Transaction may be frontrun',
  hnsWalletSlippageToleranceTitle: 'Slippage tolerance',
  hnsWalletExpiresInTitle: 'Expires in',
  hnsWalletSendPlaceholder: 'Wallet address or URL',
  hnsWalletSendNoURLPlaceholder: 'Wallet address',
  hnsWalletSwapDisclaimer: 'Hns uses $1$3$2 as a DEX aggregator.',
  hnsWalletSwapDisclaimerDescription: '0x will process the Ethereum address and IP address to fulfill a transaction (including getting quotes). 0x will ONLY use this data for the purposes of processing transactions.',
  hnsWalletJupiterSwapDisclaimerDescription: 'Jupiter will process the Solana address and IP address to fulfill a transaction (including getting quotes). Jupiter will ONLY use this data for the purposes of processing transactions.',
  hnsWalletSwapFeesNotice: 'Quote includes a $1 Hns fee.',
  hnsWalletDecimalPlacesError: 'Too many decimal places',
  hnsWalletBuyTapBuyNotSupportedMessage: 'Buy not supported for selected network',
  hnsWalletSearchingForDomain: 'Searching for domain...',
  hnsWalletEnsOffChainLookupTitle: 'Hns supports using off-chain gateways to resolve .eth domains.',
  hnsWalletEnsOffChainLookupDescription: 'It looks like you\'ve entered an ENS address. We\'ll need to use a third-party resolver to resolve this request, which may be able to see your IP address and domain.',
  hnsWalletEnsOffChainButton: 'Use ENS domain',
  hnsWalletFEVMAddressTranslationTitle: 'ETH address will be converted to the Filecoin address.',
  hnsWalletFEVMAddressTranslationDescription: 'It looks like you\'ve entered an ENS address. We\'ll need to use a third-party resolver to resolve this request, which may be able to see your IP address and domain.',

  // Send Tab
  hnsWalletSendToken: 'Send token',
  hnsWalletSendNFT: 'Send NFT',
  hnsWalletSelectToken: 'Select token',
  hnsWalletSelectNFT: 'Select NFT',
  hnsWalletSendTabSelectTokenTitle: 'Select a token to send',
  hnsWalletSendTabSelectNFTTitle: 'Select an NFT to send',
  hnsWalletEnterRecipientAddress: 'Enter recipient address',
  hnsWalletNotEnoughFunds: 'Not enough funds',
  hnsWalletSendHalf: 'HALF',
  hnsWalletSendMax: 'MAX',
  hnsWalletReviewOrder: 'Review order',
  hnsWalletReviewSend: 'Review send',
  hnsWalletNoAvailableTokens: 'No available tokens',
  hnsWalletSearchTokens: 'Search token by name',
  hnsWalletSearchNFTs: 'Search NFT by name, id',

  // Create Account Tab
  hnsWalletUnlockNeededToCreateAccount: 'Unlock needed to create an account',
  hnsWalletCreateAccountDescription: 'You don’t yet have a $1 account. Create one now?',
  hnsWalletCreateAccountYes: 'Yes',
  hnsWalletCreateAccountNo: 'No',

  // Buttons
  hnsWalletButtonContinue: 'Continue',
  hnsWalletButtonNext: 'Next',
  hnsWalletButtonGotIt: 'Got it',
  hnsWalletButtonCopy: 'Copy',
  hnsWalletButtonCopied: 'Copied!',
  hnsWalletButtonVerify: 'Verify',
  hnsWalletButtonClose: 'Close',
  hnsWalletButtonMore: 'More',
  hnsWalletButtonDone: 'Done',
  hnsWalletButtonSkip: 'Skip',
  hnsWalletButtonCancel: 'Cancel',
  hnsWalletButtonSaveChanges: 'Save changes',
  hnsWalletLearnMore: 'Learn more',

  // Onboarding - Welcome
  hnsWalletWelcomeTitle: 'Secure. Multi-chain. And oh-so-easy to use. Your Hns Wallet is just a few clicks away',
  hnsWalletWelcomeButton: 'Create new wallet',
  hnsWalletLearnMoreAboutHnsWallet: 'Learn more about Hns Wallet',
  hnsWalletImportExistingWallet: 'Import existing wallet',
  hnsWalletWelcomeRestoreButton: 'Restore',
  hnsWalletConnectHardwareWallet: 'Connect hardware wallet',
  hnsWalletWelcomeDividerText: 'or',

  // Onboarding - Disclosures
  hnsWalletDisclosuresTitle: 'Legal stuff',
  hnsWalletDisclosuresDescription: 'Please acknowledge the following:',
  hnsWalletSelfCustodyDisclosureCheckboxText: 'I understand this is a self-custody wallet, and that I alone am responsible for any associated funds, assets, or accounts, and for taking appropriate action to secure, protect and backup my wallet. I understand that Hns can NOT access my wallet or reverse transactions on my behalf, and that my recovery phrase is the ONLY way to regain access in the event of a lost password, stolen device, or similar circumstance.',
  hnsWalletTermsOfServiceCheckboxText: 'I have read and agree to the $1Terms of use$2',

  // Onboarding import or restore wallet page
  hnsWalletCheckingInstalledExtensions: 'Checking for wallet extensions...',
  hnsWalletImportOrRestoreWalletTitle: 'Connect to your existing wallet',
  hnsWalletImportOrRestoreDescription: 'To connect a wallet you already have, you may need to enter your recovery phrase. At this time we support restoring / importing from Ethereum and Solana wallets.',
  hnsWalletRestoreMyHnsWallet: 'Restore from seed phrase',
  hnsWalletRestoreMyHnsWalletDescription: '12-24 words',
  hnsWalletImportFromMetaMask: 'Import from MetaMask',
  hnsWalletImportFromMetaMaskDescription: 'Use your MetaMask password to import your seed phrase',
  hnsWalletImportFromLegacy: 'Import from legacy Hns crypto wallets',
  hnsWalletCreateWalletInsteadLink: 'Never mind, I’ll create a new wallet',

  // onboarding import wallet screen
  hnsWalletImportPasswordError: 'Password is not correct',
  hnsWalletMetaMaskPasswordInputPlaceholder: 'Type MetaMask password',
  hnsWalletImportFromMetaMaskSeedInstructions: 'Type your MetaMask 12-24 word recovery phrase.',
  hnsWalletMetaMaskExtensionDetected: 'We detected the MetaMask extension in your browser',
  hnsWalletMetaMaskExtensionImportDescription: 'Enter your MetaMask wallet password to easily import to Hns Wallet.',
  hnsWalletRestoreMyHnsWalletInstructions: 'Type your Hns Wallet 12-24 word recovery phrase.',
  hnsWalletRecoveryPhraseLengthError: 'Recovery phrase must be 12, 15, 18, 21, or 24 words long',
  hnsWalletInvalidMnemonicError: 'The mnemonic being imported is not valid for Hns Wallet',

  // Onboarding - Backup Wallet - Intro
  hnsWalletOnboardingRecoveryPhraseBackupIntroTitle: 'Before you start backing up wallet',
  hnsWalletOnboardingRecoveryPhraseBackupIntroDescription: 'The 12-24 word recovery phrase is a private key you can use to regain access to your wallet in case you lose a connected device(s). Store it someplace safe, and in the exact order it appears below.',
  hnsWalletRecoveryPhraseBackupWarningImportant: '$1Important:$2 Never share your recovery phrase. Anyone with this phrase can take your assets forever.',

  // Onboarding - Backup Wallet - Recovery Phrase Backup
  hnsWalletRecoveryPhraseBackupTitle: 'Back up your wallet recovery phrase',
  hnsWalletRecoveryPhraseBackupWarning: 'Hns cannot access your secret recovery phrase. Keep it safe, and never share it with anyone else.',
  hnsWalletCopiedToClipboard: 'Copied to clipboard',
  hnsWalletClickToSeeRecoveryPhrase: 'Click to see your phrase',

  // Onboarding - Backup Wallet - Verify Recovery Phrase
  hnsWalletVerifyRecoveryPhraseTitle: 'Verify your recovery phrase',
  hnsWalletVerifyRecoveryPhraseInstructions: 'Click the $1$7 ($8)$2, $3$9 ($10)$4, and $5$11 ($12)$6 words of your recovery phrase.',
  hnsWalletVerifyPhraseError: 'Recovery phrase didn\'t match',

  // Recovery Phrase Backup - Intro
  hnsWalletBackupIntroTitle: 'Back up your crypto wallet',
  hnsWalletBackupIntroDescription: 'In the next step you’ll see a $1-word recovery phrase, which you can use to recover your primary crypto accounts. Save it someplace safe. Your recovery phrase is the only way to regain account access in case of forgotten password, lost or stolen device, or you want to switch wallets.',
  hnsWalletBackupIntroTerms: 'I understand that if I lose my recovery words, I will not be able to access my crypto wallet.',

  // Recovery Phrase Backup - Intro
  hnsWalletRecoveryTitle: 'Your recovery phrase',
  hnsWalletRecoveryDescription: 'Write down or copy these words in the exact order shown below, and save them somewhere safe. Your recovery phrase is the only way to regain account access in case of forgotten password, lost or stolen device, or you want to switch wallets.',
  hnsWalletRecoveryWarning1: 'WARNING:',
  hnsWalletRecoveryWarning2: 'Never share your recovery phrase.',
  hnsWalletRecoveryWarning3: 'Anyone with this phrase can take your assets forever.',
  hnsWalletRecoveryTerms: 'I have backed up my phrase somewhere safe.',

  // Recovery Phrase Backup - Verify Recovery Phrase
  hnsWalletVerifyRecoveryTitle: 'Verify recovery phrase',
  hnsWalletVerifyRecoveryDescription: 'Select the words in your recovery phrase in their correct order.',
  hnsWalletVerifyError: 'Recovery phrase did not match, please try again.',

  // Create Password
  hnsWalletCreatePasswordTitle: 'Create a new password',
  hnsWalletCreatePasswordDescription: 'You\'ll use this password each time you access your wallet.',
  hnsWalletCreatePasswordInput: 'Enter new password',
  hnsWalletConfirmPasswordInput: 'Re-enter password',
  hnsWalletCreatePasswordError: 'Password criteria doesn\'t match.',
  hnsWalletConfirmPasswordError: 'Passwords do not match',
  hnsWalletPasswordMatch: 'Match!',
  hnsWalletPasswordIsStrong: 'Strong!',
  hnsWalletPasswordIsMediumStrength: 'Medium',
  hnsWalletPasswordIsWeak: 'Weak',

  // Create Password - Stength Tooltip
  hnsWalletPasswordStrengthTooltipHeading: 'At least:',
  hnsWalletPasswordStrengthTooltipIsLongEnough: '8 characters',

  // Onboarding Success
  hnsWalletOnboardingSuccessTitle: 'Congratulations! Your Hns Wallet is ready to go!',
  hnsWalletOnboardingSuccessDescription: 'To access your wallet, just click the wallet icon at the top right of any Hns browser window.',
  hnsWalletBuyCryptoButton: 'Buy crypto',
  hnsWalletDepositCryptoButton: 'Deposit',
  hnsWalletLearnAboutMyWallet: 'Learn more about my new wallet',

  // Wallet Article Links
  hnsWalletArticleLinkWhatsARecoveryPhrase: 'What’s a recovery phrase?',

  // Lock Screen
  hnsWalletEnterYourHnsWalletPassword: 'Enter your Hns Wallet password',
  hnsWalletLockScreenTitle: 'Enter password to unlock wallet',
  hnsWalletLockScreenButton: 'Unlock',
  hnsWalletLockScreenError: 'Incorrect password',
  hnsWalletUnlockWallet: 'Unlock Wallet',

  // Wallet More Popup
  hnsWalletWalletPopupSettings: 'Settings',
  hnsWalletWalletPopupLock: 'Lock wallet',
  hnsWalletWalletPopupBackup: 'Backup Wallet',
  hnsWalletWalletPopupConnectedSites: 'Connected sites',
  hnsWalletWalletPopupHideBalances: 'Balances',
  hnsWalletWalletPopupShowGraph: 'Graph',
  hnsWalletWalletNFTsTab: 'NFTs tab',

  // Backup Warning
  hnsWalletBackupWarningText: 'Back up your wallet now to protect your assets and ensure you never lose access.',
  hnsWalletBackupButton: 'Back up now',
  hnsWalletDismissButton: 'Dismiss',

  // Default Wallet Banner
  hnsWalletDefaultWalletBanner: 'Hns Wallet is not set as your default wallet and will not respond to Web3 DApps. Visit settings to change your default wallet.',

  // Restore Screen
  hnsWalletRestoreTite: 'Restore primary crypto accounts',
  hnsWalletRestoreDescription: 'Enter your recovery phrase to restore your Hns wallet crypto account.',
  hnsWalletRestoreError: 'The recovery phrase entered is invalid.',
  hnsWalletRestorePlaceholder: 'Paste recovery phrase from clipboard',
  hnsWalletRestoreShowPhrase: 'Show recovery phrase',
  hnsWalletRestoreLegacyCheckBox: 'Import from legacy Hns crypto wallets?',
  hnsWalletRestoreFormText: 'New Password',

  // Clipboard
  hnsWalletToolTipCopyToClipboard: 'Copy to Clipboard',
  hnsWalletToolTipCopiedToClipboard: 'Copied!',
  hnsWalletPasteFromClipboard: 'Paste from clipboard',

  // Accounts Tab
  hnsWalletAccountsPrimary: 'Primary crypto accounts',
  hnsWalletAccountsSecondary: 'Imported accounts',
  hnsWalletConnectedHardwareWallets: 'Connected hardware wallets',
  hnsWalletAccountsAssets: 'Assets',
  hnsWalletAccountsEditVisibleAssets: 'Visible assets',
  hnsWalletAccountBalance: 'Account balance',

  // Add Account Options
  hnsWalletCreateAccount: 'Create $1 account',
  hnsWalletAddAccountCreate: 'Create',
  hnsWalletAddAccountImport: 'Import',
  hnsWalletAddAccountImportHardware: 'Import from hardware wallet',
  hnsWalletAddAccountHardware: 'Hardware',
  hnsWalletAddAccountConnect: 'Connect',
  hnsWalletAddAccountPlaceholder: 'Account name',
  hnsWalletCreateAccountButton: 'Create account',
  hnsWalletCreateAccountImportAccount: 'Import $1 account',
  hnsWalletCreateAccountTitle: 'Select one of the following account types',
  hnsWalletCreateAccountEthereumDescription: 'Supports EVM compatible assets on the Ethereum blockchain (ERC-20, ERC-721, ERC-1551, ERC-1155)',
  hnsWalletCreateAccountSolanaDescription: 'Supports SPL compatible assets on the Solana blockchain',
  hnsWalletCreateAccountFilecoinDescription: 'Store FIL asset',
  hnsWalletFilecoinPrivateKeyProtocol: 'Private key $1',

  // Import Account
  hnsWalletImportAccountDisclaimer: 'These accounts can be used with Web3 DApps, and can be shown in your portfolio. However, note that secondary accounts cannot be restored via recovery phrase from your primary account backup.',
  hnsWalletImportAccountPlaceholder: 'Paste private key from clipboard',
  hnsWalletImportAccountKey: 'Private key',
  hnsWalletImportAccountFile: 'JSON file',
  hnsWalletImportAccountUploadButton: 'Choose file',
  hnsWalletImportAccountUploadPlaceholder: 'No file chosen',
  hnsWalletImportAccountError: 'Failed to import account, please try again.',
  hnsWalletImportAccount: 'Import account',

  // Connect Hardware Wallet
  hnsWalletConnectHardwareTitle: 'Select your hardware wallet device',
  hnsWalletConnectHardwareInfo1: 'Connect your $1 wallet directly to your computer.',
  hnsWalletConnectHardwareInfo2: 'Unlock your device and select the $1 app.',
  hnsWalletConnectHardwareTrezor: 'Trezor',
  hnsWalletConnectHardwareLedger: 'Ledger',
  hnsWalletConnectHardwareAuthorizationNeeded: 'Grant Hns access to your Ledger device.',
  hnsWalletConnectingHardwareWallet: 'Connecting...',
  hnsWalletAddCheckedAccountsHardwareWallet: 'Add checked accounts',
  hnsWalletLoadMoreAccountsHardwareWallet: 'Load more',
  hnsWalletLoadingMoreAccountsHardwareWallet: 'Loading more...',
  hnsWalletSearchScannedAccounts: 'Search account',
  hnsWalletSwitchHDPathTextHardwareWallet: 'Try switching HD path (above) if you cannot find the account you are looking for.',
  hnsWalletLedgerLiveDerivationPath: 'Ledger Live',
  hnsWalletLedgerLegacyDerivationPath: 'Legacy (MEW/MyCrypto)',
  hnsWalletUnknownInternalError: 'Unknown error, please reconnect your hardware wallet and try again.',
  hnsWalletConnectHardwareSearchNothingFound: 'No results found.',

  // Account Settings Modal
  hnsWalletAccountSettingsDetails: 'Details',
  hnsWalletAccountSettingsWatchlist: 'Visible assets',
  hnsWalletAccountSettingsPrivateKey: 'Private key',
  hnsWalletAccountSettingsSave: 'Save',
  hnsWalletAccountSettingsRemove: 'Remove account',
  hnsWalletWatchlistAddCustomAsset: 'Add custom asset',
  hnsWalletWatchListTokenName: 'Token name',
  hnsWalletWatchListTokenAddress: 'Token address',
  hnsWalletWatchListTokenSymbol: 'Token symbol',
  hnsWalletWatchListTokenDecimals: 'Decimals of precision',
  hnsWalletWatchListAdd: 'Add',
  hnsWalletWatchListDoneButton: 'Done',
  hnsWalletWatchListNoAsset: 'No assets named',
  hnsWalletWatchListSearchPlaceholder: 'Search assets or contract address',
  hnsWalletWatchListError: 'Failed to add custom token, please try again.',
  hnsWalletCustomTokenExistsError: 'This token has already been added to your portfolio.',
  hnsWalletAccountSettingsDisclaimer: 'WARNING: Never share your recovery phrase. Anyone with this phrase can take your assets forever.',
  hnsWalletAccountSettingsShowKey: 'Show key',
  hnsWalletAccountSettingsHideKey: 'Hide key',
  hnsWalletAccountSettingsUpdateError: 'Failed to update account name, please try again.',
  hnsWalletWatchListTokenId: 'Token ID (only for ERC721)',
  hnsWalletWatchListTokenIdError: 'Token ID is required',
  hnsWalletWatchListAdvanced: 'Advanced',
  hnsWalletWatchListCoingeckoId: 'Coingecko ID',
  hnsWalletIconURL: 'Icon URL',
  hnsWalletAddAsset: 'Add asset',
  hnsWalletAccountsExport: 'Export',
  hnsWalletAccountsDeposit: 'Deposit',
  hnsWalletAccountsRemove: 'Remove',

  // Empty Token List State
  hnsWalletNoAvailableAssets: 'No available assets',
  hnsWalletNoAvailableAssetsDescription:
    'Deposit or purchase tokens to get started. If you don\'t see tokens from an imported account, check the filters and display settings. Unknown tokens may need to be added as custom assets.',

  // AmountPresets
  hnsWalletPreset25: '25%',
  hnsWalletPreset50: '50%',
  hnsWalletPreset75: '75%',
  hnsWalletPreset100: '100%',

  // Ordinals
  hnsWalletOrdinalFirst: 'First',
  hnsWalletOrdinalSecond: 'Second',
  hnsWalletOrdinalThird: 'Third',
  hnsWalletOrdinalFourth: 'Fourth',
  hnsWalletOrdinalFifth: 'Fifth',
  hnsWalletOrdinalSixth: 'Sixth',
  hnsWalletOrdinalSeventh: 'Seventh',
  hnsWalletOrdinalEighth: 'Eighth',
  hnsWalletOrdinalNinth: 'Ninth',
  hnsWalletOrdinalTenth: 'Tenth',
  hnsWalletOrdinalEleventh: 'Eleventh',
  hnsWalletOrdinalTwelfth: 'Twelfth',
  hnsWalletOridinalThirteenth: 'Thirteenth',
  hnsWalletOrdinalFourteenth: 'Fourteenth',
  hnsWalletOrdinalFifteenth: 'Fifteenth',
  hnsWalletOrdinalSixteenth: 'Sixteenth',
  hnsWalletOrdinalSeventeenth: 'Seventeenth',
  hnsWalletOrdinalEighteenth: 'Eighteenth',
  hnsWalletOrdinalNineteenth: 'Nineteenth',
  hnsWalletOrdinalTwentieth: 'Twentieth',
  hnsWalletOrdinalTwentyFirst: 'Twenty-first',
  hnsWalletOrdinalTwentySecond: 'Twenty-second',
  hnsWalletOrdinalTwentyThird: 'Twenty-third',
  hnsWalletOrdinalTwentyFourth: 'Twenty-fourth',
  hnsWalletOrdinalSuffixOne: 'st',
  hnsWalletOrdinalSuffixTwo: 'nd',
  hnsWalletOrdinalSuffixFew: 'rd',
  hnsWalletOrdinalSuffixOther: 'th',

  // Networks
  hnsWalletNetworkETH: 'Ethereum',
  hnsWalletNetworkMain: 'Mainnet',
  hnsWalletNetworkTest: 'Test Network',
  hnsWalletNetworkGoerli: 'Goerli',
  hnsWalletNetworkBinance: 'Binance Smart Chain',
  hnsWalletNetworkBinanceAbbr: 'BSC',
  hnsWalletNetworkLocalhost: 'Localhost',

  // Select Screens
  hnsWalletSelectAccount: 'Select account',
  hnsWalletSearchAccount: 'Search accounts',
  hnsWalletSelectNetwork: 'Select network',
  hnsWalletSelectAsset: 'Select from',
  hnsWalletSearchAsset: 'Search coins',
  hnsWalletSelectCurrency: 'Select currency',
  hnsWalletSearchCurrency: 'Search currencies',

  // Swap
  hnsWalletSwapFrom: 'Amount',
  hnsWalletSwapTo: 'To',
  hnsWalletSwapEstimate: 'estimate',
  hnsWalletSwapMarket: 'Market',
  hnsWalletSwapLimit: 'Limit',
  hnsWalletSwapPriceIn: 'Price in',
  hnsWalletSwapInsufficientBalance: 'Insufficient balance',
  hnsWalletSwapInsufficientFundsForGas: 'Insufficient funds for gas',
  hnsWalletSwapInsufficientLiquidity: 'Insufficient liquidity',
  hnsWalletSwapInsufficientAllowance: 'Activate token',
  hnsWalletSwapUnknownError: 'Unknown error',
  hnsWalletSwapReviewSpend: 'You spend',
  hnsWalletSwapReviewReceive: "You'll receive",
  hnsWalletSwapReviewHeader: 'Confirm order',

  // Buy
  hnsWalletBuyTitle: 'Test faucet',
  hnsWalletBuyDescription: 'Get Ether from a faucet for $1',
  hnsWalletBuyFaucetButton: 'Get Ether',
  hnsWalletBuyContinueButton: 'Select purchase method',
  hnsWalletBuySelectAsset: 'Select an asset',
  hnsWalletBuyRampNetworkName: 'Ramp.Network',
  hnsWalletBuySardineName: 'Sardine',
  hnsWalletBuyTransakName: 'Transak',
  hnsWalletBuyStripeName: 'Link by Stripe',
  hnsWalletBuyCoinbaseName: 'Coinbase Pay',
  hnsWalletBuyRampDescription: 'Buy with CC/Debit or ACH. Competitive Rates.',
  hnsWalletBuySardineDescription: 'Easiest, fastest and cheapest way to buy crypto with card and bank transfers.',
  hnsWalletBuyTransakDescription: 'Instant buy with your bank account. Lower fees.',
  hnsWalletBuyStripeDescription: 'Pay with credit, debit, bank account',
  hnsWalletBuyCoinbaseDescription: 'Buy with the most trusted name in crypto.',
  hnsWalletBuyWithRamp: 'Buy with Ramp',
  hnsWalletBuyWithSardine: 'Buy with Sardine',
  hnsWalletBuyWithTransak: 'Buy with Transak',
  hnsWalletBuyWithStripe: 'Buy with Link',
  hnsWalletBuyWithCoinbase: 'Buy with Coinbase Pay',
  hnsWalletSellWithProvider: 'Sell with $1',
  hnsWalletBuyDisclaimer: 'Financial and transaction data is processed by our onramp partners. Hns does not collect or have access to such data.',

  // Fund Wallet Screen
  hnsWalletFundWalletTitle: 'To finish your $1 purchase, select one of our partners',
  hnsWalletFundWalletDescription: 'On completion, your funds will be transfered to your Hns Wallet',

  // Deposit Funds Screen
  hnsWalletDepositFundsTitle: 'Deposit crypto',
  hnsWalletDepositX: 'Deposit $1',
  hnsWalletDepositSolSplTokens: 'Deposit Solana or SPL tokens',
  hnsWalletDepositErc: 'Deposit ERC-based tokens',
  hnsWalletDepositOnlySendOnXNetwork: 'Only send tokens to this address on $1',

  // Sign Transaction Panel
  hnsWalletSignTransactionTitle: 'Your signature is being requested',
  hnsWalletSignWarning: 'Note that Hns can’t verify what will happen if you sign. A signature could authorize nearly any operation in your account or on your behalf, including (but not limited to) giving total control of your account and crypto assets to the site making the request. Only sign if you’re sure you want to take this action, and trust the requesting site.',
  hnsWalletSignWarningTitle: 'Sign at your own risk',
  hnsWalletSignTransactionMessageTitle: 'Message',
  hnsWalletSignTransactionEIP712MessageTitle: 'Details',
  hnsWalletSignTransactionEIP712MessageDomain: 'Domain',
  hnsWalletSignTransactionButton: 'Sign',
  hnsWalletApproveTransaction: 'Approve transaction',

  // Encryption Key Panel
  hnsWalletProvideEncryptionKeyTitle: 'A DApp is requesting your public encryption key',
  hnsWalletProvideEncryptionKeyDescription: '$1$url$2 is requesting your wallets public encryption key. If you consent to providing this key, the site will be able to compose encrypted messages to you.',
  hnsWalletProvideEncryptionKeyButton: 'Provide',
  hnsWalletReadEncryptedMessageTitle: 'This DApp would like to read this message to complete your request',
  hnsWalletReadEncryptedMessageDecryptButton: 'Decrypt message',
  hnsWalletReadEncryptedMessageButton: 'Allow',

  // Allow Spend ERC20 Panel
  hnsWalletAllowSpendTitle: 'Allow this app to spend your $1?',
  hnsWalletAllowSpendDescription: 'By granting this permission, you are allowing this app to withdraw your $1 and automate transactions for you.',
  hnsWalletAllowSpendBoxTitle: 'Edit permissions',
  hnsWalletAllowSpendTransactionFee: 'Transaction fee',
  hnsWalletAllowSpendEditButton: 'Edit',
  hnsWalletAllowSpendDetailsButton: 'View details',
  hnsWalletAllowSpendRejectButton: 'Reject',
  hnsWalletAllowSpendConfirmButton: 'Confirm',
  hnsWalletAllowSpendUnlimitedWarningTitle: 'Unlimited approval requested',

  // Allow Add or Change Network Panel
  hnsWalletAllowAddNetworkTitle: 'Allow this site to add a network?',
  hnsWalletAllowAddNetworkDescription: 'This will allow this network to be used within Hns Wallet.',
  hnsWalletAllowAddNetworkLearnMoreButton: 'Learn more.',
  hnsWalletAllowAddNetworkName: 'Network name',
  hnsWalletAllowAddNetworkUrl: 'Network URL',
  hnsWalletAllowAddNetworkDetailsButton: 'View all details',
  hnsWalletAllowAddNetworkButton: 'Approve',
  hnsWalletAllowAddNetworkChainID: 'Chain ID',
  hnsWalletAllowAddNetworkCurrencySymbol: 'Currency symbol',
  hnsWalletAllowAddNetworkExplorer: 'Block explorer URL',
  hnsWalletAllowChangeNetworkTitle: 'Allow this site to switch the network?',
  hnsWalletAllowChangeNetworkDescription: 'This will switch the network to a previously added network.',
  hnsWalletAllowChangeNetworkButton: 'Switch network',
  hnsWalletAllowAddNetworkNetworkPanelTitle: 'Network',
  hnsWalletAllowAddNetworkDetailsPanelTitle: 'Details',

  // Confirm Transaction Panel
  hnsWalletConfirmTransactionTotal: 'Total',
  hnsWalletConfirmTransactionGasFee: 'Gas fee',
  hnsWalletConfirmTransactionTransactionFee: 'Transaction fee',
  hnsWalletConfirmTransactionBid: 'Bid',
  hnsWalletConfirmTransactionAmountGas: 'Amount + gas',
  hnsWalletConfirmTransactionAmountFee: 'Amount + fee',
  hnsWalletConfirmTransactionNoData: 'No data.',
  hnsWalletConfirmTransactionNext: 'next',
  hnsWalletConfirmTransactionFrist: 'first',
  hnsWalletConfirmTransactions: 'transactions',
  hnsWalletConfirmTransactionAccountCreationFee: 'The associated token account does not exist yet. A small amount of SOL will be spent to create and fund it.',
  hnsWalletAllowSpendCurrentAllowance: 'Current allowance',
  hnsWalletAllowSpendProposedAllowance: 'Proposed allowance',
  hnsWalletTransactionGasLimit: 'Gas Limit',
  hnsWalletTransactionGasPremium: 'Gas Premium',
  hnsWalletTransactionGasFeeCap: 'Gas Fee Cap',
  hnsWalletNetworkFees: 'Network fees',

  // Wallet Main Panel
  hnsWalletPanelTitle: 'Hns Wallet',
  hnsWalletPanelConnected: 'Connected',
  hnsWalletPanelNotConnected: 'Connect',
  hnsWalletPanelViewAccountAssets: 'View account assets',
  hnsWalletAssetsPanelTitle: 'Account assets',
  hnsWalletPanelDisconnected: 'Disconnected',
  hnsWalletPanelBlocked: 'Blocked',
  hnsWalletTitle: 'Wallet',

  // Wallet Welcome Panel
  hnsWalletWelcomePanelDescription: 'Use this panel to securely access Web3 and all your crypto assets.',

  // Site Permissions Panel
  hnsWalletSitePermissionsTitle: 'Site permissions',
  hnsWalletSitePermissionsAccounts: '$1 accounts connected',
  hnsWalletSitePermissionsDisconnect: 'Disconnect',
  hnsWalletSitePermissionsSwitch: 'Switch',
  hnsWalletSitePermissionsNewAccount: 'New account',
  hnsWalletSitePermissionsTrust: 'Trust',
  hnsWalletSitePermissionsRevoke: 'Revoke',

  // DApp Connection Panel
  hnsWalletChangeNetwork: 'Change network',
  hnsWalletChangeAccount: 'Change account',
  hnsWalletActive: 'Active',
  hnsWalletNotConnected: 'Not connected',
  hnsWalletConnectedAccounts: 'Connected accounts',
  hnsWalletAvailableAccounts: 'Available accounts',

  // Transaction Detail Box
  hnsWalletTransactionDetailBoxFunction: 'FUNCTION TYPE',
  hnsWalletTransactionDetailBoxHex: 'HEX DATA',
  hnsWalletTransactionDetailBoxBytes: 'BYTES',

  // Connect With Site Panel
  hnsWalletConnectWithSiteNext: 'Next',
  hnsWalletConnectWallet: 'Connect wallet',
  hnsWalletConnectWithSite: 'or connect with:',
  hnsWalletConnectPermittedLabel: 'This app will be able to:',
  hnsWalletConnectNotPermittedLabel: 'It will not be able to:',
  hnsWalletConnectPermissionBalanceAndActivity: 'Check wallet balance and activity',
  hnsWalletConnectPermissionRequestApproval: 'Request approval for transactions and signatures',
  hnsWalletConnectPermissionAddress: 'View your permitted wallet address',
  hnsWalletConnectPermissionMoveFunds: 'Move funds without your permission',
  hnsWalletConnectTrustWarning: 'Make sure you trust this site.',

  // Permission Duration
  hnsWalletPermissionDuration: 'Permission duration',
  hnsWalletPermissionUntilClose: 'Until I close this site',
  hnsWalletPermissionOneDay: 'For 24 hours',
  hnsWalletPermissionOneWeek: 'For 1 week',
  hnsWalletPermissionForever: 'Forever',

  // Import from Legacy Wallet
  hnsWalletCryptoWalletsDetected: 'Existing crypto wallets detected',
  hnsWalletCryptoWalletsDescriptionTwo: 'If youd rather skip the import and keep the old Crypto Wallets experience, just navigate to the Hns Browser $1Settings$2 and change the default back to Crypto Wallets. You can also import, try the new Hns Wallet, and change back at any time.',
  hnsWalletImportHnsLegacyDescription: 'Enter your existing crypto wallets password to import to Hns Wallet. Enjoy a faster and more secure way to manage crypto assets and interact with Web3 DApps.',
  hnsWalletImportHnsLegacyInput: 'Type Crypto wallets password',

  // Connect Hardware Wallet Panel
  hnsWalletConnectHardwarePanelConnected: '$1 connected',
  hnsWalletConnectHardwarePanelDisconnected: '$1 disconnected',
  hnsWalletConnectHardwarePanelInstructions: 'Instructions',
  hnsWalletConnectHardwarePanelConnect: 'Connect your $1',
  hnsWalletConnectHardwarePanelConfirmation: 'Hardware wallet requires transaction confirmation on device.',
  hnsWalletConnectHardwarePanelOpenApp: 'Hardware wallet requires $1 App opened on $2',

  // Transaction History Panel (Empty)
  hnsWalletNoTransactionsYet: 'No transaction history',
  hnsWalletNoTransactionsYetDescription:
    'Transactions made with your Hns Wallet will appear here.',

  // Transaction List Item
  hnsWalletTransactionSent: 'sent',
  hnsWalletTransactionReceived: 'received',
  hnsWalletTransactionExplorerMissing: 'Block explorer URL is not available.',
  hnsWalletTransactionExplorer: 'View on block explorer',
  hnsWalletTransactionCopyHash: 'Copy transaction hash',
  hnsWalletTransactionSpeedup: 'Speedup transaction',
  hnsWalletTransactionCancel: 'Cancel transaction',
  hnsWalletTransactionRetry: 'Retry transaction',
  hnsWalletTransactionPlaceholder: 'Transactions will appear here',
  hnsWalletTransactionApproveUnlimited: 'Unlimited',
  hnsWalletApprovalTransactionIntent: 'approve',

  // Asset Detail Accounts (Empty)
  hnsWalletNoAccountsWithABalance: 'No available accounts',
  hnsWalletNoAccountsWithABalanceDescription:
    'Accounts with a balance will appear here.',

  // Edit Gas
  hnsWalletEditGasTitle1: 'Max priority fee',
  hnsWalletEditGasTitle2: 'Edit gas',
  hnsWalletEditGasDescription: 'While not a guarantee, miners will likely prioritize your transaction if you pay a higher fee.',
  hnsWalletEditGasLow: 'Low',
  hnsWalletEditGasOptimal: 'Optimal',
  hnsWalletEditGasHigh: 'High',
  hnsWalletEditGasLimit: 'Gas limit',
  hnsWalletEditGasPerGasPrice: 'Per-gas price (Gwei)',
  hnsWalletEditGasPerTipLimit: 'Per-gas tip limit (Gwei)',
  hnsWalletEditGasPerPriceLimit: 'Per-gas price limit (Gwei)',
  hnsWalletEditGasMaxFee: 'Max fee',
  hnsWalletEditGasMaximumFee: 'Maximum fee',
  hnsWalletEditGasBaseFee: 'Current base fee',
  hnsWalletEditGasGwei: 'Gwei',
  hnsWalletEditGasSetCustom: 'Set custom',
  hnsWalletEditGasSetSuggested: 'Set suggested',
  hnsWalletEditGasZeroGasPriceWarning: 'Transaction may not be propagated in the network.',
  hnsWalletEditGasLimitError: 'Gas limit must be an integer greater than 0',
  hnsWalletGasFeeLimitLowerThanBaseFeeWarning:
    'Fee limit is set lower than the base fee. ' +
    'Your transaction may take a long time or fail.',

  // Advanced transaction settings
  hnsWalletAdvancedTransactionSettings: 'Advanced settings',
  hnsWalletAdvancedTransactionSettingsPlaceholder: 'Enter custom nonce value',
  hnsWalletEditNonce: 'Nonce',
  hnsWalletEditNonceInfo: 'The nonce value will be auto-determined if this field is not specified.',

  // Edit permissions
  hnsWalletEditPermissionsTitle: 'Edit permissions',
  hnsWalletEditPermissionsDescription: 'Spend limit permission allows $1 to withdraw and spend up to the following amount:',
  hnsWalletEditPermissionsButton: 'Edit permissions',
  hnsWalletEditPermissionsProposedAllowance: 'Proposed allowance',
  hnsWalletEditPermissionsCustomAllowance: 'Custom allowance',

  // Send Input Errors
  hnsWalletNotValidFilAddress: 'Not a valid FIL address',
  hnsWalletNotValidEthAddress: 'Not a valid ETH address',
  hnsWalletNotValidSolAddress: 'Not a valid SOL address',
  hnsWalletNotValidAddress: 'Not a valid address',
  hnsWalletNotDomain: 'Domain doesn\'t have a linked $ address',
  hnsWalletSameAddressError: 'The receiving address is your own address',
  hnsWalletContractAddressError: 'The receiving address is a tokens contract address',
  hnsWalletFailedChecksumTitle: 'Address doesn’t look correct',
  hnsWalletFailedChecksumDescription: 'Check your address to make sure it’s the right address (e.g. letters with lower or upper case).',
  hnsWalletHowToSolve: 'How can I solve it?',
  hnsWalletAddressMissingChecksumInfoWarning: 'This address cannot be verified (missing checksum). Proceed?',
  hnsWalletNotValidChecksumAddressError: 'Address did not pass verification (invalid checksum). Please try again, replacing lowercase letters with uppercase.',
  hnsWalletMissingGasLimitError: 'Missing gas limit',
  hnsWalletZeroBalanceError: 'Amount must be greater than 0',
  hnsWalletAddressRequiredError: 'To address is required',
  hnsWalletInvalidRecipientAddress: 'Invalid recipient address',
  hnsWalletChecksumModalTitle: 'How can I find the right address?',
  hnsWalletChecksumModalDescription: 'Hns validates and prevents users from sending funds to the wrong address due to incorrect capitalization. This is a "checksum" process to verify that it is a valid Ethereum address.',
  hnsWalletChecksumModalStepOneTitle: '1. Visit',
  hnsWalletChecksumModalStepOneDescription: 'Visit etherscan and paste the wallet address you want to send tokens. Then enter.',
  hnsWalletChecksumModalStepTwoTitle: '2. Copy and enter ETH address',
  hnsWalletChecksumModalStepTwoDescription: 'Copy and enter the correct address. You can see that some characters have been converted correctly.',
  hnsWalletChecksumModalNeedHelp: 'Need more help?',

  // Transaction Queue Strings
  hnsWalletQueueOf: 'of',
  hnsWalletQueueNext: 'next',
  hnsWalletQueueFirst: 'first',
  hnsWalletQueueRejectAll: 'Reject transactions',

  // Add Suggested Token Panel
  hnsWalletAddSuggestedTokenTitle: 'Add suggested token',
  hnsWalletAddSuggestedTokenDescription: 'Would you like to import this token?',

  // Transaction Detail Panel
  hnsWalletRecentTransactions: 'Recent transactions',
  hnsWalletTransactionDetails: 'Transaction details',
  hnsWalletTransactionDetailDate: 'Date',
  hnsWalletTransactionDetailSpeedUp: 'Speedup',
  hnsWalletTransactionDetailHash: 'Transaction hash',
  hnsWalletTransactionDetailNetwork: 'Network',
  hnsWalletTransactionDetailStatus: 'Status',

  // Transactions Status
  hnsWalletTransactionStatusUnapproved: 'Unapproved',
  hnsWalletTransactionStatusApproved: 'Approved',
  hnsWalletTransactionStatusRejected: 'Rejected',
  hnsWalletTransactionStatusSubmitted: 'Submitted',
  hnsWalletTransactionStatusConfirmed: 'Confirmed',
  hnsWalletTransactionStatusError: 'Error',
  hnsWalletTransactionStatusDropped: 'Dropped',
  hnsWalletTransactionStatusSigned: 'Signed',

  // NFT Details Page
  hnsWalletNFTDetailBlockchain: 'Blockchain',
  hnsWalletNFTDetailTokenStandard: 'Token standard',
  hnsWalletNFTDetailTokenID: 'Token ID',
  hnsWalletNFTDetailContractAddress: 'Contract address',
  hnsWalletNFTDetailDescription: 'Description',
  hnsWalletNFTDetailImageAddress: 'Image URL',
  hnsWalletNFTDetailsPinningInProgress: 'In progress',
  hnsWalletNFTDetailsPinningSuccessful: 'Pinned',
  hnsWalletNFTDetailsPinningFailed: 'Failed',
  hnsWalletNFTDetailsNotAvailable: 'Not available yet',
  hnsWalletNFTDetailsOverview: 'Overview',
  hnsWalletNFTDetailsOwnedBy: 'Owned by',
  hnsWalletNFTDetailsViewAccount: 'View account',
  hnsWalletNFTDetailsPinningStatusLabel: 'IPFS pinning status',
  hnsWalletNFTDetailsCid: 'CID',
  hnsWalletNFTDetailsProperties: 'Properties',

  // Sweepstakes
  hnsWalletSweepstakesTitle: 'Hns Swap-stakes',
  hnsWalletSweepstakesDescription: '7 days of crypto giveaways, ~$500k in prizes.',
  hnsWalletSweepstakesCallToAction: 'Enter now!',

  // Market Data Filters
  hnsWalletMarketDataAllAssetsFilter: 'All Assets',
  hnsWalletMarketDataTradableFilter: 'Tradable',
  hnsWalletMarketDataAssetsColumn: 'Assets',
  hnsWalletMarketDataPriceColumn: 'Price',
  hnsWalletMarketData24HrColumn: '24Hr',
  hnsWalletMarketDataMarketCapColumn: 'Cap',
  hnsWalletMarketDataVolumeColumn: 'Volume',
  hnsWalletMarketDataBuyDepositColumn: 'Buy/Deposit',

  // Market Coin Detail Screen
  hnsWalletInformation: 'Information',
  hnsWalletRankStat: 'Rank',
  hnsWalletVolumeStat: '24h Volume',
  hnsWalletMarketCapStat: 'Market Cap',

  // Network Filter
  hnsWalletNetworkFilterAll: 'All Networks',
  hnsWalletNetworkFilterSecondary: 'Secondary networks',
  hnsWalletNetworkFilterTestNetworks: 'Test networks',

  // Asset Filter
  hnsWalletAssetFilterLowToHigh: 'Low to High',
  hnsWalletAssetFilterHighToLow: 'High to Low',
  hnsWalletAssetFilterAToZ: 'A to Z',
  hnsWalletAssetFilterZToA: 'Z to A',

  // Asset Group By
  hnsWalletNone: 'None',
  hnsWalletNetworks: 'Networks',
  hnsWalletPortfolioGroupByTitle: 'Group by',
  hnsWalletPortfolioGroupByDescription: 'Group assets by',

  // Portfolio Filters
  hnsWalletPortfolioFiltersTitle: 'Filters and display settings',
  hnsWalletPortfolioNftsFiltersTitle: 'NFT display settings',
  hnsWalletSortAssets: 'Sort assets',
  hnsWalletSortAssetsDescription: 'Sort by fiat value or name',
  hnsWalletHideSmallBalances: 'Hide small balances',
  hnsWalletHideSmallBalancesDescription: 'Assets with value less than $1',
  hnsWalletSelectAccounts: 'Select accounts',
  hnsWalletSelectNetworks: 'Select networks',
  hnsWalletSelectAll: 'Select all',
  hnsWalletDeselectAll: 'Deselect all',
  hnsWalletPrimaryNetworks: 'Primary networks',
  hnsWalletETHAccountDescrption: 'Ethereum + EVM Chains',
  hnsWalletSOLAccountDescrption: 'Solana + SVM Chains',
  hnsWalletFILAccountDescrption: 'Filecoin',
  hnsWalletBTCAccountDescrption: 'Bitcoin',
  hnsWalletShowNetworkLogoOnNftsTitle: 'Network Logo',
  hnsWalletShowNetworkLogoOnNftsDescription: 'Show network logo on NFTs',
  hnsWalletShowSpamNftsTitle: 'Spam NFTs',
  hnsWalletShowSpamNftsDescription: 'Show Spam NFTs',


  // Account Filter
  hnsWalletAccountFilterAllAccounts: 'All accounts',

  // Transaction post-confirmation

  // Submitted
  hnsWalletTransactionSubmittedTitle: 'Transaction submitted',
  hnsWalletTransactionSubmittedDescription: 'Transaction has been successfully sent to the network and awaits confirmation.',

  // Failed
  hnsWalletTransactionFailedHeaderTitle: '$1 was returned to your wallet',
  hnsWalletTransactionFailedTitle: 'Transaction failed',
  hnsWalletTransactionFailedDescription: 'Transaction was failed due to a large price movement. Increase slippage tolerance to succeed at a larger price movement.',
  hnsWalletTransactionFailedSwapNextCTA: 'New trade',
  hnsWalletTransactionFailedNextCTA: 'New transaction',
  hnsWalletTransactionFailedViewErrorCTA: 'View error',
  hnsWalletTransactionFailedReceiptCTA: 'Receipt',
  hnsWalletTransactionFailedModalTitle: 'Error message',
  hnsWalletTransactionFailedModalSubtitle: 'Please save the error message for future reference.',

  // Complete
  hnsWalletTransactionCompleteSwapHeaderTitle: 'Swapped $1 to $2',
  hnsWalletTransactionCompleteTitle: 'Transaction complete!',
  hnsWalletTransactionCompleteDescription: 'Transaction was successful. Please wait for confirmations, to avoid the risk of double-spend.',
  hnsWalletTransactionCompleteReceiptCTA: 'Receipt',

  // Confirming
  hnsWalletTransactionConfirmingTitle: 'Transaction is processing',
  // [FIXME]: change the wording after ETH2.
  hnsWalletTransactionConfirmingDescription: 'Transaction was successfully included in a block. To avoid the risk of double spending, we recommend waiting for block confirmations.',
  hnsWalletTransactionConfirmingText: 'Confirming',

  // Transaction intents for confirmation panels
  hnsWalletTransactionIntentDappInteraction: 'Dapp interaction',
  hnsWalletTransactionIntentSend: 'Send $1',
  hnsWalletTransactionIntentSwap: 'Swap $1 to $2',

  // Solana ProgramID Names
  hnsWalletSolanaSystemProgram: 'System Program',
  hnsWalletSolanaConfigProgram: 'Config Program',
  hnsWalletSolanaStakeProgram: 'Stake Program',
  hnsWalletSolanaVoteProgram: 'Vote Program',
  hnsWalletSolanaBPFLoader: 'BPF Loader',
  hnsWalletSolanaEd25519Program: 'Ed25519 Program',
  hnsWalletSolanaSecp256k1Program: 'Secp256k1 Program',
  hnsWalletSolanaTokenProgram: 'Token Program',
  hnsWalletSolanaAssociatedTokenProgram: 'Associated Token Program',
  hnsWalletSolanaMetaDataProgram: 'MetaData Program',
  hnsWalletSolanaSysvarRentProgram: 'SysvarRent Program',

  // Solana Instruction Parameter Names
  hnsWalletSolanaAccounts: 'Accounts:',
  hnsWalletSolanaData: 'Data:',
  hnsWalletSolanaProgramID: 'Program ID:',
  hnsWalletSolanaMaxRetries: 'Max Retries:',
  hnsWalletSolanaPreflightCommitment: 'Preflight Commitment:',
  hnsWalletSolanaSkipPreflight: 'Skip Preflight:',
  hnsWalletSolanaAddressLookupTableAccount: 'Address Lookup Table Account:',
  hnsWalletSolanaAddressLookupTableIndex: 'Address Lookup Table Index:',

  // Help Center
  hnsWalletHelpCenter: 'Help center',
  hnsWalletHelpCenterText: 'Need help? See',

  // Remove Account Modal
  hnsWalletRemoveAccountModalTitle: 'Are you sure you want to remove "$1"?',

  // Bridge to Aurora
  hnsWalletAuroraModalTitle: 'Open the Rainbow Bridge app?',
  hnsWalletAuroraModalDescription: 'Rainbow Bridge is an independent service that helps you bridge assets across networks, and use your crypto on other networks and DApp ecosystems. Bridging assets to other networks has some risks.',
  hnsWalletAuroraModalLearnMore: 'Learn more about using Rainbow Bridge',
  hnsWalletAuroraModalLearnMoreAboutRisk: 'Learn more about mitigating risk on Rainbow Bridge',
  hnsWalletAuroraModalDontShowAgain: 'Don\'t show again',
  hnsWalletAuroraModalOPenButtonText: 'Open the Rainbow Bridge app',
  hnsWalletBridgeToAuroraButton: 'Bridge to Aurora',

  // Input field labels
  hnsWalletInputLabelPassword: 'Password',

  // Wallet Welcome Perks
  hnsWalletPerksTokens: 'Buy, send, and swap 200+ crypto assets',
  hnsWalletMultiChain: 'Multi-chain & NFT support',
  hnsWalletPerksBrowserNative: 'Browser-native, no risky extensions',

  // Portfolio Asset More Popup
  hnsWalletPortfolioTokenDetailsMenuLabel: 'Token details',
  hnsWalletPortfolioViewOnExplorerMenuLabel: 'View on block explorer',
  hnsWalletPortfolioHideTokenMenuLabel: 'Hide token',
  hnsWalletHideTokenModalTitle: 'Hide token',

  // Token detail modals
  hnsWalletMakeTokenVisibleInstructions: 'You can make this asset visible again in the future by clicking the "+ Visible assets" button at the bottom of the "Portfolio" tab',
  hnsWalletConfirmHidingToken: 'Hide',
  hnsWalletCancelHidingToken: 'Cancel',

  // Visible assets modal
  hnsWalletMyAssets: 'My assets',
  hnsWalletAvailableAssets: 'Available assets',
  hnsWalletDidntFindAssetEndOfList: 'Didn\'t find your asset on the list?',
  hnsWalletDidntFindAssetInList:
    'If you didn\'t find your asset in this list, you can add it manually by using the button below',
  hnsWalletAssetNotFound: 'Asset not found',

  // Request feature button
  hnsWalletRequestFeatureButtonText: 'Request feature',

  // Warnings
  hnsWalletNonAsciiCharactersInMessageWarning: 'Non-ASCII characters detected!',

  // ASCII toggles
  hnsWalletViewEncodedMessage: 'View original message',
  hnsWalletViewDecodedMessage: 'View message in ASCII encoding',

  // NFTs Tab
  hnsNftsTabImportNft: 'Import NFT',
  hnsNftsTabEmptyStateHeading: 'No NFTs here yet.',
  hnsNftsTabEmptyStateSubHeading: 'Ready to add some? Just click the button below to import.',
  hnsNftsTabEmptyStateDisclaimer: 'Compatible with NFTs on Solana (SPL) and Ethereum (ERC-721).',
  hnsNftsTab: 'NFTs',
  hnsNftsTabHidden: 'Hidden',
  hnsNftsTabCollected: 'Collected',
  hnsNftsTabHide: 'Hide',
  hnsNftsTabUnhide: 'Unhide',
  hnsNftsTabEdit: 'Edit',
  hnsNftsTabRemove: 'Don\'t show in wallet',

  // Add asset tabs
  hnsWalletAddAssetTokenTabTitle: 'Token',
  hnsWalletAddAssetNftTabTitle: 'NFT',
  hnsWalletNftFetchingError:
    'Something went wrong when fetching NFT details. Please try again later.',

  // Add Custom Asset Form
  hnsWalletNetworkIsRequiredError: 'Network is required',
  hnsWalletTokenNameIsRequiredError: 'Token name is required',
  hnsWalletInvalidTokenContractAddressError:
    'Invalid or empty token contract address',
  hnsWalletTokenSymbolIsRequiredError: 'Token symbol is required',
  hnsWalletTokenDecimalsIsRequiredError:
    'Token decimals of precision value is required',
  hnsWalletTokenContractAddress: 'Token Contract Address',
  hnsWalletTokenDecimal: 'Token Decimal',
  hnsWalletTokenMintAddress: 'Mint address',
  hnsWalletTransactionHasFeeEstimatesError: 'Unable to fetch fee estimates',

  // NFT Pinning
  hnsWalletNftPinningWhyNotAvailable: 'Why aren\'t some NFTs eligible?',
  hnsWalletNftPinningTooltip: 'Some NFT data is stored on centralized servers like AWS, Google Cloud, etc. In this case, it\’s not possible to pin your NFT data to the IPFS network.',
  hnsWalletNftPinningBenefitsHeading: 'By enabling IPFS in Hns, your NFTs will be pinned automatically. It\'s the best way to securely back up your NFTs.',
  hnsWalletNftPinningPinNftsButton: 'Get started with IPFS',
  hnsWalletNftPinningBackButton: 'Back',
  hnsWalletNftPinningCloseButton: 'Close',
  hnsWalletNftPinningHeading: 'The safest way to host NFTs',
  hnsWalletNftPinningRunNodeHeading: 'Enable IPFS in Hns to automatically back up your NFTs',
  hnsWalletNftPinningRunNodeDescription: 'IPFS is a community-driven storage network, like a hard drive that everyone can use. But instead of being controlled by one authority, thousands of individuals work together to host content on IPFS. When you “pin” something to IPFS, you’re ensuring that at least one copy of that content is safely stored. And as long as one person has a copy, a file can never disappear.$1By enabling IPFS in Hns, your NFTs will be pinned automatically. It\'s the best way to securely back up your NFTs.',
  hnsWalletNftPinningCheckNftsButton: 'See which of my NFTs are eligible',
  hnsWalletNftPinningBannerStart: 'Enable IPFS in Hns to automatically back up your NFTs for extra security.',
  hnsWalletNftPinningBannerUploading: 'NFTs are being pinned to your local IPFS node.',
  hnsWalletNftPinningBannerSuccess: '$1 supported NFTs are pinned to your local IPFS node.',
  hnsWalletNftPinningBannerLearnMore: 'Learn more',
  hnsWalletNftPinningInspectHeading: '$1 NFT is eligible',
  hnsWalletNftPinningInspectHeadingPlural: '$1 NFTs are eligible',
  hnsWalletNftPinningUnableToPin: 'Unable to pin',
  hnsWalletNftPinningNodeRunningStatus: 'You\’re running an IPFS node',
  hnsWalletNftPinningNodeNotRunningStatus: 'Local IPFS node is not running',
  hnsWalletNftPinningStatusPinned: 'Pinned to your local IPFS node.',
  hnsWalletNftPinningStatusPinning: 'NFT data is being pinned to your local IPFS node.',
  hnsWalletNftPinningStatusPinningFailed: 'Cannot be pinned to your local IPFS node.',
  hnsWalletNftPinningErrorTooltipHeading: 'Most common reasons:',
  hnsWalletNftPinningErrorTooltipReasonOne: 'NFT has non-IPFS metadata url problems',
  hnsWalletNftPinningErrorTooltipReasonTwo: 'Internal IPFS node problems',
  hnsWalletNftPinningErrorTooltipReasonThree: 'Not enough space on local node',
  hnsWalletImportNftModalTitle: 'Import NFT',
  hnsWalletEditNftModalTitle: 'Edit NFT',
  hnsWalletNftMoveToSpam: 'Mark as junk',
  hnsWalletNftUnspam: 'Mark as not junk',
  hnsWalletNftJunk: 'Junk',

  // Remove NFT modal
  hnsWalletRemoveNftModalHeader: 'Remove from Hns Wallet?',
  hnsWalletRemoveNftModalDescription: 'NFT will be removed from Hns Wallet but will remain on the blockchain. If you remove it, then change your mind, you\'ll need to import it again manually.',
  hnsWalletRemoveNftModalCancel: 'Cancel',
  hnsWalletRemoveNftModalConfirm: 'Remove',

  // NFT auto discovery modal
  hnsWalletEnableNftAutoDiscoveryModalHeader: 'Want your NFTs displayed automatically?',
  hnsWalletEnableNftAutoDiscoveryModalDescription: 'Hns Wallet can use a third-party service to automatically display your NFTs. Hns will share your wallet addresses with $1SimpleHash$2 to provide this service. $3Learn more.$4',
  hnsWalletEnableNftAutoDiscoveryModalConfirm: 'Yes, proceed',
  hnsWalletEnableNftAutoDiscoveryModalCancel: 'No thanks, I\'ll do it manually',
  hnsWalletAutoDiscoveryEmptyStateHeading: 'No NFTs to display',
  hnsWalletAutoDiscoveryEmptyStateSubHeading: 'Once an NFT is detected, it\’ll be displayed here.',
  hnsWalletAutoDiscoveryEmptyStateFooter: 'Can\’t see your NFTs?',
  hnsWalletAutoDiscoveryEmptyStateActions: '$1Refresh$2 or $3Import Manually$4',
  hnsWalletAutoDiscoveryEmptyStateRefresh: 'Refreshing'
})
