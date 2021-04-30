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
        report_code_coverage(args.bullseye, args.covfile)
        if args.html:
            generate_html(args.bullseye, args.covfile, args.html_path)
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
    root_parser.add_argument(
        "--html",
        action="store_true",
        help="generate HTML report in addition to the regular one",
    )
    root_parser.add_argument(
        "--html-path",
        type=Path,
        default=Path("../coverage_report"),
        help="destination directory for HTML report",
    )


def report_code_coverage(bullseye_path, covfile_path):
    bullseye_bin_path = bullseye_path / "bin"

    covsrc_path = bullseye_bin_path / "covsrc"
    cmd = f"{covsrc_path} --file {covfile_path} --by-name --width 120"
    logger.info("Bullseye command line: %s", cmd)
    process = subprocess.run(
        cmd, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT
    )
    output = process.stdout
    logger.info("Bullseye output:\n%s", output)
    process.check_returncode()


def generate_html(bullseye_path, covfile_path, destination):
    bullseye_bin_path = bullseye_path / "bin"

    covhtml_path = bullseye_bin_path / "covhtml"
    cmd = f"{covhtml_path} --file {covfile_path} {destination}"
    logger.info("Bullseye command line: %s", cmd)
    process = subprocess.run(
        cmd, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT
    )
    output = process.stdout
    logger.info("Bullseye output:\n%s", output)
    process.check_returncode()


if __name__ == "__main__":
    sys.exit(main())
