# Copyright (c) 2019 The Hns Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/. */

import os

HNS_THIRD_PARTY_DIRS = [
    'vendor',
]

ANDROID_ONLY_PATHS = []

DESKTOP_ONLY_PATHS = []

_REPOSITORY_ROOT = None


def AddHnsCredits(root, prune_paths, special_cases, prune_dirs,
                    additional_paths):
    global _REPOSITORY_ROOT  # pylint: disable=global-statement
    _REPOSITORY_ROOT = root

    # Exclude these specific paths from needing a README.chromium file.
    prune_paths.update([
        # Formerly external Hns code which has moved to hns-core
        # (i.e these are already covered by the Hns Browser license notice).
        os.path.join('hns', 'third_party', 'challenge_bypass_ristretto_ffi'),
        os.path.join('hns', 'vendor', 'hns-ios'),
        os.path.join('hns', 'vendor', 'hns_base'),

        # No third-party code directly under android_deps. It's all under
        # android_deps/libs instead and it's special-cased further down.
        os.path.join('hns', 'third_party', 'android_deps'),

        # Hns overrides to third-party code, also covered by main notice.
        os.path.join('hns', 'third_party', 'blink'),
        os.path.join('hns', 'third_party', 'libaddressinput'),
        os.path.join('hns', 'patches', 'third_party'),

        # Build dependencies which don't end up in the binaries.
        os.path.join('hns', 'vendor', 'depot_tools'),
        os.path.join('hns', 'vendor', 'gn-project-generators'),
    ])

    # Add the licensing info that would normally be in a README.chromium file.
    # This is for when we pull in external repos directly.
    special_cases.update({
        os.path.join('hns', 'vendor', 'bat-native-tweetnacl'): {
            "Name": "TweetNaCl",
            "URL": "https://github.com/hns-intl/bat-native-tweetnacl",
            "License": "MPL-2.0",
        },
        os.path.join('hns', 'third_party', 'bip39wally-core-native'): {
            "Name": "libwally-core",
            "URL": "https://github.com/hns-intl/bat-native-bip39wally-core",
            "License": "MIT",
        },
        os.path.join('hns', 'vendor', 'boto'): {
            "Name": "boto",
            "URL": "https://github.com/huhisoft/boto",
            "License": "MIT",
        },
        os.path.join('hns', 'vendor', 'hns-extension'): {
            "Name": "Hns Only Extension",
            "URL": "https://github.com/hnsvn/hns-extension",
            "License": "MPL-2.0",
        },
        os.path.join('hns', 'vendor', 'web-discovery-project'): {
            "Name": "Web Discovery Project",
            "URL": "https://github.com/hnsvn/web-discovery-project.",
            "License": "MPL-2.0",
        },
        os.path.join('hns', 'vendor', 'omaha'): {
            "Name": "Omaha",
            "URL": "https://github.com/huhisoft/omaha",
            "License": "Apache-2.0",
            "License File": ["/hns/vendor/omaha/LICENSE.txt"],
        },
        os.path.join('hns', 'vendor', 'omaha', 'third_party', 'breakpad'): {
            "Name": "Breakpad",
            "URL": "https://chromium.googlesource.com/breakpad/breakpad",
            "License File": [
                "/hns/vendor/omaha/third_party/breakpad/LICENSE"
            ],
        },
        os.path.join('hns', 'vendor', 'omaha', 'third_party', 'googletest'): {
            "Name": "GoogleTest",
            "URL": "https://github.com/google/googletest",
            "License": "BSD",
            "License File":
                ["/hns/vendor/omaha/third_party/googletest/LICENSE"],
        },
        os.path.join('hns', 'vendor', 'omaha', 'third_party', 'libzip'): {
            "Name": "libzip",
            "URL": "https://libzip.org",
            "License": "BSD",
            "License File": ["/hns/vendor/omaha/third_party/libzip/LICENSE"],
        },
        os.path.join('hns', 'third_party', 'rapidjson'): {
            "Name": "RapidJSON",
            "URL": "https://github.com/Tencent/rapidjson",
            "License": "MIT",
            "License File": ["/hns/third_party/rapidjson/src/license.txt"],
        },
        os.path.join('hns', 'third_party', 'rust'): {
            "Name": "rust-cxx",
            "URL": "https://crates.io/crates/cxx",
            "License": "Apache-2.0",
            "License File": \
                ["/hns/third_party/rust/cxx/v1/crate/LICENSE-APACHE"],
        },
        os.path.join('hns', 'third_party', 'rust'): {
            "Name": "rust-cxx",
            "URL": "https://crates.io/crates/either",
            "License": "MIT",
            "License File": \
                ["/hns/third_party/rust/either/v1/crate/LICENSE-MIT"],
        },
        os.path.join('hns', 'vendor', 'omaha', 'third_party', 'zlib'): {
            "Name": "zlib",
            "URL": "https://zlib.net",
            "License": "zlib",
            "License File": ["/hns/vendor/omaha/third_party/zlib/README"],
        },
        os.path.join('hns', 'vendor', 'python-patch'): {
            "Name": "Python Patch",
            "URL": "https://github.com/huhisoft/python-patch",
            "License": "MIT",
            "License File": ["/hns/vendor/python-patch/doc/LICENSE"],
        },
        os.path.join('hns', 'vendor', 'requests'): {
            "Name": "Requests",
            "URL": "https://github.com/psf/requests",
            "License": "Apache-2.0",
        },
        os.path.join('hns', 'vendor', 'sparkle'): {
            "Name": "Sparkle",
            "URL": "https://github.com/huhisoft/Sparkle",
            "License": "MIT",
        },
        os.path.join('hns', 'third_party', 'cryptography'): {
            "Name": "cryptography",
            "URL": "https://cryptography.io",
            "License": "Apache-2.0",
            "License File": ["/hns/common/licenses/Apache-2.0"],
        },
        os.path.join('hns', 'third_party', 'macholib'): {
            "Name": "macholib",
            "URL": "https://github.com/Mr2k6/macholib",
            "License": "MIT",
        },
    })

    # Don't recurse into these directories looking for third-party code.
    prune_list = list(prune_dirs)
    prune_list += [
        'chromium_src',  # Hns's overrides, covered by main notice.
        'node_modules',  # See hns/third_party/npm-* instead.
        '.vscode',       # Automatically added by Visual Studio.
    ]
    prune_dirs = tuple(prune_list)

    # Look for a README.chromium file directly inside these directories.
    # This is for directories which include third-party code that isn't
    # contained under a "third_party" or "vendor" directory.
    additional_list = list(additional_paths)
    additional_list += [
        os.path.join('hns', 'components', 'hns_new_tab_ui', 'data'),
        os.path.join('hns', 'browser', 'hns_vpn', 'win',
                     'hns_vpn_wireguard_service'),
        os.path.join('hns', 'components', 'filecoin'),
    ]

    # Add all Android libraries since they're not directly contained
    # within a third_party directory.
    android_libs = os.path.join('hns', 'third_party', 'android_deps', 'libs')
    for _, dirs, _ in os.walk(os.path.join(root, android_libs)):
        for dirpath in dirs:
            dirname = os.path.basename(dirpath)
            additional_list += [os.path.join(android_libs, dirname)]

    additional_paths = tuple(additional_list)

    return (prune_dirs, additional_paths)


def CheckHnsMissingLicense(target_os, path, error):
    if path.startswith('hns'):
        if target_os == 'android':
            if path in DESKTOP_ONLY_PATHS:
                return  # Desktop failures are not relevant on Android.
        else:
            if path in ANDROID_ONLY_PATHS:
                return  # Android failures are not relevant on desktop.
        if not ContainsFiles(os.path.join(_REPOSITORY_ROOT, path)):
            return  # Empty directories do not require license.
        print('\nERROR: missing license information in %s\n'
              "If this is code you added, then you'll have to add the required "
              "metadata.\nIf the path that's mentioned isn't something you "
              "added, then you probably just need to remove that obsolete path "
              "from your local checkout.\n" % path)
        raise error


def ContainsFiles(path):
    assert os.path.exists(path), f'{path} not found'

    def reraise(e):
        raise e

    for _, _, filenames in os.walk(path, onerror=reraise):
        if filenames:
            return True

    return False
