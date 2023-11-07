const path = require('path')
const fs = require('fs-extra')
const ip = require('ip')
const URL = require('url').URL
const config = require('../lib/config')
const util = require('../lib/util')

const start = (passthroughArgs, buildConfig = config.defaultBuildConfig, options) => {
  config.buildConfig = buildConfig
  config.update(options)

  let hnsArgs = [
    '--enable-logging',
    '--v=' + options.v,
  ]
  if (options.vmodule) {
    hnsArgs.push('--vmodule=' + options.vmodule);
  }
  if (options.no_sandbox) {
    hnsArgs.push('--no-sandbox')
  }
  if (options.disable_hns_extension) {
    hnsArgs.push('--disable-hns-extension')
  }
  if (options.disable_hns_rewards_extension) {
    hnsArgs.push('--disable-hns-rewards-extension')
  }
  if (options.disable_pdfjs_extension) {
    hnsArgs.push('--disable-pdfjs-extension')
  }
  if (options.disable_webtorrent_extension) {
    hnsArgs.push('--disable-webtorrent-extension')
  }
  if (options.ui_mode) {
    hnsArgs.push(`--ui-mode=${options.ui_mode}`)
  }
  if (!options.enable_hns_update) {
    // This only has meaning with MacOS and official build.
    hnsArgs.push('--disable-hns-update')
  }
  if (options.disable_doh) {
    hnsArgs.push('--disable-doh')
  }
  if (options.single_process) {
    hnsArgs.push('--single-process')
  }
  if (options.show_component_extensions) {
    hnsArgs.push('--show-component-extension-options')
  }
  if (options.rewards) {
    hnsArgs.push(`--rewards=${options.rewards}`)
  }
  if (options.hns_ads_testing) {
    hnsArgs.push('--hns-ads-testing')
  }
  if (options.hns_ads_debug) {
    hnsArgs.push('--hns-ads-debug')
  }
  if (options.hns_ads_production) {
    hnsArgs.push('--hns-ads-production')
  }
  if (options.hns_ads_staging) {
    hnsArgs.push('--hns-ads-staging')
  }
  hnsArgs = hnsArgs.concat(passthroughArgs)

  let user_data_dir
  if (options.user_data_dir_name) {
    if (process.platform === 'darwin') {
      user_data_dir = path.join(process.env.HOME, 'Library', 'Application\\ Support', 'HnsSoftware', options.user_data_dir_name)
    } else if (process.platform === 'win32') {
      user_data_dir = path.join(process.env.LocalAppData, 'HnsSoftware', options.user_data_dir_name)
    } else {
      user_data_dir = path.join(process.env.HOME, '.config', 'HnsSoftware', options.user_data_dir_name)
    }
    hnsArgs.push('--user-data-dir=' + user_data_dir);
  }

  let cmdOptions = {
    stdio: 'inherit',
    timeout: undefined,
    continueOnFail: false,
    shell: process.platform === 'darwin' ? true : false,
    killSignal: 'SIGTERM'
  }

  let outputPath = options.output_path
  if (!outputPath) {
    outputPath = path.join(config.outputDir, 'hns')
    if (process.platform === 'win32') {
      outputPath = outputPath + '.exe'
    } else if (process.platform === 'darwin') {
      outputPath = fs.readFileSync(outputPath + '_helper').toString().trim()
    }
  }
  util.run(outputPath, hnsArgs, cmdOptions)
}

module.exports = start
