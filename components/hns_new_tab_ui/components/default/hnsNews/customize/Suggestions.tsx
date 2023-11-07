// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import { getLocale } from '$web-common/locale'
import * as React from 'react'
import Flex from '../../../Flex'
import Carousel from './Carousel'
import { useHnsNews } from './Context'
import CustomizeLink from './CustomizeLink'
import CustomizePage from './CustomizePage'
import DiscoverSection from './DiscoverSection'
import FeedCard from './FeedCard'

export function SuggestionsCarousel () {
  const { suggestedPublisherIds, setCustomizePage } = useHnsNews()

  return <Carousel
    title={<Flex justify='space-between'>
      {getLocale('hnsNewsSuggestionsTitle')}
      <CustomizeLink onClick={() => setCustomizePage('suggestions')}>
        {getLocale('hnsNewsViewAllButton')}
      </CustomizeLink>
    </Flex>}
    subtitle={getLocale('hnsNewsSuggestionsSubtitle')}
    publisherIds={suggestedPublisherIds}/>
}

export function SuggestionsPage () {
  const { suggestedPublisherIds } = useHnsNews()
  return <CustomizePage title={getLocale('hnsNewsSuggestionsTitle')}>
    <DiscoverSection>
      {suggestedPublisherIds.map(p => <FeedCard key={p} publisherId={p} />)}
    </DiscoverSection>
  </CustomizePage>
}
