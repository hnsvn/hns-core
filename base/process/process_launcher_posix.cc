/* Copyright (c) 2023 The Hns Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "hns/base/process/process_launcher.h"

#include <string>
#include <vector>

#include "base/files/file_util.h"
#include "base/process/launch.h"

namespace hns {

ProcessLauncher::ProcessLauncher() = default;
ProcessLauncher::~ProcessLauncher() = default;

// static
absl::optional<std::string> ProcessLauncher::ReadAppOutput(
    base::CommandLine cmdline,
    base::LaunchOptions options,
    int timeout_sec) {
  int pipe_fd[2];
  if (pipe(pipe_fd) < 0) {
    return absl::nullopt;
  }

  options.fds_to_remap.emplace_back(pipe_fd[1], STDOUT_FILENO);
  options.fds_to_remap.emplace_back(pipe_fd[1], STDERR_FILENO);

  base::Process process = base::LaunchProcess(cmdline, options);
  if (!process.IsValid()) {
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    return absl::nullopt;
  }

  bool exited = false;
  int exit_code = 0;
  std::string result;
  close(pipe_fd[1]);
  bool read_result = base::ReadStreamToString(
      base::FileToFILE(base::File(pipe_fd[0]), "r"), &result);
  close(pipe_fd[0]);

  base::ScopedAllowBaseSyncPrimitives allow_wait_for_process;
  exited =
      process.WaitForExitWithTimeout(base::Seconds(timeout_sec), &exit_code);
  if (!exited) {
    process.Terminate(0, true);
  }
  if (exited && !exit_code && read_result) {
    return result;
  } else {
    return absl::nullopt;
  }
}

}  // namespace hns
