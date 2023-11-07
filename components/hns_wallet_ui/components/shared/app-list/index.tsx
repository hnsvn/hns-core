// Copyright (c) 2021 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.
import * as React from 'react'
import { AppsListType, HnsWallet } from '../../../constants/types'
import { NavButton } from '../../extension/buttons/nav-button/index'
import { AppListItem } from '../app-list-item/index'
import { getLocale } from '../../../../common/locale'
// Styled Components
import {
  StyledWrapper,
  CategoryRow,
  CategoryTitle,
  ButtonRow
} from './style'

export interface Props {
  list: AppsListType[]
  favApps: HnsWallet.AppItem[]
  action: () => void
  addToFav: (app: HnsWallet.AppItem) => void
  removeFromFav: (app: HnsWallet.AppItem) => void
}

export class AppList extends React.PureComponent<Props> {
  checkIsSelected = (app: HnsWallet.AppItem) => {
    return this.props.favApps.some((a) => a.name === app.name)
  }

  toggleFavorite = (app: HnsWallet.AppItem) => () => {
    if (this.checkIsSelected(app)) {
      this.props.removeFromFav(app)
    } else {
      this.props.addToFav(app)
    }
  }

  render () {
    const { list, action } = this.props
    return (
      <StyledWrapper>
        {list.map((option) => (
          <StyledWrapper key={option.category}>
            <CategoryRow>
              <CategoryTitle>{option.category}</CategoryTitle>
            </CategoryRow>
            {option.appList.map((item) =>
              <AppListItem
                key={item.name}
                appInfo={item}
                isStared={this.checkIsSelected(item)}
                toggleFavorite={this.toggleFavorite(item)}
              />
            )}
            {option.category !== getLocale('hnsWalletSearchCategory') &&
              <ButtonRow>
                <NavButton
                  disabled={false}
                  text={option.categoryButtonText}
                  onSubmit={action}
                  buttonType='secondary'

                />
              </ButtonRow>
            }
          </StyledWrapper>
        )
        )}
      </StyledWrapper>
    )
  }
}

export default AppList