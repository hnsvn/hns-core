// Copyright (c) 2023 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// You can obtain one at https://mozilla.org/MPL/2.0/.

import * as React from 'react';
import { useInspectContext } from './context';
import Card from './feed/Card';
import Radio from '@hns/leo/react/radioButton'
import Dropdown from '@hns/leo/react/dropdown';
import Input from '@hns/leo/react/input';
import Flex from '../../../hns_new_tab_ui/components/Flex'
import { Channel, Publisher } from 'gen/hns/components/hns_news/common/hns_news.mojom.m';
import styled from 'styled-components';

interface Props {
}

const Container = styled(Flex)`
  width: 800px;
`

function SignalCards<T>({ items, sort, filter, getName, getKey }: { items: T[], sort: 'name' | 'subscribed' | 'visitWeight', filter: string, getName: (item: T) => string, getKey: (item: T) => string }) {
  const { signals } = useInspectContext()

  const filteredAndSorted = items
    .filter(item => getName(item).toLowerCase().includes(filter))
    .sort((a, b) => {
      if (sort === 'name') return getName(a).localeCompare(getName(b))

      const aSignal = signals[getKey(a)]
      const bSignal = signals[getKey(b)]

      if (sort === 'subscribed') return bSignal.subscribedWeight - aSignal.subscribedWeight
      return bSignal.visitWeight - aSignal.visitWeight
    })
  return <>
    {filteredAndSorted.map(a => <Card>
      <b>{getName(a)}</b>
      <br />
      <b>Subscribed Weighting:</b> {signals[getKey(a)]?.subscribedWeight.toString()}
      <br />
      <b>Visit Weighting:</b> {signals[getKey(a)]?.visitWeight}
    </Card>)}
  </>
}

const getChannelKey = (channel: Channel) => channel.channelName
const getPublisherKey = (p: Publisher) => p.publisherId
const getPublisherName = (p: Publisher) => p.publisherName

export default function SignalsPage(props: Props) {
  const { publishers, channels } = useInspectContext();
  const [show, setShow] = React.useState<'all' | 'publishers' | 'channels'>('all')
  const [sort, setSort] = React.useState<'name' | 'subscribed' | 'visitWeight'>('visitWeight')
  const [filter, setFilter] = React.useState('')

  return <Container direction='column'>
    <h2>Signals Page</h2>
    <Flex direction='row' gap={8}>
      <Flex direction='column' gap={8}>
        Show only:
        <Radio name='show' value="all" currentValue={show} onChange={e => setShow(e.detail.value)} />
        <Radio name='show' value="publishers" currentValue={show} onChange={e => setShow(e.detail.value)} />
        <Radio name='show' value="channels" currentValue={show} onChange={e => setShow(e.detail.value)} />
      </Flex>
      <Input size='normal' hasErrors={false} showErrors={false} mode='outline' type='text' placeholder='filter...' value={filter} onInput={e => setFilter((e.target as any).value)}>
        Filter
      </Input>
      <Dropdown value={sort} onChange={e => setSort(e.detail.value)}>
        <div slot="label">Sort by</div>
        <leo-option>name</leo-option>
        <leo-option>subscribed</leo-option>
        <leo-option>visitWeight</leo-option>
      </Dropdown>
    </Flex>

    <Flex direction='column' gap={8}>
      {show !== 'channels' && <>
        <b>Publishers</b>
        <SignalCards filter={filter} getKey={getPublisherKey} getName={getPublisherName} items={Object.values(publishers)} sort={sort} />
      </>}
      {show !== 'publishers' && <>
        <b>Channels</b>
        <SignalCards filter={filter} getKey={getChannelKey} getName={getChannelKey} items={Object.values(channels)} sort={sort} />
      </>}
    </Flex>
  </Container>
}
