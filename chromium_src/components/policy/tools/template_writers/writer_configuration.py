# Copyright (c) 2022 The Hns Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at https://mozilla.org/MPL/2.0/.

import override_utils

@override_utils.override_function(globals())
def GetConfigurationForBuild(original_function, defines):
  base = original_function(defines)
  return _merge_dicts(_HNS_VALUES, base)

_HNS_VALUES = {
  'build': 'hns',
  'app_name': 'Hns',
  'doc_url':
    'https://support.hns.com/hc/en-us/articles/360039248271-Group-Policy',
  'frame_name': 'Hns Frame',
  'webview_name': 'Hns WebView',
  'win_config': {
    'win': {
      'reg_mandatory_key_name': 'Software\\Policies\\HnsSoftware\\Hns',
      'reg_recommended_key_name':
        'Software\\Policies\\HnsSoftware\\Hns\\Recommended',
      'mandatory_category_path': ['Hns:Cat_Hns', 'hns'],
      'recommended_category_path': ['Hns:Cat_Hns', 'hns_recommended'],
      'category_path_strings': {
        'hns': 'Hns',
        'hns_recommended':
        'Hns - {doc_recommended}'
      },
      'namespace': 'HnsSoftware.Policies.Hns',
    },
  },
  # The string 'Hns' is defined in hns.adml for ADMX, but ADM doesn't
  # support external references, so we define this map here.
  'adm_category_path_strings': {
    'Hns:Cat_Hns': 'Hns'
  },
  'admx_prefix': 'hns',
  'admx_using_namespaces': {
    'Hns': 'HnsSoftware.Policies'  # prefix: namespace
  },
  'linux_policy_path': '/etc/hns/policies/',
  'bundle_id': 'com.hns.ios.core',
}

def _merge_dicts(src, dst):
  result = dict(dst)
  for k, v in src.items():
    result[k] = _merge_dicts(v, dst.get(k, {})) if isinstance(v, dict) else v
  return result
