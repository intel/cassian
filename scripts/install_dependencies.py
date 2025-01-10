#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

import argparse
import logging
import platform
import urllib.request
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
        install_dependencies()
    except:
        logger.exception("Fatal error")
        return -1

    return 0


def process_command_line():
    parser = argparse.ArgumentParser()
    setup_parser(parser)
    return parser.parse_args()


def setup_parser(root_parser):
    pass


def get_root_directory():
    p = Path(Path.cwd() / __file__)
    while p.parent != ".git":
        p = p.parent
        for i in p.rglob("*"):
            if i.is_dir() and i.name == ".git":
                return Path(p)
    return None


def install_dependencies():
    root_directory = get_root_directory()

    install_directory = root_directory / "third_party"
    if not install_directory.exists():
        install_directory.mkdir()

    tmp_directory = install_directory / "tmp"
    if not tmp_directory.exists():
        tmp_directory.mkdir()

    if platform.system() == "Windows":
        architecture = " -A x64"
    else:
        architecture = ""

    install_catch2(install_directory, tmp_directory, architecture)
    install_opencl(install_directory, tmp_directory, architecture)
    install_level_zero(install_directory, tmp_directory, architecture)
    cleanup_tmp_dir(tmp_directory)


def rmreadonly(func, path, exc_info):
    import stat
    import os

    if not os.access(path, os.W_OK):
        os.chmod(path, stat.S_IWUSR)
        func(path)
    else:
        raise


def cleanup_tmp_dir(tmp_directory):
    if tmp_directory.exists():
        shutil.rmtree(tmp_directory, onerror=rmreadonly)


def install_catch2(install_directory, tmp_directory, architecture, clear=False):
    catch2_install_directory = install_directory / "catch2"
    if catch2_install_directory.exists():
        if clear:
            shutil.rmtree(catch2_install_directory, onerror=rmreadonly)
        else:
            print("catch2 already installed")
            return
    catch2_tmp_directory = tmp_directory / "Catch2"
    cleanup_tmp_dir(catch2_tmp_directory)

    subprocess.run(
        "git clone https://github.com/catchorg/Catch2.git --depth 1 --branch v2.13.10",
        cwd=tmp_directory,
        shell=True,
        check=True,
    )
    subprocess.run(
        f"mkdir build && cd build && cmake .. -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX={catch2_install_directory}"
        + architecture,
        cwd=catch2_tmp_directory,
        shell=True,
        check=True,
    )
    subprocess.run(
        "cmake --build build --target install",
        cwd=catch2_tmp_directory,
        shell=True,
        check=True,
    )


def install_opencl(install_directory, tmp_directory, architecture, clear=False):
    opencl_install_directory = install_directory / "opencl"
    if opencl_install_directory.exists():
        if clear:
            shutil.rmtree(opencl_install_directory, onerror=rmreadonly)
        else:
            print("opencl already installed")
            return

    opencl_headers_tmp_directory = tmp_directory / "OpenCL-Headers"
    cleanup_tmp_dir(opencl_headers_tmp_directory)
    opencl_headers_install_directory = opencl_install_directory / "include"

    subprocess.run(
        "git clone https://github.com/KhronosGroup/OpenCL-Headers.git --depth 1 --branch v2024.10.24",
        cwd=tmp_directory,
        shell=True,
        check=True,
    )
    shutil.copytree(
        opencl_headers_tmp_directory / "CL", opencl_headers_install_directory / "CL"
    )


def install_level_zero(install_directory, tmp_directory, architecture, clear=False):
    level_zero_install_directory = install_directory / "level_zero"
    if level_zero_install_directory.exists():
        if clear:
            shutil.rmtree(level_zero_install_directory, onerror=rmreadonly)
        else:
            print("level zero already installed")
            return

    level_zero_tmp_directory = tmp_directory / "level-zero"
    cleanup_tmp_dir(level_zero_tmp_directory)
    level_zero_headers_install_directory = level_zero_install_directory / "include"

    subprocess.run(
        "git clone https://github.com/oneapi-src/level-zero.git --depth 1 --branch v1.19.2",
        cwd=tmp_directory,
        shell=True,
        check=True,
    )
    shutil.copytree(
        level_zero_tmp_directory / "include", level_zero_headers_install_directory
    )


if __name__ == "__main__":
    sys.exit(main())
