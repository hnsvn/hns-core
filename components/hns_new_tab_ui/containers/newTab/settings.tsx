// Copyright (c) 2020 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react'

import {
  SettingsMenu,
  SettingsTitle,
  SettingsWrapper,
  SettingsSidebar,
  SettingsFeatureBody,
  SettingsContent,
  SettingsCloseIcon,
  SettingsSidebarButton,
  SettingsSidebarActiveButtonSlider,
  SettingsSidebarButtonText,
  SettingsSidebarSVGContent
} from '../../components/default'

import { getLocale } from '../../../common/locale'
import { Publishers } from '../../api/hns_news'
import { HnsNewsContext } from '../../components/default/hnsNews/customize/Context'

// Icons
import { CloseStrokeIcon } from 'hns-ui/components/icons'
import BackgroundImageIcon from './settings/icons/backgroundImage.svg'
import HnsStatsIcon from './settings/icons/hnsStats.svg'
import TopSitesIcon from './settings/icons/topSites.svg'
import ClockIcon from './settings/icons/clock.svg'
import CardsIcon from './settings/icons/cards.svg'
import TodayIcon from './settings/icons/hnsNews.svg'

// Tabs
const BackgroundImageSettings = React.lazy(() => import('./settings/backgroundImage'))
const HnsStatsSettings = React.lazy(() => import('./settings/hnsStats'))
const TopSitesSettings = React.lazy(() => import('./settings/topSites'))
const ClockSettings = React.lazy(() => import('./settings/clock'))
const CardsSettings = React.lazy(() => import('./settings/cards'))

// Types
import { NewTabActions } from '../../constants/new_tab_types'

export interface Props {
  newTabData: NewTab.State
  actions: NewTabActions
  textDirection: string
  showSettingsMenu: boolean
  featureCustomBackgroundEnabled: boolean
  onClose: () => void
  onDisplayTodaySection: () => any
  onClearTodayPrefs: () => any
  toggleShowBackgroundImage: () => void
  toggleShowTopSites: () => void
  setMostVisitedSettings: (show: boolean, customize: boolean) => void
  toggleShowRewards: () => void
  toggleShowHnsTalk: () => void
  toggleBrandedWallpaperOptIn: () => void
  toggleCards: (show: boolean) => void
  chooseNewCustomImageBackground: () => void
  setCustomImageBackground: (selectedBackground: string) => void
  removeCustomImageBackground: (background: string) => void
  setHnsBackground: (selectedBackground: string) => void
  setColorBackground: (color: string, useRandomColor: boolean) => void
  onEnableRewards: () => void
  showBackgroundImage: boolean
  showTopSites: boolean
  customLinksEnabled: boolean
  brandedWallpaperOptIn: boolean
  allowBackgroundCustomization: boolean
  showRewards: boolean
  showHnsTalk: boolean
  hnsRewardsSupported: boolean
  hnsTalkSupported: boolean
  todayPublishers?: Publishers
  setActiveTab?: TabType
  cardsHidden: boolean
}

export enum TabType {
  BackgroundImage = 'backgroundImage',
  HnsStats = 'hnsStats',
  TopSites = 'topSites',
  HnsNews = 'hnsNews',
  Clock = 'clock',
  Cards = 'cards'
}

interface State {
  activeTab: TabType
}

export default class Settings extends React.PureComponent<Props, State> {
  static contextType: typeof HnsNewsContext = HnsNewsContext
  settingsMenuRef: React.RefObject<any>
  allTabTypes: TabType[]
  allTabTypesWithoutBackground: TabType[]

  constructor (props: Props) {
    super(props)
    // Cache allowed tabs array on instance.
    // Feature flags won't change during page lifecycle, so we don't need to
    // change this when props change.
    this.allTabTypes = [...Object.values(TabType)]
    this.allTabTypesWithoutBackground = [...this.allTabTypes]
    this.allTabTypesWithoutBackground.splice(
      this.allTabTypesWithoutBackground.indexOf(TabType.BackgroundImage), 1
    )
    this.settingsMenuRef = React.createRef()
    this.state = {
      activeTab: this.getInitialTab()
    }
  }

