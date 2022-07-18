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
        clang_format_diff_path = (
            find_clang_format_diff()
            if not args.clang_format_diff
            else args.clang_format_diff
        )
        test_clang_format(clang_format_diff_path, args.source_directory)
    except:
        logger.exception("Fatal error")
        return -1

    return 0


def process_command_line():
    parser = argparse.ArgumentParser()
    setup_parser(parser)
    return parser.parse_args()


def setup_parser(root_parser):
    root_parser.add_argument("--clang-format-diff", help="path to clang-format-diff")
    root_parser.add_argument(
        "--source-directory", type=Path, default=Path("../"), help="source directory"
    )


def find_clang_format_diff():
    paths = ["clang-format-diff", "clang-format-diff.py"]
    if platform.system() == "Windows":
        paths.append("C:\\Program Files\\LLVM\\share\\clang\\clang-format-diff.py")
    for path in paths:
        if shutil.which(path):
            return path
    raise RuntimeError("Failed to found clang-format-diff")


def run_git_cmd(cmd, source_directory):
    git = shutil.which("git")
    try:
        process = subprocess.run(
            [git] + cmd.split(' '),
            cwd=source_directory,
            shell=False,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )
        logger.debug("Git command line: %s %s", git, cmd)
    except subprocess.CalledProcessError as e:
        logger.error("Git output:\n%s", e.stdout.decode())
        raise
    return process.stdout.decode().strip()


def find_common_ancestor(source_directory):
    last_main_commit = run_git_cmd("rev-parse remotes/origin/main", source_directory)
    head_commit = run_git_cmd("rev-parse HEAD", source_directory)
    if last_main_commit == head_commit:
        return run_git_cmd("rev-parse HEAD^", source_directory)
    return run_git_cmd(f'merge-base {last_main_commit} HEAD', source_directory)


def test_clang_format(clang_format_diff, source_directory):
    common_ancestor = find_common_ancestor(source_directory)
    git_diff_cmd = f'git diff -U0 --no-color {common_ancestor}..HEAD'
    cmd = f'{git_diff_cmd} | "{clang_format_diff}" -p1 -i && git diff --exit-code'
    logger.info("clang-format-diff command line: %s", cmd)

    process = subprocess.run(
        cmd,
        shell=True,
        cwd=source_directory,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("clang-format-diff output:\n%s", output)
    process.check_returncode()


if __name__ == "__main__":
    sys.exit(main())
