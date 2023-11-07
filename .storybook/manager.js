import { addons } from '@storybook/addons'
import { create } from '@storybook/theming'

const hnsTheme = create({
  base: 'dark',
  brandTitle: 'Hns Browser UI',
  brandUrl: 'https://github.com/hnsvn/hns-core'
})

addons.setConfig({
  isFullscreen: false,
  showNav: true,
  showPanel: true,
  panelPosition: 'right',
  theme: hnsTheme
})
