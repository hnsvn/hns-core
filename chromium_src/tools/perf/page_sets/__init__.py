# Copyright (c) 2022 The Hns Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# you can obtain one at http://mozilla.org/MPL/2.0/.
"""A inline part of page_sets/__init__.py"""

hns_top_level_dir = os.path.abspath(os.path.join(os.path.dirname(
      __file__), os.pardir, os.pardir, os.pardir, 'hns', 'tools', 'perf'))
hns_start_dir = os.path.join(hns_top_level_dir, 'hns_page_sets')
for base_class in base_classes:
  for cls in discover.DiscoverClasses(
          hns_start_dir, hns_top_level_dir, base_class).values():
    setattr(sys.modules[__name__], cls.__name__, cls)
