#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

import argparse
import logging
import platform
import shutil
import subprocess
import sys
from pathlib import Path

logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)


def main(args=None):
    if not args:
        args = process_command_line()
    logger.debug("Arguments: %s", args)

    try:
        run_clang_tidy_path = (
            find_run_clang_tidy() if not args.run_clang_tidy else args.run_clang_tidy
        )
        run_clang_tidy(run_clang_tidy_path, args.build_directory, args.source_directory)
    except:
        logger.exception("Fatal error")
        return -1

    return 0


def process_command_line():
    parser = argparse.ArgumentParser()
    setup_parser(parser)
    return parser.parse_args()


def setup_parser(root_parser):
    root_parser.add_argument(
        "--build-directory", type=Path, default=Path("../build"), help="build directory"
    )
    root_parser.add_argument(
        "--source-directory", type=Path, default=Path("../"), help="source directory"
    )
    root_parser.add_argument("--run-clang-tidy", help="path to run-clang-tidy")


def find_run_clang_tidy():
    paths = ["run-clang-tidy", "run-clang-tidy.py"]
    if platform.system() == "Windows":
        paths.append("C:\\Program Files\\LLVM\\share\\clang\\run-clang-tidy.py")
    for path in paths:
        if shutil.which(path):
            return path
    raise RuntimeError("Failed to found run-clang-tidy")


def run_clang_tidy(run_clang_tidy, build_directory, source_directory):
    paths = [
        str(source_directory / "src"),
        str(source_directory / "tests"),
    ]
    paths = " ".join(paths)
    cmd = f'"{run_clang_tidy}" {paths} -p {build_directory}'
    logger.info("run-clang-tidy command line: %s", cmd)

    process = subprocess.run(
        cmd, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("run-clang-tidy output:\n%s", output)
    process.check_returncode()


if __name__ == "__main__":
    sys.exit(main())
