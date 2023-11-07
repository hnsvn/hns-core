#!/usr/bin/env python

# Copyright (c) 2019 The Hns Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/. */


CHROMIUM_POLICY_KEY = 'SOFTWARE\\\\Policies\\\\HnsSoftware\\\\Hns'


def AddHnsPolicies(template_file_contents):
    highest_id = template_file_contents['highest_id_currently_used']
    policies = [
        {
            'name': 'TorDisabled',
            'type': 'main',
            'schema': {
                'type': 'boolean'
            },
            'supported_on': [
                'chrome.win:78-', 'chrome.mac:93-', 'chrome.linux:93-'
            ],
            'features': {
                'dynamic_refresh': False,
                'per_profile': False,
                'can_be_recommended': False,
                'can_be_mandatory': True
            },
            'example_value': True,
            'id': 0,
            'caption': '''Disables the tor feature.''',
            'tags': [],
            'desc': ('''This policy allows an admin to specify that tor '''
                     '''must be disabled at startup.'''),
        },
        {
            'name': 'IPFSEnabled',
            'type': 'main',
            'schema': {
                'type': 'boolean'
            },
            'supported_on': ['chrome.*:87-'],
            'future_on': ['android'],
            'features': {
                'dynamic_refresh': False,
                'per_profile': True,
                'can_be_recommended': False,
                'can_be_mandatory': True
            },
            'example_value': True,
            'id': 1,
            'caption': '''Enable IPFS feature''',
            'tags': [],
            'desc': ('''This policy allows an admin to specify whether IPFS '''
                     '''feature can be enabled.'''),
        },
        {
            'name': 'HnsRewardsDisabled',
            'type': 'main',
            'schema': {
                'type': 'boolean'
            },
            'supported_on': ['chrome.*:105-'],
            'features': {
                'dynamic_refresh': False,
                'per_profile': True,
                'can_be_recommended': False,
                'can_be_mandatory': True
            },
            'example_value': True,
            'id': 2,
            'caption': '''Disable Hns Rewards feature.''',
            'tags': [],
            'desc': ('''This policy allows an admin to specify that Hns '''
                     '''Rewards feature will be disabled.'''),
        },
        {
            'name': 'HnsWalletDisabled',
            'type': 'main',
            'schema': {
                'type': 'boolean'
            },
            'supported_on': ['chrome.*:106-'],
            'features': {
                'dynamic_refresh': False,
                'per_profile': True,
                'can_be_recommended': False,
                'can_be_mandatory': True
            },
            'example_value': True,
            'id': 3,
            'caption': '''Disable Hns Wallet feature.''',
            'tags': [],
            'desc': ('''This policy allows an admin to specify that Hns '''
                     '''Wallet feature will be disabled.'''),
        },
        {
            'name': 'HnsShieldsDisabledForUrls',
            'type': 'main',
            'schema': {
                'type': 'array',
                'items': {
                    'type': 'string'
                },
            },
            'supported_on': ['chrome.*:107-'],
            'features': {
                'dynamic_refresh': False,
                'per_profile': True,
                'can_be_recommended': False,
                'can_be_mandatory': True
            },
            'example_value': ['https://hns.com'],
            'id': 4,
            'caption': '''Disables Hns Shields for urls.''',
            'tags': [],
            'desc': ('''This policy allows an admin to specify that Hns '''
                     '''Shields disabled.'''),
        },
        {
            'name': 'HnsShieldsEnabledForUrls',
            'type': 'main',
            'schema': {
                'type': 'array',
                'items': {
                    'type': 'string'
                },
            },
            'supported_on': ['chrome.*:107-'],
            'features': {
                'dynamic_refresh': False,
                'per_profile': True,
                'can_be_recommended': False,
                'can_be_mandatory': True
            },
            'example_value': ['https://hns.com'],
            'id': 5,
            'caption': '''Enables Hns Shields for urls.''',
            'tags': [],
            'desc': ('''This policy allows an admin to specify that Hns '''
                     '''Shields enabled.'''),
        },
        {
            'name': 'HnsVPNDisabled',
            'type': 'main',
            'schema': {
                'type': 'boolean'
            },
            'supported_on': ['chrome.*:112-'],
            'future_on': ['android'],
            'features': {
                'dynamic_refresh': False,
                'per_profile': True,
                'can_be_recommended': False,
                'can_be_mandatory': True
            },
            'example_value': True,
            'id': 6,
            'caption': '''Disable Hns VPN feature.''',
            'tags': [],
            'desc': ('''This policy allows an admin to specify that Hns '''
                     '''VPN feature will be disabled.'''),
        },
    ]

    # Our new polices are added with highest id
    next_id = highest_id + 1
    for policy in policies:
        next_id += 1
        policy['id'] = next_id
        template_file_contents['policy_definitions'].append(policy)

    # Update highest id
    template_file_contents['highest_id_currently_used'] = highest_id + \
        len(policies)
