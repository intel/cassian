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
            if i.is_dir() and i.name == ".git":
                return Path(p)
    return None


paths_to_format = [
    "./CMakeLists.txt",
    "./cmake/*.cmake",
    "./src/**/CMakeLists.txt",
    "./tests/**/CMakeLists.txt",
]

root_directory = get_root_directory()

paths = []
for path_to_format in paths_to_format:
    paths.extend(list(root_directory.rglob(path_to_format)))

path_strings = [str(p) for p in paths]
path_string = " ".join(path_strings)
format_process = subprocess.run(
    f"cmake-format -i {path_string}",
    shell=True,
    check=True,
    capture_output=True,
    text=True,
)
