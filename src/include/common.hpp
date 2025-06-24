#ifndef COMMON_HPP
#define COMMON_HPP

#include <string>
#include <vector>

std::string ltrim(const std::string& s, const char* t = " \t\n\r\f\v");
std::string rtrim(const std::string& s, const char* t = " \t\n\r\f\v");
std::string trim(const std::string& s, const char* t = " \t\n\r\f\v");
std::vector<std::string> split(const std::string& str, char delimiter = ' ', bool trim_tokens = false);

template <typename Iterator>
void shuffler(Iterator first, Iterator last);

#endif

#include <algorithm>    // std::shuffle
#include <chrono>       // std::chrono::system_clock
#include <random>       // std::default_random_engine
template <typename Iterator>
inline void shuffler(Iterator first, Iterator last) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  std::shuffle(first, last, std::default_random_engine(seed));
}
