// Copyright (c) 2022 The Hns Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at https://mozilla.org/MPL/2.0/.

import { getLocale } from '../../common/locale'
import { unbiasedRandom } from './random-utils'

export const ORDINALS = {
  0: getLocale('hnsWalletOrdinalFirst'),
  1: getLocale('hnsWalletOrdinalSecond'),
  2: getLocale('hnsWalletOrdinalThird'),
  3: getLocale('hnsWalletOrdinalFourth'),
  4: getLocale('hnsWalletOrdinalFifth'),
  5: getLocale('hnsWalletOrdinalSixth'),
  6: getLocale('hnsWalletOrdinalSeventh'),
  7: getLocale('hnsWalletOrdinalEighth'),
  8: getLocale('hnsWalletOrdinalNinth'),
  9: getLocale('hnsWalletOrdinalTenth'),
  10: getLocale('hnsWalletOrdinalEleventh'),
  11: getLocale('hnsWalletOrdinalTwelfth'),
  12: getLocale('hnsWalletOridinalThirteenth'),
  13: getLocale('hnsWalletOrdinalFourteenth'),
  14: getLocale('hnsWalletOrdinalFifteenth'),
  15: getLocale('hnsWalletOrdinalSixteenth'),
  16: getLocale('hnsWalletOrdinalSeventeenth'),
  17: getLocale('hnsWalletOrdinalEighteenth'),
  18: getLocale('hnsWalletOrdinalNineteenth'),
  19: getLocale('hnsWalletOrdinalTwentieth'),
  20: getLocale('hnsWalletOrdinalTwentyFirst'),
  21: getLocale('hnsWalletOrdinalTwentySecond'),
  22: getLocale('hnsWalletOrdinalTwentyThird'),
  23: getLocale('hnsWalletOrdinalTwentyFourth')
}

const suffixes = new Map([
  ['one', getLocale('hnsWalletOrdinalSuffixOne')],
  ['two', getLocale('hnsWalletOrdinalSuffixTwo')],
  ['few', getLocale('hnsWalletOrdinalSuffixFew')],
  ['other', getLocale('hnsWalletOrdinalSuffixOther')]
])

export const formatOrdinals = (n: number) => {
  const pr = new Intl.PluralRules(navigator.language, { type: 'ordinal' })
  const rule = pr.select(n)
  const suffix = suffixes.get(rule)
  return `${n}${suffix}`
}

export const getWordIndicesToVerfy = (_wordsLength: number): number[] => {
  if (_wordsLength < 3) {
    return [-3, -2, -1] // phrase is not long enough (must be longer than 3 words)
  }

  // limit randomness to first 24 words
  const wordsLength = _wordsLength > 24 ? 24 : _wordsLength

  // get next random index
  const indicesSet = new Set<number>([])

  while (indicesSet.size < 3) {
    const nextIndex = unbiasedRandom(0, wordsLength - 1)
    indicesSet.add(nextIndex)
  }

  return Array.from(indicesSet).sort((a, b) => a - b) // verify in order
}
