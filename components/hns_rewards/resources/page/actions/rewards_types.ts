/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

export const enum types {
  IS_INITIALIZED = '@@rewards/IS_INITIALIZED',
  ON_SETTING_SAVE = '@@rewards/ON_SETTING_SAVE',
  GET_IS_GRANDFATHERED_USER = '@@rewards/GET_IS_GRANDFATHERED_USER',
  ON_IS_GRANDFATHERED_USER = '@@rewards/ON_IS_GRANDFATHERED_USER',
  ON_USER_TYPE = '@@rewards/ON_USER_TYPE',
  GET_USER_TYPE = '@@rewards/GET_USER_TYPE',
  ON_REWARDS_PARAMETERS = '@@rewards/ON_REWARDS_PARAMETERS',
  GET_REWARDS_PARAMETERS = '@@rewards/GET_REWARDS_PARAMETERS',
  GET_IS_AUTO_CONTRIBUTE_SUPPORTED = '@@rewards/GET_IS_AUTO_CONTRIBUTE_SUPPORTED',
  ON_IS_AUTO_CONTRIBUTE_SUPPORTED = '@@rewards/ON_IS_AUTO_CONTRIBUTE_SUPPORTED',
  GET_AUTO_CONTRIBUTE_PROPERTIES = '@@rewards/GET_AUTO_CONTRIBUTE_PROPERTIES',
  ON_AUTO_CONTRIBUTE_PROPERTIES = '@@rewards/ON_AUTO_CONTRIBUTE_PROPERTIES',
  FETCH_PROMOTIONS = '@@rewards/FETCH_PROMOTIONS',
  ON_PROMOTIONS = '@@rewards/ON_PROMOTION',
  CLAIM_PROMOTION = '@@rewards/CLAIM_PROMOTION',
  DELETE_PROMOTION = '@@rewards/DELETE_PROMOTION',
  ON_PROMOTION_CLAIM_STARTED = '@@rewards/ON_PROMOTION_CLAIM_STARTED',
  ON_PROMOTION_FINISH = '@@rewards/ON_PROMOTION_FINISH',
  ON_MODAL_RESET_CLOSE = '@@rewards/ON_MODAL_RESET_CLOSE',
  ON_MODAL_RESET_OPEN = '@@rewards/ON_MODAL_RESET_OPEN',
  ON_MODAL_CONNECT_CLOSE = '@@rewards/ON_MODAL_CONNECT_CLOSE',
  ON_MODAL_CONNECT_OPEN = '@@rewards/ON_MODAL_CONNECT_OPEN',
  ON_MODAL_ADS_HISTORY_CLOSE = '@@rewards/ON_MODAL_ADS_HISTORY_CLOSE',
  ON_MODAL_ADS_HISTORY_OPEN = '@@rewards/ON_MODAL_ADS_HISTORY_OPEN',
  ON_ADS_SETTINGS_CLOSE = '@@rewards/ON_ADS_SETTINGS_CLOSE',
  ON_ADS_SETTINGS_OPEN = '@@rewards/ON_ADS_SETTINGS_OPEN',
  ON_AUTO_CONTRIBUTE_SETTINGS_CLOSE = '@@rewards/ON_AUTO_CONTRIBUTE_SETTINGS_CLOSE',
  ON_AUTO_CONTRIBUTE_SETTINGS_OPEN = '@@rewards/ON_AUTO_CONTRIBUTE_SETTINGS_OPEN',
  ON_CONTRIBUTIONS_SETTINGS_CLOSE = '@@rewards/ON_CONTRIBUTIONS_SETTINGS_CLOSE',
  ON_CONTRIBUTIONS_SETTINGS_OPEN = '@@rewards/ON_CONTRIBUTIONS_SETTINGS_OPEN',
  ON_CLEAR_ALERT = '@@rewards/ON_CLEAR_ALERT',
  ON_RECONCILE_STAMP = '@@rewards/ON_RECONCILE_STAMP',
  ON_CONTRIBUTE_LIST = '@@rewards/ON_CONTRIBUTE_LIST',
  ON_EXCLUDE_PUBLISHER = '@@rewards/ON_EXCLUDE_PUBLISHER',
  ON_RESTORE_PUBLISHERS = '@@rewards/ON_RESTORE_PUBLISHERS',
  ON_EXCLUDED_PUBLISHERS_NUMBER = '@@rewards/ON_EXCLUDED_PUBLISHERS_NUMBER',
  GET_CONTRIBUTION_AMOUNT = '@@rewards/GET_CONTRIBUTION_AMOUNT',
  ON_CONTRIBUTION_AMOUNT = '@@rewards/ON_CONTRIBUTION_AMOUNT',
  ON_RECURRING_TIPS = '@@rewards/ON_RECURRING_TIPS',
  REMOVE_RECURRING_TIP = '@@rewards/REMOVE_RECURRING_TIP',
  ON_CURRENT_TIPS = '@@rewards/ON_CURRENT_TIPS',
  GET_TIP_TABLE = '@@rewards/GET_TIP_TABLE',
  GET_CONTRIBUTE_LIST = '@@rewards/GET_CONTRIBUTE_LIST',
  INIT_AUTOCONTRIBUTE_SETTINGS = '@@rewards/INIT_AUTOCONTRIBUTE_SETTINGS',
  GET_ADS_DATA = '@@rewards/GET_ADS_DATA',
  ON_ADS_DATA = '@@rewards/ON_ADS_DATA',
  GET_ADS_HISTORY = '@@rewards/GET_ADS_HISTORY',
  TOGGLE_AD_THUMB_UP = '@@rewards/TOGGLE_AD_THUMB_UP',
  ON_TOGGLE_AD_THUMB_UP = '@@rewards/ON_TOGGLE_AD_THUMB_UP',
  TOGGLE_AD_THUMB_DOWN = '@@rewards/TOGGLE_AD_THUMB_DOWN',
  ON_TOGGLE_AD_THUMB_DOWN = '@@rewards/ON_TOGGLE_AD_THUMB_DOWN',
  TOGGLE_AD_OPT_IN = '@@rewards/TOGGLE_AD_OPT_IN',
  ON_TOGGLE_AD_OPT_IN = '@@rewards/ON_TOGGLE_AD_OPT_IN',
  TOGGLE_AD_OPT_OUT = '@@rewards/TOGGLE_AD_OPT_OUT',
  ON_TOGGLE_AD_OPT_OUT = '@@rewards/ON_TOGGLE_AD_OPT_OUT',
  TOGGLE_SAVED_AD = '@@rewards/TOGGLE_SAVED_AD',
  ON_TOGGLE_SAVED_AD = '@@rewards/ON_TOGGLE_SAVED_AD',
  TOGGLE_FLAGGED_AD = '@@rewards/TOGGLE_FLAGGED_AD',
  ON_TOGGLE_FLAGGED_AD = '@@rewards/ON_TOGGLE_FLAGGED_AD',
  ON_ADS_HISTORY = '@@rewards/ON_ADS_HISTORY',
  ON_ADS_SETTING_SAVE = '@@rewards/ON_ADS_SETTING_SAVE',
  GET_BALANCE_REPORT = '@@rewards/GET_BALANCE_REPORT',
  ON_BALANCE_REPORT = '@@rewards/ON_BALANCE_REPORT',
  GET_RECONCILE_STAMP = '@@rewards/GET_RECONCILE_STAMP',
  GET_STATEMENT = '@@rewards/GET_STATEMENT',
  ON_STATEMENT = '@@rewards/ON_STATEMENT',
  ON_STATEMENT_CHANGED = '@@rewards/ON_STATEMENT_CHANGED',
  ON_RECURRING_TIP_SAVED = '@@rewards/ON_RECURRING_TIP_SAVED',
  ON_RECURRING_TIP_REMOVED = '@@rewards/ON_RECURRING_TIP_REMOVED',
  ON_INLINE_TIP_SETTINGS_CHANGE = '@@rewards/ON_INLINE_TIP_SETTINGS_CHANGE',
  ON_INLINE_TIPS_ENABLED_CHANGE = '@@rewards/ON_INLINE_TIPS_ENABLED_CHANGE',
  ON_EXCLUDED_LIST = '@@rewards/ON_EXCLUDED_LIST',
  ON_RESTORE_PUBLISHER = '@@rewards/ON_RESTORE_PUBLISHER',
  GET_EXCLUDED_SITES = '@@rewards/GET_EXCLUDED_SITES',
  GET_BALANCE = '@@rewards/GET_BALANCE',
  ON_BALANCE = '@@rewards/ON_BALANCE',
  GET_EXTERNAL_WALLET_PROVIDERS = '@@rewards/GET_EXTERNAL_WALLET_PROVIDERS',
  GET_EXTERNAL_WALLET = '@@rewards/GET_EXTERNAL_WALLET',
  ON_GET_EXTERNAL_WALLET = '@@rewards/ON_GET_EXTERNAL_WALLET',
  CONNECT_EXTERNAL_WALLET = '@@rewards/CONNECT_EXTERNAL_WALLET',
  ON_CONNECT_EXTERNAL_WALLET = '@@rewards/ON_CONNECT_EXTERNAL_WALLET',
  HIDE_REDIRECT_MODAL = '@@rewards/HIDE_REDIRECT_MODAL',
  GET_MONTHLY_REPORT = '@@rewards/GET_MONTHLY_REPORT',
  ON_MONTHLY_REPORT = '@@rewards/ON_MONTHLY_REPORT',
  ON_RECONCILE_STAMP_RESET = '@@rewards/ON_RECONCILE_STAMP_RESET',
  GET_MONTHLY_REPORT_IDS = '@@rewards/GET_MONTHLY_REPORT_IDS',
  ON_MONTHLY_REPORT_IDS = '@@rewards/ON_MONTHLY_REPORT_IDS',
  DISMISS_PROMO_PROMPT = '@@rewards/DISMISS_PROMO_PROMPT',
  GET_COUNTRY_CODE = '@@rewards/GET_COUNTRY_CODE',
  ON_COUNTRY_CODE = '@@rewards/ON_COUNTRY_CODE',
  ON_INITIALIZED = '@@rewards/ON_INITIALIZED',
  COMPLETE_RESET = '@@rewards/COMPLETE_RESET',
  ON_COMPLETE_RESET = '@@rewards/ON_COMPLETE_RESET',
  GET_ONBOARDING_STATUS = '@@rewards/GET_ONBOARDING_STATUS',
  ON_ONBOARDING_STATUS = '@@rewards/ON_ONBOARDING_STATUS',
  ENABLE_REWARDS = '@@rewards/ENABLE_REWARDS',
  GET_ENABLED_INLINE_TIPPING_PLATFORMS = '@@rewards/GET_ENABLED_INLINE_TIPPING_PLATFORMS',
  ON_ENABLED_INLINE_TIPPING_PLATFORMS = '@@rewards/ON_ENABLED_INLINE_TIPPING_PLATFORMS',
  ON_EXTERNAL_WALLET_PROVIDER_LIST = '@@rewards/ON_EXTERNAL_WALLET_PROVIDER_LIST',
  SET_EXTERNAL_WALLET_TYPE = '@@rewards/SET_EXTERNAL_WALLET_TYPE',
  RESTART_BROWSER = '@@rewards/RESTART_BROWSER',
  ON_PREF_CHANGED = '@@rewards/ON_PREF_CHANGED',
  GET_IS_UNSUPPORTED_REGION = '@@rewards/GET_IS_UNSUPPORTED_REGION',
  ON_IS_UNSUPPORTED_REGION = '@@rewards/ON_IS_UNSUPPORTED_REGION'
}