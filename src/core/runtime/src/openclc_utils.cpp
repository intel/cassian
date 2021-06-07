#include <cassian/logging/logging.hpp>
#include <cassian/runtime/openclc_utils.hpp>
#include <string>

namespace cassian {
bool check_optional_feature_support(Runtime *runtime,
                                    const std::string &program_type,
                                    const std::string &feature_macro) {

  std::string source = "#ifndef " + feature_macro +
                       "\n"
                       "#error \"Feature unsupported\"\n"
                       "#endif \n"
                       "void kernel test() {} ";
  try {
    runtime->create_kernel("test", source, " -cl-std=CL3.0", program_type);
  } catch (const RuntimeException &e) {
    logging::info() << feature_macro << " unsupported\n";
    return false;
  }
  return true;
}
} // namespace cassian