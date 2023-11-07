// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { HnsWallet } from '../constants/types'
import Amount from './amount'

interface EVMStateChanges {
  erc20Approvals: HnsWallet.BlowfishERC20ApprovalData[]
  erc721Approvals: HnsWallet.BlowfishERC721ApprovalData[]
  erc721ApprovalsForAll: HnsWallet.BlowfishERC721ApprovalForAllData[]
  erc1155ApprovalsForAll: HnsWallet.BlowfishERC1155ApprovalForAllData[]

  inboundErc1155Transfers: HnsWallet.BlowfishERC1155TransferData[]
  inboundErc20Transfers: HnsWallet.BlowfishERC20TransferData[]
  inboundErc721Transfers: HnsWallet.BlowfishERC721TransferData[]
  inboundNativeAssetTransfers: HnsWallet.BlowfishNativeAssetTransferData[]

  outboundErc1155Transfers: HnsWallet.BlowfishERC1155TransferData[]
  outboundErc20Transfers: HnsWallet.BlowfishERC20TransferData[]
  outboundErc721Transfers: HnsWallet.BlowfishERC721TransferData[]
  outboundNativeAssetTransfers: HnsWallet.BlowfishNativeAssetTransferData[]
}

interface SVMStateChanges {
  solStakeAuthorityChanges: HnsWallet.BlowfishSOLStakeAuthorityChangeData[]
  splApprovals: HnsWallet.BlowfishSPLApprovalData[]
  inboundNativeAssetTransfers: HnsWallet.BlowfishSOLTransferData[]
  outboundNativeAssetTransfers: HnsWallet.BlowfishSOLTransferData[]
  inboundSplTransfers: HnsWallet.BlowfishSPLTransferData[]
  outboundSplTransfers: HnsWallet.BlowfishSPLTransferData[]
}

type DiffSign = 'PLUS' | 'MINUS'

const isInboundEVMTransfer = (transferData: {
  amount: HnsWallet.BlowfishEVMAmount
}) => {
  const { amount } = transferData
  if (new Amount(amount.after).gt(amount.before)) {
    return true
  }

  // outbound
  return false
}

const isInboundSVMTransfer = (transferData: {
  diff: HnsWallet.BlowfishSolanaDiff
}) => {
  const { diff } = transferData

  return diff.sign as DiffSign === 'PLUS'
}

export const decodeSimulatedEVMStateChanges = (
  evmStateChanges: HnsWallet.BlowfishEVMStateChange[]
): EVMStateChanges => {
  const changes: EVMStateChanges = {
    erc1155ApprovalsForAll: [],
    erc20Approvals: [],
    erc721Approvals: [],
    erc721ApprovalsForAll: [],
    inboundErc1155Transfers: [],
    inboundErc20Transfers: [],
    inboundErc721Transfers: [],
    inboundNativeAssetTransfers: [],
    outboundErc1155Transfers: [],
    outboundErc20Transfers: [],
    outboundErc721Transfers: [],
    outboundNativeAssetTransfers: []
  }

  for (const { rawInfo } of evmStateChanges) {
    const { data } = rawInfo

    // approvals
    if (data.erc20ApprovalData) {
      changes.erc20Approvals.push(data.erc20ApprovalData)
    }

    if (data.erc721ApprovalData) {
      changes.erc721Approvals.push(data.erc721ApprovalData)
    }

    // approvals for all
    if (data.erc721ApprovalForAllData) {
      changes.erc721ApprovalsForAll.push(data.erc721ApprovalForAllData)
    }

    if (data.erc1155ApprovalForAllData) {
      changes.erc1155ApprovalsForAll.push(data.erc1155ApprovalForAllData)
    }

    // transfers
    if (data.erc20TransferData) {
      if (isInboundEVMTransfer(data.erc20TransferData)) {
        changes.inboundErc20Transfers.push(data.erc20TransferData)
      }

      // outbound
      changes.outboundErc20Transfers.push(data.erc20TransferData)
    }

    if (data.erc721TransferData) {
      if (data.erc721TransferData) {
        changes.inboundErc721Transfers.push(data.erc721TransferData)
      }

      // outbound
      changes.outboundErc721Transfers.push(data.erc721TransferData)
    }

    if (data.erc1155TransferData) {
      if (isInboundEVMTransfer(data.erc1155TransferData)) {
        changes.inboundErc1155Transfers.push(data.erc1155TransferData)
      }

      // outbound
      changes.outboundErc1155Transfers.push(data.erc1155TransferData)
    }

    if (data.nativeAssetTransferData) {
      if (isInboundEVMTransfer(data.nativeAssetTransferData)) {
        changes.inboundNativeAssetTransfers.push(data.nativeAssetTransferData)
      }

      // outbound
      changes.outboundNativeAssetTransfers.push(data.nativeAssetTransferData)
    }
  }

  return changes
}

export const decodeSimulatedSVMStateChanges = (
  stateChanges: HnsWallet.BlowfishSolanaStateChange[]
): SVMStateChanges => {

  const changes: SVMStateChanges = {
    solStakeAuthorityChanges: [],
    splApprovals: [],
    inboundNativeAssetTransfers: [],
    inboundSplTransfers: [],
    outboundNativeAssetTransfers: [],
    outboundSplTransfers: []
  }

  for (const { rawInfo } of stateChanges) {
    const { data } = rawInfo

    // staking auth changes
    if (data.solStakeAuthorityChangeData) {
      changes.solStakeAuthorityChanges.push(data.solStakeAuthorityChangeData)
    }

    // approvals
    if (data.splApprovalData) {
      changes.splApprovals.push(data.splApprovalData)
    }

    // transfers
    if (data.solTransferData) {
      if (isInboundSVMTransfer(data.solTransferData)) {
        changes.inboundNativeAssetTransfers.push(data.solTransferData)
      }
      changes.outboundNativeAssetTransfers.push(data.solTransferData)
    }

    if (data.splTransferData) {
      if (isInboundSVMTransfer(data.splTransferData)) {
        changes.inboundSplTransfers.push(data.splTransferData)
      }
      changes.outboundSplTransfers.push(data.splTransferData)
    }
  }

  return changes
}
