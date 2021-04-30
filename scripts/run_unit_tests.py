#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

import argparse
import logging
import os
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
        if args.code_coverage:
            enable_bullseye(args.covfile)
        run_unit_tests(args.build_directory, args.jobs, args.build_type)
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
    root_parser.add_argument("--jobs", type=int, help="number of parallel jobs to run")
    root_parser.add_argument("--build-type", help="CMake build type to use")
    root_parser.add_argument(
        "--code-coverage", action="store_true", help="enable Bullseye coverage"
    )
    root_parser.add_argument(
        "--covfile",
        type=Path,
        default=Path("../test.cov"),
        help="path to Bullseye coverage file",
    )


def enable_bullseye(covfile_path):
    os.environ["COVFILE"] = str(covfile_path.resolve())
    logger.info("Bullseye covfile: %s", os.environ["COVFILE"])


def run_unit_tests(build_directory, jobs=None, build_type=None):
    cmd = f"ctest --output-on-failure"
    if build_type:
        cmd += f" -C {build_type}"
    cmd += f" --parallel"
    if jobs:
        cmd += f" {jobs}"
    logger.info("CTest command line: %s", cmd)
    process = subprocess.run(
        cmd,
        cwd=build_directory,
        shell=True,
        text=True,
        errors="replace",
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("CTest output:\n%s", output)
    process.check_returncode()


if __name__ == "__main__":
    sys.exit(main())
