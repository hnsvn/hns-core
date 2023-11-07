// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'
import Button from '@hns/leo/react/button'
import { useLocation } from 'react-router-dom'

// types
import {
  HnsWallet,
  WalletState,
  WalletRoutes
} from '../../../../constants/types'

// options
import { AllNetworksOption } from '../../../../options/network-filter-options'
import {
  EditVisibleAssetsOptions
} from '../../../../options/nav-options'
import { makeNetworkAsset } from '../../../../options/asset-options'

// utils
import { getLocale } from '../../../../../common/locale'
import { checkIfTokensMatch } from '../../../../utils/asset-utils'

// components
import {
  PopupModal //
} from '../../popup-modals/index'
import { VirtualizedVisibleAssetsList } from './virtualized-visible-assets-list'
import { AddAsset } from '../../add-asset/add-asset'
import {
  SegmentedControl
} from '../../../shared/segmented-control/segmented-control'
import {
  SearchBar
} from '../../../shared/search-bar'
import {
  NetworkFilterSelector
} from '../../network-filter-selector'

// Styled Components
import {
  LoadIcon,
  LoadingWrapper,
  NoAssetButton,
  StyledWrapper,
  ButtonRow,
  ListWrapper,
  AddButtonText,
  AddIcon,
  InfoIcon,
  EmptyStateWrapper,
  TitleRow
} from './style'
import {
  Column,
  HorizontalSpace,
  Row,
  Text,
  VerticalSpace
} from '../../../shared/style'
import { PaddedRow } from '../style'

// hooks
import { useAssetManagement } from '../../../../common/hooks/assets-management'
import { useSelector } from 'react-redux'
import {
  useGetSelectedChainQuery,
  useGetTokensRegistryQuery
} from '../../../../common/slices/api.slice'
import {
  getEntitiesListFromEntityState
} from '../../../../utils/entities.utils'
import {
  blockchainTokenEntityAdaptorInitialState
} from '../../../../common/slices/entities/blockchain-token.entity'

export interface Props {
  onClose: () => void
}

