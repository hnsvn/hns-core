// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

// Types
import {
  NavOption,
  WalletRoutes,
  AccountPageTabs
} from '../constants/types'

export const BuySendSwapDepositOptions: NavOption[] = [
  {
    id: 'buy',
    name: 'hnsWalletBuy',
    icon: 'coins-alt1',
    route: WalletRoutes.FundWalletPageStart
  },
  {
    id: 'send',
    name: 'hnsWalletSend',
    icon: 'send',
    route: WalletRoutes.SendPageStart
  },
  {
    id: 'swap',
    name: 'hnsWalletSwap',
    icon: 'currency-exchange',
    route: WalletRoutes.Swap
  },
  {
    id: 'deposit',
    name: 'hnsWalletDepositCryptoButton',
    icon: 'money-bag-coins',
    route: WalletRoutes.DepositFundsPageStart
  }
]

const ActivityNavOption: NavOption = {
  id: 'activity',
  name: 'hnsWalletActivity',
  icon: 'activity',
  route: WalletRoutes.Activity
}

// We can remove this once we go live with Panel 2.0
export const PanelNavOptionsOld: NavOption[] = [
  ...BuySendSwapDepositOptions,
  ActivityNavOption
]

export const PanelNavOptions: NavOption[] = [
  {
    id: 'portfolio',
    name: 'hnsWalletTopNavPortfolio',
    icon: 'coins',
    route: WalletRoutes.Portfolio
  },
  ActivityNavOption,
  {
    id: 'accounts',
    name: 'hnsWalletTopNavAccounts',
    icon: 'user-accounts',
    route: WalletRoutes.Accounts
  },
  {
    id: 'market',
    name: 'hnsWalletTopNavMarket',
    icon: 'discover',
    route: WalletRoutes.Market
  }
]

export const NavOptions: NavOption[] = [
  {
    id: 'portfolio',
    name: 'hnsWalletTopNavPortfolio',
    icon: 'coins',
    route: WalletRoutes.Portfolio
  },
  ActivityNavOption,
  {
    id: 'accounts',
    name: 'hnsWalletTopNavAccounts',
    icon: 'user-accounts',
    route: WalletRoutes.Accounts
  },
  {
    id: 'market',
    name: 'hnsWalletTopNavMarket',
    icon: 'discover',
    route: WalletRoutes.Market
  }
]

export const AllNavOptions: NavOption[] = [
  ...NavOptions,
  ...BuySendSwapDepositOptions,
  ActivityNavOption
]

export const PortfolioNavOptions: NavOption[] = [
  {
    id: 'assets',
    name: 'hnsWalletAccountsAssets',
    icon: 'coins',
    route: WalletRoutes.PortfolioAssets
  },
  {
    id: 'nfts',
    name: 'hnsWalletTopNavNFTS',
    icon: 'grid04',
    route: WalletRoutes.PortfolioNFTs
  },
]

export const PortfolioAssetOptions: NavOption[] = [
  {
    id: 'accounts',
    name: 'hnsWalletTopNavAccounts',
    icon: 'user-accounts',
    route: WalletRoutes.AccountsHash
  },
  {
    id: 'transactions',
    name: 'hnsWalletTransactions',
    icon: 'activity',
    route: WalletRoutes.TransactionsHash
  }
]

export const EditVisibleAssetsOptions: NavOption[] = [
  {
    id: 'my_assets',
    name: 'hnsWalletMyAssets',
    icon: '',
    route: WalletRoutes.MyAssetsHash
  },
  {
    id: 'available_assets',
    name: 'hnsWalletAvailableAssets',
    icon: '',
    route: WalletRoutes.AvailableAssetsHash
  }
]

export const CreateAccountOptions: NavOption[] = [
  {
    id: 'accounts',
    name: 'hnsWalletCreateAccountButton',
    icon: 'plus-add',
    route: WalletRoutes.CreateAccountModalStart
  },
  {
    id: 'accounts',
    name: 'hnsWalletImportAccount',
    icon: 'product-hns-wallet',
    route: WalletRoutes.ImportAccountModalStart
  },
  {
    id: 'accounts',
    name: 'hnsWalletConnectHardwareWallet',
    icon: 'flashdrive',
    route: WalletRoutes.AddHardwareAccountModalStart
  },
]

export const AccountDetailsOptions: NavOption[] = [
  {
    id: 'assets',
    name: 'hnsWalletAccountsAssets',
    icon: '',
    route: AccountPageTabs.AccountAssetsSub
  },
  {
    id: 'nfts',
    name: 'hnsWalletTopNavNFTS',
    icon: '',
    route: AccountPageTabs.AccountNFTsSub
  },
  {
    id: 'transactions',
    name: 'hnsWalletTransactions',
    icon: '',
    route: AccountPageTabs.AccountTransactionsSub
  },
]
