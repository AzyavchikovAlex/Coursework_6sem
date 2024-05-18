#include <bits/stdc++.h>

int main() {

  std::string numbers;

  const std::regex ws_re("\\s+"); // whitespace
  while (true) {
    std::getline(std::cin, numbers);
    std::vector<std::string> result
        (std::sregex_token_iterator(numbers.begin(), numbers.end(), ws_re, -1),
         std::sregex_token_iterator());
    std::cerr << "[";
    for (auto& str : result) {
      std::cerr << str << ", ";
    }
    std::cerr << "]\n";
  }
}