  handleClickOutside = (event: Event) => {
    if (
      this.settingsMenuRef &&
      this.settingsMenuRef.current &&
      !this.settingsMenuRef.current.contains(event.target) &&
      // Don't close the settings dialog for a click outside if we're in the
      // Hns News modal - the user expects closing that one to bring them back
      // to this one.
      !this.context.customizePage
    ) {
      this.props.onClose()
    }
  }

  componentDidMount () {
    document.addEventListener('mousedown', this.handleClickOutside)
    document.addEventListener('keydown', this.onKeyPressSettings)
  }

  componentWillUnmount () {
    document.removeEventListener('mousedown', this.handleClickOutside)
  }

  componentDidUpdate (prevProps: Props) {
    if (prevProps.setActiveTab !== this.props.setActiveTab && this.props.setActiveTab) {
      this.setActiveTab(this.props.setActiveTab)
    }
    const isNewlyShown = (!prevProps.showSettingsMenu && this.props.showSettingsMenu)
    if (isNewlyShown) {
      this.setActiveTab(this.getInitialTab())
    }
  }

  onKeyPressSettings = (event: KeyboardEvent) => {
    if (event.key === 'Escape') {
      this.props.onClose()
    }
  }

  getInitialTab () {
    let tab = this.props.allowBackgroundCustomization
      ? TabType.BackgroundImage
      : TabType.HnsStats
    if (this.props.setActiveTab) {
      if (this.getActiveTabTypes().includes(this.props.setActiveTab)) {
        tab = this.props.setActiveTab
      }
    }
    return tab
  }

  toggleShowBackgroundImage = () => {
    this.props.toggleShowBackgroundImage()
  }

  setHnsBackground = (selectedBackground: string) => {
    this.props.setHnsBackground(selectedBackground)
  }

  setColorBackground = (color: string, useRandomColor: boolean) => {
    this.props.setColorBackground(color, useRandomColor)
  }

  setActiveTab (activeTab: TabType) {
    if (activeTab === TabType.HnsNews) {
      this.context.setCustomizePage('news')
      return
    }

    this.setState({ activeTab })
  }

  getActiveTabTypes (): TabType[] {
    if (this.props.allowBackgroundCustomization) {
      return this.allTabTypes
    } else {
      return this.allTabTypesWithoutBackground
    }
  }

  getTabIcon (tab: TabType, isActiveTab: boolean) {
    let srcUrl
    switch (tab) {
      case TabType.BackgroundImage:
        srcUrl = BackgroundImageIcon
        break
      case TabType.HnsStats:
        srcUrl = HnsStatsIcon
        break
      case TabType.TopSites:
        srcUrl = TopSitesIcon
        break
      case TabType.HnsNews:
        srcUrl = TodayIcon
        break
      case TabType.Clock:
        srcUrl = ClockIcon
        break
      case TabType.Cards:
        srcUrl = CardsIcon
        break
      default:
        srcUrl = BackgroundImageIcon
        break
    }
    return <SettingsSidebarSVGContent isActive={isActiveTab} src={srcUrl} />
  }

  getTabTitleKey = (tab: TabType) => {
    switch (tab) {
      case TabType.BackgroundImage:
        return 'backgroundImageTitle'
      case TabType.HnsStats:
        return 'statsTitle'
      case TabType.TopSites:
        return 'topSitesTitle'
      case TabType.HnsNews:
        return 'hnsNewsTitle'
      case TabType.Clock:
        return 'clockTitle'
      case TabType.Cards:
        return 'cards'
      default:
        return ''
    }
  }

