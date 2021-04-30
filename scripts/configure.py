#
# Copyright (C) 2021 Intel Corporation
#
# SPDX-License-Identifier: MIT
#

import argparse
import logging
import os
import platform
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
            enable_bullseye(args.bullseye)
        configure_cmake(
            build_directory=args.build_directory,
            source_directory=args.source_directory,
            generator=args.generator,
            ccache=args.ccache,
            extra=args.extra,
            c_compiler=args.c_compiler,
            cxx_compiler=args.cxx_compiler,
            linker=args.linker,
            shared=args.shared,
            build_type=args.build_type,
            clang_tidy=args.clang_tidy,
            iwyu=args.iwyu,
            lwyu=args.lwyu,
            sanitizer=args.sanitizer,
            docs=args.docs,
            no_tests=args.no_tests,
            packages=args.packages,
        )
    except Exception:
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
    root_parser.add_argument("--generator", help="CMake generator to use")
    root_parser.add_argument("--ccache", action="store_true", help="use ccache")
    root_parser.add_argument("--extra", nargs="*", help="extra CMake flags")
    root_parser.add_argument("--c-compiler", help="path to C compiler")
    root_parser.add_argument("--cxx-compiler", help="path to C++ compiler")
    root_parser.add_argument("--linker", help="path to linker")
    root_parser.add_argument(
        "--shared", action="store_true", help="build shared libraries"
    )
    root_parser.add_argument("--build-type", help="CMake build type to use")
    root_parser.add_argument("--clang-tidy", action="store_true", help="use clang-tidy")
    root_parser.add_argument(
        "--iwyu", action="store_true", help="use include-what-you-use"
    )
    root_parser.add_argument(
        "--lwyu", action="store_true", help="use link-what-you-use"
    )
    root_parser.add_argument(
        "--sanitizer",
        choices=["asan", "tsan", "ubsan", "lsan"],
        help="use clang sanitizer",
    )
    root_parser.add_argument("--docs", action="store_true", help="build documentation")
    root_parser.add_argument(
        "--no-tests", action="store_true", help="do not build unit tests"
    )
    root_parser.add_argument(
        "--packages", help="package types to prepare, separated by ;"
    )
    root_parser.add_argument(
        "--code-coverage", action="store_true", help="enable Bullseye coverage"
    )
    root_parser.add_argument(
        "--bullseye",
        type=Path,
        default=Path("/opt/BullseyeCoverage"),
        help="path to Bullseye installation directory",
    )


def enable_bullseye(bullseye_path):
    os.environ["PATH"] = os.pathsep.join(
        [str(bullseye_path / "bin"), os.environ["PATH"]]
    )


def configure_cmake(
    build_directory,
    source_directory,
    generator=None,
    ccache=None,
    extra=None,
    c_compiler=None,
    cxx_compiler=None,
    linker=None,
    shared=None,
    build_type=None,
    clang_tidy=None,
    iwyu=None,
    lwyu=None,
    sanitizer=None,
    docs=None,
    no_tests=None,
    packages=None,
):
    if not extra:
        extra = ""
    else:
        extra = " ".join(extra)
    if clang_tidy or iwyu:
        extra += " -DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    if iwyu:
        extra += ' -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE="iwyu;-Xiwyu;--no_fwd_decls"'
    if lwyu:
        extra += " -DCMAKE_LINK_WHAT_YOU_USE=ON"
    if sanitizer == "asan":
        extra += " -DUSE_ASAN=ON"
    elif sanitizer == "tsan":
        extra += " -DUSE_TSAN=ON"
    elif sanitizer == "ubsan":
        extra += " -DUSE_UBSAN=ON"
    elif sanitizer == "lsan":
        extra += " -DUSE_LSAN=ON"
    if docs:
        extra += " -DBUILD_DOCS=ON"
    if no_tests:
        extra += " -DBUILD_TESTING=OFF"
    if packages:
        extra += f' -DCPACK_GENERATOR="{packages}"'
    if sanitizer:
        if not build_type:
            build_type = "RelWithDebInfo"

    cmd = f"cmake -B {build_directory} -S {source_directory}"
    if generator:
        cmd += f' -G "{generator}"'
    if ccache:
        cmd += " -DCMAKE_CXX_COMPILER_LAUNCHER=ccache"
    if extra:
        cmd += f" {extra}"
    if c_compiler:
        cmd += f" -DCMAKE_C_COMPILER={c_compiler}"
    if cxx_compiler:
        cmd += f" -DCMAKE_CXX_COMPILER={cxx_compiler}"
    if linker:
        cmd += f" -DCMAKE_LINKER={linker}"
    if shared:
        cmd += " -DBUILD_SHARED_LIBS=ON"
        if platform.system() == "Windows":
            cmd += " -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON"
    if build_type:
        cmd += f" -DCMAKE_BUILD_TYPE={build_type}"
    logger.info("CMake command line: %s", cmd)

    process = subprocess.run(
        cmd, shell=True, text=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
    )
    output = process.stdout
    logger.info("CMake output:\n%s", output)
    process.check_returncode()


if __name__ == "__main__":
    sys.exit(main())
