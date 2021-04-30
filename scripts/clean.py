#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

import argparse
import logging
import shutil
import sys
from pathlib import Path

logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)


def main(args=None):
    if not args:
        args = process_command_line()
    logger.debug("Arguments: %s", args)

    try:
        clean_build_directory(args.build_directory)
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
        "build_directory",
        metavar="build-directory",
        type=Path,
        nargs="?",
        default=Path("../build"),
        help="directory to clean",
    )


def clean_build_directory(directory):
    logger.info("Cleaning: %s", directory)
    shutil.rmtree(directory)


if __name__ == "__main__":
    sys.exit(main())
