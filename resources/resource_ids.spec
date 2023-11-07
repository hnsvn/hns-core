# Since recent changes upstream, Chromium is now using IDs up to ~46310
# (see end of //out/Component/gen/tools/gritsettings/default_resource_ids),
# so let's leave some padding after that and start assigning them on 47500.
{
  "SRCDIR": "../..",
  "hns/common/extensions/api/hns_api_resources.grd": {
    "includes": [50700],
  },
  "hns/components/resources/hns_components_resources.grd": {
    "includes": [50710],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_adblock/hns_adblock.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [50900],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_new_tab/hns_new_tab.grd": {
    "META": {"sizes": {"includes": [50]}},
    "includes": [50920],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_welcome/hns_welcome.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [50970],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/browser/resources/settings/hns_settings_resources.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [50990],
  },
  "hns/app/hns_generated_resources.grd": {
    "includes": [51000],
    "messages": [51050],
  },
  "hns/app/theme/hns_theme_resources.grd": {
    "structures": [52000],
  },
  "hns/app/theme/hns_unscaled_resources.grd": {
    "includes": [52100],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_webtorrent/hns_webtorrent.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [52110],
  },
  "hns/components/hns_webtorrent/resources.grd": {
    "includes": [52120],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_rewards_panel/hns_rewards_panel.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [52130],
  },
  "hns/components/hns_rewards/resources/hns_rewards_static_resources.grd": {
    "includes": [52150],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_rewards_page/hns_rewards_page.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [52160],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_rewards_internals/hns_rewards_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [52180],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_rewards_tip/hns_rewards_tip.grd": {
    "META": {"sizes": {"includes": [20]}},
    "includes": [52190],
  },
  "hns/components/resources/hns_components_strings.grd": {
    "messages": [52250],
  },
  "hns/components/hns_ads/resources/bat_ads_resources.grd": {
    "includes": [54750]
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_wallet_page/hns_wallet_page.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [54760],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-ethereum_remote_client_page/ethereum_remote_client_page.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [54960],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_wallet_panel/hns_wallet_panel.grd": {
    "META": {"sizes": {"includes": [200]}},
    "includes": [54970],
  },
  "hns/components/hns_extension/extension/resources.grd": {
    "includes": [55250],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_extension/hns_extension.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55270],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-webcompat_reporter/webcompat_reporter.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55280],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-ipfs/ipfs.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55290],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-cosmetic_filters/cosmetic_filters.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55300],
  },
  "hns/components/tor/resources/tor_static_resources.grd": {
    "includes": [55310],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-tor_internals/tor_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55320],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_wallet_script/hns_wallet_script.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55330],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_vpn_panel/hns_vpn_panel.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55340],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_shields_panel/hns_shields_panel.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55350],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-trezor_bridge/trezor_bridge.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55360],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-market_display/market_display.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [55370],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_private_new_tab/hns_private_new_tab.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55470],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-playlist/playlist.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55480],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-ledger_bridge/ledger_bridge.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55490],
  },
  # This file is generated during the build.
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-nft_display/nft_display.grd": {
    "META": {"sizes": {"includes": [40]}},
    "includes": [55500],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-cookie_list_opt_in/cookie_list_opt_in.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55540],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_speedreader_toolbar/hns_speedreader_toolbar.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55550],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_adblock_internals/hns_adblock_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55560],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-commands/commands.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [55570],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_wallet_swap_page/hns_wallet_swap_page.grd": {
    "META": {"sizes": {"includes": [150]}},
    "includes": [55580],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_wallet_send_page/hns_wallet_send_page.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [55720],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_wallet_deposit_page/hns_wallet_deposit_page.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [55820],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_wallet_fund_wallet_page/hns_wallet_fund_wallet_page.grd": {
    "META": {"sizes": {"includes": [100]}},
    "includes": [55920],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-tip_panel/tip_panel.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [56020]
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-ai_chat_ui/ai_chat_ui.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [56030],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-skus_internals/skus_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [56040],
  },
  "<(SHARED_INTERMEDIATE_DIR)/hns/web-ui-hns_news_internals/hns_news_internals.grd": {
    "META": {"sizes": {"includes": [10]}},
    "includes": [56050],
  }
}
