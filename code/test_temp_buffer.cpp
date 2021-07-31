#include <iostream>
#include <cstdlib>

std::pair<void*, size_t> get_temporary_buffer(size_t n) { 
  // this is bogus code and needs to be replaced
  void* buffer = malloc(n);
  while (!buffer && n) {
    n = n >> 1;
    buffer = malloc(n);
  }
  return std::make_pair(buffer, n);
}

int main(int argc, const char* argv[])
{
  // 64 GB
  size_t starting_amount = 1024UL * 1024 * 1024 * 64;
  std::pair<void*, size_t> result = get_temporary_buffer(starting_amount);

  std::cout << "allocated: " << result.second << std::endl;
}
