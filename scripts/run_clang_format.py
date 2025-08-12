#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

import logging
import subprocess
from pathlib import Path


logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)


def run_git_cmd(cmd):
    try:
        process = subprocess.run(
            cmd,
            shell=True,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
        logger.debug("Git command line: %s", cmd)
    except subprocess.CalledProcessError as e:
        logger.error("Git output:\n%s", e.stdout.decode())
        raise
    return process.stdout.decode().strip()


def get_root_directory():
    return Path(run_git_cmd("git rev-parse --show-toplevel"))


directories_to_format = ["src", "tests"]
root_directory = get_root_directory()

for directory_to_format in directories_to_format:
    path = root_directory / directory_to_format
    for p in path.rglob("*"):
        if p.suffix not in [".cpp", ".hpp", ".h", ".cl"]:
            continue
        format_process = subprocess.run(
            f"clang-format -i {p}",
            shell=True,
            check=True,
            capture_output=True,
            text=True,
        )
