#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

import argparse
import logging
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
        test_cmake_format(args.source_directory)
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
        "--source-directory", type=Path, default=Path("../"), help="source directory"
    )


def test_cmake_format(source_directory):
    run_cmake_format_path = Path(__file__).resolve().parent / "run_cmake_format.py"
    python = "python3" if shutil.which("python3") else "python"
    cmd = f"{python} {run_cmake_format_path} && git diff --exit-code"
    logger.info("cmake-format command line: %s", cmd)

    process = subprocess.run(
        cmd,
        shell=True,
        cwd=source_directory,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("cmake-format output:\n%s", output)
    process.check_returncode()


if __name__ == "__main__":
    sys.exit(main())
