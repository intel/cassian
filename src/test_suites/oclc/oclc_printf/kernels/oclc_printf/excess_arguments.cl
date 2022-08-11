/*
 * Copyright (C) 2023 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

char modify_char() { return '1'; }

kernel void test_printf_excess_arguments_evaluation(global char *output) {
  const int gid = get_global_id(0);
  char evaluated = '0';
  int i = 0;
  printf("text with exhausted format\n", i++, evaluated = modify_char());
  if (i == 1) {
    output[gid] = evaluated;
  } else {
    output[gid] = '0';
  }
}
