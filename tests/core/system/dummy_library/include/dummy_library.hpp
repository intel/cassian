/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef DUMMY_LIBRARY_HPP
#define DUMMY_LIBRARY_HPP

#if defined(_WIN32)
#define DUMMY_LIBRARY_API __declspec(dllexport)
#define DUMMY_LIBRARY_API_CALL __stdcall
#elif defined(__linux__)
#define DUMMY_LIBRARY_API
#define DUMMY_LIBRARY_API_CALL
#endif

extern "C" {
DUMMY_LIBRARY_API int DUMMY_LIBRARY_API_CALL dummy_function(int x);
}

#endif
