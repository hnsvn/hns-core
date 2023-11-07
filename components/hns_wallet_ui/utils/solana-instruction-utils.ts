// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { EntityState } from '@reduxjs/toolkit'

// types
import {
  HnsWallet,
  SerializableSolanaTxData
} from '../constants/types'

// utils
import { findAccountByAddress } from './account-utils'
import { getGetCleanedMojoEnumKeys } from './enum-utils'
import { lamportsToSol } from './web3-utils'

export const SolanaSystemInstructionKeys = getGetCleanedMojoEnumKeys(
  HnsWallet.SolanaSystemInstruction
)

export const SolanaTokenInstructionKeys = getGetCleanedMojoEnumKeys(
  HnsWallet.SolanaTokenInstruction
)

export type SolanaSystemInstructionType =
  typeof SolanaSystemInstructionKeys[number]

export type SolanaTokenInstructionType =
  typeof SolanaTokenInstructionKeys[number]

export type TypedSolanaInstructionWithParams = {
  accountMetas: HnsWallet.SolanaAccountMeta[]
  programId: string
  accountParams: HnsWallet.SolanaInstructionParam[]
  type?: SolanaSystemInstructionType | SolanaTokenInstructionType
  params: HnsWallet.SolanaInstructionParam[]
  data: number[]
}

export const getSolanaTransactionInstructionParamsAndType = ({
  programId,
  decodedData,
  accountMetas,
  data,
}: HnsWallet.SolanaInstruction): TypedSolanaInstructionWithParams => {
  // the signers are the `accountMetas` from this index to the end of the array
  // its possible to have any number of signers, including 0
  const accountParams: HnsWallet.SolanaInstructionParam[] = (
    decodedData?.accountParams || []
  ).map(({ localizedName, name }, i) => {
    const isSignersParam = name === HnsWallet.SIGNERS

    return {
      name,
      localizedName,
      type: isSignersParam
        ? HnsWallet.SolanaInstructionParamType.kString
        : HnsWallet.SolanaInstructionParamType.kPublicKey,
      // add a comma separated list of signers as a value if param name is "signers"
      value: isSignersParam
        ? accountMetas.slice(i).join(',')
        : accountMetas[i]?.pubkey
    }
  })

  const typedInstruction: TypedSolanaInstructionWithParams = {
    programId,
    accountParams: accountParams,
    params: decodedData?.params || [],
    type: undefined,
    accountMetas,
    data
  }

  if (!decodedData) {
    // return early if nothing to decode
    return typedInstruction
  }

  switch (programId) {
    case HnsWallet.SOLANA_SYSTEM_PROGRAM_ID: {
      typedInstruction.type =
        SolanaSystemInstructionKeys[decodedData.instructionType]
      break
    }
    case HnsWallet.SOLANA_TOKEN_PROGRAM_ID: {
      typedInstruction.type =
        SolanaTokenInstructionKeys[decodedData.instructionType]
      break
    }
  }

  return typedInstruction
}

export const getTypedSolanaTxInstructions = (solTxData?: SerializableSolanaTxData | HnsWallet.SolanaTxData): TypedSolanaInstructionWithParams[] => {
  const instructions: TypedSolanaInstructionWithParams[] = (solTxData?.instructions || []).map((instruction) => {
    return getSolanaTransactionInstructionParamsAndType(instruction)
  })
  return instructions || []
}

/**
 * formatted if possible:
 *
 * lamports -> SOL
 *
 * pubkey -> friendly account address name
 */
export const formatSolInstructionParamValue = (
  { name, value, type }: HnsWallet.SolanaInstructionParam,
  accounts: EntityState<HnsWallet.AccountInfo> | undefined
): {
  valueType: 'lamports' | 'address' | 'other'
  formattedValue: string
} => {
  const isAddressParam =
    type === HnsWallet.SolanaInstructionParamType.kOptionalPublicKey ||
    type === HnsWallet.SolanaInstructionParamType.kPublicKey

  const isLamportsParam = name === HnsWallet.LAMPORTS

  const formattedParamValue = (
    isLamportsParam
      ? lamportsToSol(value).formatAsAsset(9, 'SOL')
      : isAddressParam
      ? findAccountByAddress(value, accounts) ?? value
      : value
  ).toString()

  return {
    formattedValue: formattedParamValue,
    valueType: isAddressParam
      ? 'address'
      : isLamportsParam
      ? 'lamports'
      : 'other'
  }
}

export const getSolInstructionAccountParamsObj = (
  accountParams: HnsWallet.SolanaInstructionAccountParam[],
  accountMetas: HnsWallet.SolanaAccountMeta[]
) => {
  let fromAccount: string = ''
  let toAccount: string = ''
  let nonceAccount: string = ''
  let newAccount: string = ''

  accountParams.forEach(({ name }, i) => {
    // Do not show account public key of address table lookup account because
    // it might give the wrong impression to users. For example, users might
    // think they're sending funds to this address table lookup account, but
    // actually they're sending to the account pointed by the index in this
    // address table lookup account.
    const isAddrTableLookupAccount = accountMetas[i]?.addrTableLookupIndex
    const value = isAddrTableLookupAccount ? '' : accountMetas[i]?.pubkey

    switch (name) {
      case HnsWallet.FROM_ACCOUNT: {
        fromAccount = value
        break
      }
      case HnsWallet.TO_ACCOUNT: {
        toAccount = value
        break
      }
      case HnsWallet.NONCE_ACCOUNT: {
        nonceAccount = value
        break
      }
      case HnsWallet.NEW_ACCOUNT: {
        newAccount = value
        break
      }
      default: break
    }
  })

  return {
    fromAccount,
    toAccount,
    nonceAccount,
    newAccount
  }
}

export const getSolInstructionParamsObj = (
  params: HnsWallet.SolanaInstructionParam[]
) => {
  let lamports: string = '0'

  params.forEach(({ name, value }, i) => {
    switch (name) {
      case HnsWallet.LAMPORTS: {
        lamports = value ?? '0'
        break
      }
      default: break
    }
  })

  return {
    lamports
  }
}