export const EditVisibleAssetsModal = ({ onClose }: Props) => {
  // routing
  const { hash } = useLocation()

  // redux
  const userVisibleTokensInfo = useSelector(({ wallet }: { wallet: WalletState }) => wallet.userVisibleTokensInfo)

  // queries
  const { data: selectedNetwork } = useGetSelectedChainQuery()
  const {
    data: tokenEntityState =
    blockchainTokenEntityAdaptorInitialState,
    isLoading
  } = useGetTokensRegistryQuery()

  // custom hooks
  const {
    onUpdateVisibleAssets
  } = useAssetManagement()

  // Token List States
  const [searchValue, setSearchValue] = React.useState<string>('')
  const [updatedTokensList, setUpdatedTokensList] = React.useState<HnsWallet.BlockchainToken[]>([])
  const [removedTokensList, setRemovedTokensList] = React.useState<HnsWallet.BlockchainToken[]>([])
  const [tokenContractAddress, setTokenContractAddress] = React.useState<string>('')
  const [selectedNetworkFilter, setSelectedNetworkFilter] = React.useState<HnsWallet.NetworkInfo>(AllNetworksOption)

  // Modal UI States
  const [showAddCustomToken, setShowAddCustomToken] = React.useState<boolean>(false)

  // If a user removes all of their assets from the userVisibleTokenInfo list,
  // there is a check in the async/lib.ts folder that will still return the networks
  // native asset (example 'ETH') with the value of visible: false to prevent breaking
  // other parts of the wallet.

  // This method here is used to determine if that is the case
  // and allows us to handle our true visible lists.
  const isUserVisibleTokensInfoEmpty = React.useMemo((): boolean => {
    return userVisibleTokensInfo.length === 1 &&
      userVisibleTokensInfo[0].contractAddress === '' &&
      !userVisibleTokensInfo[0].visible
  }, [userVisibleTokensInfo])

  React.useEffect(() => {
    if (isUserVisibleTokensInfoEmpty) {
      return
    }
    setUpdatedTokensList(userVisibleTokensInfo)
  }, [userVisibleTokensInfo])

  // Memos
  const nativeAsset = React.useMemo(() => {
    return selectedNetwork && makeNetworkAsset(selectedNetwork)
  }, [selectedNetwork])

  const fullTokenListAllChains = React.useMemo(() => {
    return getEntitiesListFromEntityState(
      tokenEntityState,
      tokenEntityState.ids
    )
  }, [tokenEntityState])

  // Token list based on selectedNetworkFilter
  const tokenListForSelectedNetworks: HnsWallet.BlockchainToken[] =
    React.useMemo(() => {
      if (
        selectedNetworkFilter.chainId ===
        AllNetworksOption.chainId
      ) {
        return fullTokenListAllChains
      }

      const tokenIds =
        tokenEntityState
          .idsByChainId[selectedNetworkFilter.chainId]
      return getEntitiesListFromEntityState(tokenEntityState, tokenIds)
    }, [
      selectedNetworkFilter.chainId,
      tokenEntityState,
      fullTokenListAllChains
    ])

  // User tokens sorted by visibility
  const usersTokensSortedByVisibility
    : HnsWallet.BlockchainToken[] =
    React.useMemo(() => {
      return [...userVisibleTokensInfo]
        .sort((a, b) =>
          Number(b.visible) - Number(a.visible)
        )
    }, [userVisibleTokensInfo])

  // Users visible tokens based on selectedNetworkFilter
  const userVisibleTokensBySelectedNetwork
    : HnsWallet.BlockchainToken[] =
    React.useMemo(() => {
      if (
        selectedNetworkFilter.chainId ===
        AllNetworksOption.chainId
      ) {
        return usersTokensSortedByVisibility
      }
      return usersTokensSortedByVisibility
        .filter(
          (token) =>
            token.chainId ===
            selectedNetworkFilter.chainId
        )
    }, [
      usersTokensSortedByVisibility,
      selectedNetworkFilter.chainId
    ])

  // Constructed list based on Users Visible Tokens and Full Token List
  const tokenList: HnsWallet.BlockchainToken[] = React.useMemo(() => {
    const userVisibleContracts = isUserVisibleTokensInfoEmpty
      ? []
      : userVisibleTokensBySelectedNetwork.map((token) => token.contractAddress.toLowerCase())

    const fullAssetsListPlusNativeToken = userVisibleContracts.includes('') || !nativeAsset
      ? tokenListForSelectedNetworks
      : [nativeAsset, ...tokenListForSelectedNetworks]

    const filteredTokenRegistry = fullAssetsListPlusNativeToken
      .filter((token) => !userVisibleContracts.includes(token?.contractAddress?.toLowerCase()))

    return isUserVisibleTokensInfoEmpty
      ? filteredTokenRegistry
      : [...userVisibleTokensBySelectedNetwork, ...filteredTokenRegistry]
  }, [
    isUserVisibleTokensInfoEmpty,
    tokenListForSelectedNetworks,
    userVisibleTokensInfo,
    nativeAsset,
    userVisibleTokensBySelectedNetwork
  ])

  // Filtered token list based on user removed tokens
  const filteredOutRemovedTokens = React.useMemo(() => {
    return tokenList.filter((token) =>
      !removedTokensList.some((t) =>
        t.contractAddress.toLowerCase() === token.contractAddress.toLowerCase() &&
        t.tokenId === token.tokenId))
  }, [tokenList, removedTokensList])

  // Filtered token list based on search value
  const filteredTokenList = React.useMemo(() => {
    if (searchValue === '') {
      return filteredOutRemovedTokens
    }
    return filteredOutRemovedTokens.filter((item) => {
      return (
        item.name.toLowerCase() === searchValue.toLowerCase() ||
        item.name.toLowerCase().startsWith(searchValue.toLowerCase()) ||
        item.symbol.toLocaleLowerCase() === searchValue.toLowerCase() ||
        item.symbol.toLowerCase().startsWith(searchValue.toLowerCase()) ||
        item.contractAddress.toLocaleLowerCase() === searchValue.toLowerCase()
      )
    })
  }, [filteredOutRemovedTokens, searchValue])

  const myAssetsOrAvailableAssets = React.useMemo(() => {
    return hash === WalletRoutes.AvailableAssetsHash
      ? filteredTokenList.filter((leftValue) =>
        !updatedTokensList.some(
          rightValue => checkIfTokensMatch(leftValue, rightValue)
        ))
      : filteredTokenList.filter((leftValue) =>
        updatedTokensList.some(
          rightValue => checkIfTokensMatch(leftValue, rightValue)
        ))
  }, [hash, filteredTokenList, updatedTokensList])

  // Methods
  const filterWatchlist = React.useCallback((event: React.ChangeEvent<HTMLInputElement>) => {
    setSearchValue(event.target.value)
  }, [tokenList])

  const findUpdatedTokenInfo = React.useCallback((token: HnsWallet.BlockchainToken) => {
    return updatedTokensList.find((t) => checkIfTokensMatch(t, token))
  }, [updatedTokensList])

  const isUserToken = React.useCallback((token: HnsWallet.BlockchainToken) => {
    return updatedTokensList.some(t => checkIfTokensMatch(t, token))
  }, [updatedTokensList])

  const isAssetSelected = React.useCallback((token: HnsWallet.BlockchainToken): boolean => {
    return (isUserToken(token) && findUpdatedTokenInfo(token)?.visible) ?? false
  }, [isUserToken, findUpdatedTokenInfo])

  const isRemovable = React.useCallback(
    (
      token: HnsWallet.BlockchainToken
    ): boolean => {
      // Native assets should not be removable.
      if (token.contractAddress === '') {
        return false
      }

      // Any NFT should be removable.
      if (
        token.isErc721 ||
        token.isErc1155 ||
        token.isNft
      ) {
        return true
      }

      return !fullTokenListAllChains
        .some(
          each =>
            each.contractAddress.toLowerCase() ===
            token.contractAddress.toLowerCase()
        )
    }, [fullTokenListAllChains])

  const onCheckWatchlistItem = React.useCallback(
    (
      selected: boolean,
      token: HnsWallet.BlockchainToken
    ) => {
      if (isUserToken(token)) {
        const updatedToken = { ...token, visible: selected }
        const tokenIndex =
          updatedTokensList
            .findIndex((t) => checkIfTokensMatch(t, token))
        let newList = [...updatedTokensList]
        newList.splice(tokenIndex, 1, updatedToken)
        setUpdatedTokensList(newList)
        return
      }
      setUpdatedTokensList([...updatedTokensList, token])
    }, [isUserToken, updatedTokensList])

  const toggleShowAddCustomToken = () => setShowAddCustomToken(prev => !prev)

  const onClickSuggestAdd = React.useCallback(() => {
    setTokenContractAddress(searchValue)
    toggleShowAddCustomToken()
  }, [searchValue, toggleShowAddCustomToken])

  const onRemoveAsset = React.useCallback((token: HnsWallet.BlockchainToken) => {
    const filterFn = (t: HnsWallet.BlockchainToken) => !(t.contractAddress.toLowerCase() === token.contractAddress.toLowerCase() && t.tokenId === token.tokenId)
    const newUserList = updatedTokensList.filter(filterFn)
    setUpdatedTokensList(newUserList)
    setRemovedTokensList([token, ...removedTokensList])
  }, [updatedTokensList, filteredTokenList, removedTokensList])

  const onClickDone = React.useCallback(() => {
    onUpdateVisibleAssets(updatedTokensList)
    onClose()
  }, [updatedTokensList, onUpdateVisibleAssets, onClose])

  const onSelectAssetsNetwork = React.useCallback((network: HnsWallet.NetworkInfo) => {
    setSelectedNetworkFilter(network)
  }, [])

  return (
    <PopupModal
      title={showAddCustomToken
        ? getLocale('hnsWalletWatchlistAddCustomAsset')
        : getLocale('hnsWalletAccountsEditVisibleAssets')
      }
      onClose={onClose}
      width='500px'
      borderRadius={16}
      height={
        showAddCustomToken
          ? 'unset'
          : '90vh'
      }
    >
      {!showAddCustomToken &&
        <>
          <PaddedRow
            margin='12px 0px'
            marginBottom={16}
          >
            <SegmentedControl
              navOptions={EditVisibleAssetsOptions}
            />
          </PaddedRow>
          <TitleRow
            marginBottom={8}
            justifyContent='space-between'
          >
            <Text
              textSize='16px'
              textColor='text03'
              isBold={true}
            >
              {
                hash === WalletRoutes.AvailableAssetsHash
                  ? getLocale('hnsWalletAvailableAssets')
                  : getLocale('hnsWalletMyAssets')
              }
            </Text>
            {hash === WalletRoutes.AvailableAssetsHash &&
              <NoAssetButton onClick={toggleShowAddCustomToken}>
                {getLocale('hnsWalletWatchlistAddCustomAsset')}
              </NoAssetButton>
            }
          </TitleRow>
          <PaddedRow
            marginBottom={8}
          >
            <SearchBar
              placeholder={getLocale('hnsWalletSearchText')}
              action={filterWatchlist}
              autoFocus={true}
              value={searchValue}
              isV2={true}
            />
            <HorizontalSpace space='16px' />
            <NetworkFilterSelector
              onSelectNetwork={onSelectAssetsNetwork}
              selectedNetwork={selectedNetworkFilter}
              isV2={true}
            />
          </PaddedRow>
        </>
      }
      {showAddCustomToken
        ? <AddAsset
          contractAddress={tokenContractAddress}
          onHideForm={toggleShowAddCustomToken}
        />
        : <StyledWrapper>
          {(myAssetsOrAvailableAssets.length === 0 && searchValue === '') ||
            isLoading ? (
            <LoadingWrapper>
              <LoadIcon />
            </LoadingWrapper>
          ) : (
            <>
              {myAssetsOrAvailableAssets.length === 0
                ? <EmptyStateWrapper>
                  <Column>
                    <InfoIcon />
                    <Text
                      textSize='16px'
                      textColor='text01'
                      isBold={true}
                    >
                      {getLocale('hnsWalletAssetNotFound')}
                    </Text>
                    <VerticalSpace space='12px' />
                    <Text
                      textSize='14px'
                      textColor='text03'
                      isBold={false}
                    >
                      {getLocale('hnsWalletDidntFindAssetInList')}
                    </Text>
                    <VerticalSpace space='16px' />
                    <Button
                      onClick={
                        searchValue.toLowerCase()
                          .startsWith('0x')
                          ? onClickSuggestAdd
                          : toggleShowAddCustomToken
                      }
                      kind='outline'
                    >
                      <Row
                        width='unset'
                      >
                        <AddIcon />
                        <AddButtonText>
                          {getLocale('hnsWalletWatchlistAddCustomAsset')}
                        </AddButtonText>
                      </Row>
                    </Button>
                  </Column>
                </EmptyStateWrapper>
                : <ListWrapper>
                  <VirtualizedVisibleAssetsList
                    tokenList={myAssetsOrAvailableAssets}
                    isRemovable={isRemovable}
                    onRemoveAsset={onRemoveAsset}
                    isAssetSelected={isAssetSelected}
                    onCheckWatchlistItem={onCheckWatchlistItem}
                    onClickAddCustomAsset={toggleShowAddCustomToken}
                  />
                </ListWrapper>
              }
            </>
          )}
        </StyledWrapper>
      }
      {!showAddCustomToken &&
        <ButtonRow>
          <Button
            onClick={onClose}
            kind='outline'
          >
            {getLocale('hnsWalletButtonCancel')}
          </Button>
          <HorizontalSpace space='16px' />
          <Button
            onClick={onClickDone}
          >
            {getLocale('hnsWalletButtonSaveChanges')}
          </Button>
        </ButtonRow>
      }
    </PopupModal>
  )
}

export default EditVisibleAssetsModal
