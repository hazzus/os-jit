#include "function.hpp"
#include <cstring>
#include <iostream>

#include <sys/mman.h>
#include <unistd.h>

void error(std::string const &cause) {
  std::cerr << "Error: " << cause << " " << strerror(errno) << std::endl;
}

template <typename f_ptr, typename val_type> struct function_wrapper {
  function_wrapper(unsigned char code[], size_t cs)
      : _data(mmap(nullptr, cs, PROT_READ | PROT_WRITE | PROT_EXEC,
                   MAP_PRIVATE | MAP_ANON, -1, 0)),
        code_size(cs) {
    if (_data == MAP_FAILED) {
      error("Unable to map memory");
      return;
    }
    std::memcpy(_data, code, code_size);
  }

  void eval(val_type value) const {
    if (mprotect(_data, code_size, PROT_EXEC) == -1) {
      error("Unable to make mapped memory executable");
      return;
    }
    auto result = ((f_ptr)_data)(value);
    std::cout << "Function evaluation returned: " << result << std::endl;
  }

  ~function_wrapper() {
    if (munmap(_data, code_size) == -1) {
      error("Unable to unmap memory");
    }
  }

private:
  void *_data = nullptr;
  size_t code_size;
};

int main() {
  std::cout << "This programm jit-compiles the code of mult by 5(or your's "
               "multiplier) function\n";
  std::cout << "Do you want to change multiplier? [N/y]: ";
  char agreement;
  std::cin.get(agreement);
  if (agreement == 'y') {
    std::cout << "Insert new multiplier: ";
    func::val_type patch;
    std::cin >> patch;
    func::patch_code(patch);
  }
  function_wrapper<func::fptr_type, func::val_type> f(func::code,
                                                      func::code_size);
  func::val_type value;
  std::cout << "Insert value to multiply: ";
  std::cin >> value;
  f.eval(value);
}
