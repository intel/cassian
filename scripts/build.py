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
            enable_bullseye(args.bullseye, args.covfile)
        build_project(args.build_directory, args.jobs, args.target, args.build_type)
        if args.code_coverage:
            select_coverage_paths(args.covfile)
            disable_bullseye()
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
    root_parser.add_argument("--target", help="target to build")
    root_parser.add_argument("--build-type", help="CMake build type to use")
    root_parser.add_argument(
        "--code-coverage", action="store_true", help="enable Bullseye coverage"
    )
    root_parser.add_argument(
        "--bullseye",
        type=Path,
        default=Path("/opt/BullseyeCoverage"),
        help="path to Bullseye installation directory",
    )
    root_parser.add_argument(
        "--covfile",
        type=Path,
        default=Path("../test.cov"),
        help="path to Bullseye coverage file",
    )


def enable_bullseye(bullseye_path, covfile_path):
    os.environ["PATH"] = os.pathsep.join(
        [str(bullseye_path / "bin"), os.environ["PATH"]]
    )
    os.environ["COVFILE"] = str(covfile_path.resolve())
    logger.info("Bullseye covfile: %s", os.environ["COVFILE"])

    cmd = f"cov01 -1"
    logger.info("Bullseye command line: %s", cmd)
    process = subprocess.run(
        cmd, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("Bullseye output:\n%s", output)
    process.check_returncode()


def select_coverage_paths(covfile_path):
    cmd = f"covselect --file {covfile_path.resolve()} --add '!third_party/'"
    logger.info("Bullseye command line: %s", cmd)
    process = subprocess.run(
        cmd, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("Bullseye output:\n%s", output)
    process.check_returncode()


def disable_bullseye():
    cmd = f"cov01 -0"
    logger.info("Bullseye command line: %s", cmd)
    process = subprocess.run(
        cmd, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("Bullseye output:\n%s", output)
    process.check_returncode()


def build_project(build_directory, jobs=None, target=None, build_type=None):
    cmd = f"cmake --build {build_directory} --parallel"
    if jobs:
        cmd += f" {jobs}"
    if target:
        cmd += f" --target {target}"
    if build_type:
        cmd += f" --config {build_type}"
    logger.info("CMake command line: %s", cmd)
    process = subprocess.run(
        cmd, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("CMake output:\n%s", output)
    process.check_returncode()


if __name__ == "__main__":
    sys.exit(main())