  render () {
    const {
      textDirection,
      showSettingsMenu,
      toggleShowTopSites,
      setMostVisitedSettings,
      toggleShowRewards,
      toggleShowHnsTalk,
      toggleBrandedWallpaperOptIn,
      showBackgroundImage,
      featureCustomBackgroundEnabled,
      showTopSites,
      customLinksEnabled,
      showRewards,
      showHnsTalk,
      brandedWallpaperOptIn,
      hnsRewardsSupported,
      hnsTalkSupported,
      toggleCards,
      cardsHidden,
      onEnableRewards
    } = this.props
    const { activeTab } = this.state

    if (!showSettingsMenu) {
      return null
    }

    const tabTypes = this.getActiveTabTypes()
    return (
      <SettingsWrapper textDirection={textDirection}>
        <SettingsMenu
          ref={this.settingsMenuRef}
          textDirection={textDirection}
          title={getLocale('dashboardSettingsTitle')}
        >
          <SettingsTitle id='settingsTitle'>
            <h1>{getLocale('dashboardSettingsTitle')}</h1>
            <SettingsCloseIcon onClick={this.props.onClose}>
              <CloseStrokeIcon />
            </SettingsCloseIcon>
          </SettingsTitle>
          <SettingsContent id='settingsBody'>
            <SettingsSidebar id='sidebar'>
              <SettingsSidebarActiveButtonSlider
                translateTo={tabTypes.indexOf(activeTab)}
              />
              {
                tabTypes.map((tabType, index) => {
                  const titleKey = this.getTabTitleKey(tabType)
                  const isActive = (activeTab === tabType)
                  return (
                    <SettingsSidebarButton
                      tabIndex={0}
                      key={`sidebar-button-${index}`}
                      activeTab={isActive}
                      onClick={this.setActiveTab.bind(this, tabType)}
                    >
                      {this.getTabIcon(tabType, isActive)}
                      <SettingsSidebarButtonText
                        isActive={isActive}
                        data-text={getLocale(titleKey)}
                      >
                        {getLocale(titleKey)}
                      </SettingsSidebarButtonText>
                    </SettingsSidebarButton>
                  )
                })
              }
            </SettingsSidebar>
            <SettingsFeatureBody id='content'>
              {/* Empty loading fallback is ok here since we are loading from local disk. */}
              <React.Suspense fallback={(<div/>)}>
              {
                activeTab === TabType.BackgroundImage
                  ? (
                  <BackgroundImageSettings
                    newTabData={this.props.newTabData}
                    toggleBrandedWallpaperOptIn={toggleBrandedWallpaperOptIn}
                    toggleShowBackgroundImage={this.toggleShowBackgroundImage}
                    chooseNewCustomImageBackground={this.props.chooseNewCustomImageBackground}
                    setCustomImageBackground={this.props.setCustomImageBackground}
                    removeCustomImageBackground={this.props.removeCustomImageBackground}
                    setHnsBackground={this.setHnsBackground}
                    setColorBackground={this.setColorBackground}
                    brandedWallpaperOptIn={brandedWallpaperOptIn}
                    showBackgroundImage={showBackgroundImage}
                    featureCustomBackgroundEnabled={featureCustomBackgroundEnabled}
                    onEnableRewards={onEnableRewards}
                    hnsRewardsSupported={hnsRewardsSupported}
                  />
                ) : null
              }
              {activeTab === TabType.HnsStats && <HnsStatsSettings />}
              {
                activeTab === TabType.TopSites
                  ? (
                    <TopSitesSettings
                      toggleShowTopSites={toggleShowTopSites}
                      showTopSites={showTopSites}
                      customLinksEnabled={customLinksEnabled}
                      setMostVisitedSettings={setMostVisitedSettings}
                    />
                  ) : null
              }
              {activeTab === TabType.Clock && <ClockSettings />}
              {
                activeTab === TabType.Cards
                  ? (
                    <CardsSettings
                      toggleCards={toggleCards}
                      cardsHidden={cardsHidden}
                      toggleShowHnsTalk={toggleShowHnsTalk}
                      showHnsTalk={showHnsTalk}
                      hnsTalkSupported={hnsTalkSupported}
                      toggleShowRewards={toggleShowRewards}
                      hnsRewardsSupported={hnsRewardsSupported}
                      showRewards={showRewards}
                    />
                  ) : null
              }
              </React.Suspense>
            </SettingsFeatureBody>
          </SettingsContent>
        </SettingsMenu>
      </SettingsWrapper>
    )
  }
}
