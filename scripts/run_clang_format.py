#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

import subprocess
from pathlib import Path


def get_root_directory():
    p = Path(Path.cwd() / __file__)
    while p.parent != ".git":
        p = p.parent
        for i in p.rglob("*"):
            if i.name == ".git":
                return Path(p)
    return None


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
