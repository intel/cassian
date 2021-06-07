/*
 * Copyright (C) 2021 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include <algorithm>
#include <cassian/utility/utility.hpp>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#endif
namespace cassian {

std::string load_text_file(const std::string &file_path) {
  std::ifstream stream(file_path.c_str());

  std::string text_file;
  if (!stream.good()) {
    return text_file;
  }

  text_file = std::string((std::istreambuf_iterator<char>(stream)),
                          std::istreambuf_iterator<char>());
  return text_file;
}

void save_text_file(const std::string &data, const std::string &file_path) {
  std::ofstream ofs(file_path);
  ofs << data;
  ofs.close();
}

std::vector<uint8_t> load_binary_file(const std::string &file_path) {
  std::ifstream stream(file_path, std::ios::in | std::ios::binary);

  std::vector<uint8_t> binary_file;
  if (!stream.good()) {
    return binary_file;
  }

  size_t length = 0;
  stream.seekg(0, std::ifstream::end);
  length = static_cast<size_t>(stream.tellg());
  stream.seekg(0, std::ifstream::beg);

  binary_file.resize(length);
  stream.read(reinterpret_cast<char *>(binary_file.data()), length);

  return binary_file;
}

void save_binary_file(const std::vector<uint8_t> &data,
                      const std::string &file_path) {
  std::ofstream stream(file_path, std::ios::out | std::ios::binary);
  stream.write(reinterpret_cast<const char *>(data.data()),
               sizeof(uint8_t) * data.size());
}

fs::path get_application_directory() {
#if defined(_WIN32)
  char result[MAX_PATH];
  const auto count = GetModuleFileName(NULL, result, MAX_PATH);
#else
  char result[PATH_MAX];
  const auto count = readlink("/proc/self/exe", result, PATH_MAX);
#endif
  if (count > 0) {
    fs::path app_dir = std::string(result, count);
    return app_dir.parent_path();
  }
  throw PathNotFoundException("Failed to find application directory");
}

std::string get_asset(const std::string &asset_path) {
  fs::path path = asset_path;

  if (fs::exists(path)) {
    return path.string();
  }

  fs::path share_path = "../share/cassian";
  path = share_path / asset_path;
  if (fs::exists(path)) {
    return path.string();
  }

  path = get_application_directory() / share_path / asset_path;
  if (fs::exists(path)) {
    return path.string();
  }

  path = get_application_directory() / asset_path;
  if (fs::exists(path)) {
    return path.string();
  }

  path = get_application_directory().parent_path() / asset_path;
  if (fs::exists(path)) {
    return path.string();
  }

  throw AssetNotFoundException("Failed to find asset: " + asset_path);
}

std::string convert_to_forward_slashes(const std::string &str) {
  auto str_to_convert = str;
  std::replace(str_to_convert.begin(), str_to_convert.end(), '\\', '/');
  return str_to_convert;
}

} // namespace cassian
