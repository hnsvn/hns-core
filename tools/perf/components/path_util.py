# Copyright (c) 2022 The Hns Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# you can obtain one at http://mozilla.org/MPL/2.0/.

import contextlib
import os
import sys


def GetSrcDir() -> str:
  return os.path.abspath(
      os.path.join(os.path.dirname(__file__), os.pardir, os.pardir, os.pardir,
                   os.pardir))


def GetHnsDir() -> str:
  return os.path.join(GetSrcDir(), 'hns')


def GetHnsPerfDir() -> str:
  return os.path.join(GetSrcDir(), 'hns', 'tools', 'perf')


def GetChromiumPerfDir() -> str:
  return os.path.join(GetSrcDir(), 'tools', 'perf')


def GetHnsPerfProfileDir() -> str:
  return os.path.join(GetHnsPerfDir(), 'profiles')


def GetHnsPerfConfigDir() -> str:
  return os.path.join(GetHnsPerfDir(), 'configs')


def GetHnsPerfBucket() -> str:
  return 'hns-telemetry'


def GetDepotToolsDir() -> str:
  return os.path.join(GetHnsDir(), 'vendor', 'depot_tools')


def GetPyJson5Dir() -> str:
  return os.path.join(GetSrcDir(), 'third_party', 'pyjson5', 'src')


def GetCatapultDir() -> str:
  return os.path.join(GetSrcDir(), 'third_party', 'catapult')


def GetTelemetryDir() -> str:
  return os.path.join(GetCatapultDir(), 'telemetry')


def GetGoogleAuthDir() -> str:
  return os.path.join(GetCatapultDir(), 'third_party', 'google-auth')


def GetAdbPath() -> str:
  return os.path.join(GetSrcDir(), 'third_party', 'android_sdk', 'public',
                      'platform-tools', 'adb')


def GetVpython3Path() -> str:
  return os.path.join(GetDepotToolsDir(),
                      'vpython3.bat' if sys.platform == 'win32' else 'vpython3')


def GetChromeReleasesJsonPath() -> str:
  return os.path.join(GetHnsPerfDir(), 'chrome_releases.json')


@contextlib.contextmanager
def SysPath(path, position=None):
  if position is None:
    sys.path.append(path)
  else:
    sys.path.insert(position, path)
  try:
    yield
  finally:
    if sys.path[-1] == path:
      sys.path.pop()
    else:
      sys.path.remove(path)
