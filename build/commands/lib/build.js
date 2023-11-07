const config = require('../lib/config')
const util = require('../lib/util')
const path = require('path')
const fs = require('fs-extra')

/**
 * Checks to make sure the src/chrome/VERSION matches hns-core's package.json version
 */
const checkVersionsMatch = () => {
  const srcChromeVersionDir = path.resolve(path.join(config.srcDir, 'chrome', 'VERSION'))
  const versionData = fs.readFileSync(srcChromeVersionDir, 'utf8')
  const re = /MAJOR=(\d+)\s+MINOR=(\d+)\s+BUILD=(\d+)\s+PATCH=(\d+)/
  const found = versionData.match(re)
  const hnsVersionFromChromeFile = `${found[2]}.${found[3]}.${found[4]}`
  if (hnsVersionFromChromeFile !== config.hnsVersion) {
    // Only a warning. The CI environment will choose to proceed or not within its own script.
    console.warn(`Version files do not match!\nsrc/chrome/VERSION: ${hnsVersionFromChromeFile}\nhns-core package.json version: ${config.hnsVersion}`)
  }
}

const build = (buildConfig = config.defaultBuildConfig, options = {}) => {
  config.buildConfig = buildConfig
  config.update(options)
  checkVersionsMatch()

  util.touchOverriddenFiles()
  util.updateBranding()

  if (config.xcode_gen_target) {
    util.generateXcodeWorkspace()
  } else {
    if (options.no_gn_gen === undefined)
      util.generateNinjaFiles()
    util.buildTarget()
  }
}

module.exports = build
