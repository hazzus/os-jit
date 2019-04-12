#ifndef FUNCTION
#define FUNCTION

#include <cassert>
#include <cstring>
#include <string>
namespace func {

using val_type = int32_t;
using fptr_type = int32_t (*)(val_type);

val_type mult_by(val_type a) {
  const val_type b = 5;
  return a * b;
}

const size_t mult_pos[] = {10, 19};
// clang-format off
static unsigned char code[] = {
       0x55                                     // push   %rbp
      ,0x48 ,0x89 ,0xe5                         // mov    %rsp,        %rbp
      ,0x89 ,0x7d ,0xec                         // mov    %edi,        -0x14(%rbp)
      ,0xc7 ,0x45 ,0xfc ,0x05 ,0x00 ,0x00 ,0x00 // movl   $0x05,       -0x4(%rbp)
      ,0x8b ,0x45 ,0xec                         // mov    -0x14(%rbp), %eax
      ,0x69 ,0xc0 ,0x05 ,0x00 ,0x00 ,0x00       // imul   $0x05,       %eax,       %eax
      ,0x5d                                     // pop    %rbp
      ,0xc3                                     // retq
};
// clang-format on

const size_t code_size = sizeof(code);

void patch_code(val_type x) {
  // val_type is int, actually
  std::memcpy(code + mult_pos[0], &x, 4);
  std::memcpy(code + mult_pos[1], &x, 4);
}
} // namespace func
#endif
