#include "include/common.hpp"

#include <iostream>
#include <sstream>      // std::istringstream
#include <string>

/**
 * Trims characters from the beginning of a string.
 *
 * @param s The input string.
 * @param t A C-string containing the characters to trim.
 * @return A new string with leading characters removed.
 */

std::string ltrim(const std::string& s, const char* t) {
  std::string clone{ s };
  clone.erase(0, clone.find_first_not_of(t));
  return clone;
}

/**
 * Trims characters from the end of a string.
 *
 * @param s The input string.
 * @param t A C-string containing the characters to trim.
 * @return A new string with trailing characters removed.
 */
std::string rtrim(const std::string& s, const char* t) {
  std::string clone{ s };
  clone.erase(clone.find_last_not_of(t) + 1);
  return clone;
}

/**
 * Applies ltrim and rtrim in sequence.
 *
 * @param s The input string.
 * @param t A C-string containing the characters to trim.
 * @return A new string with leading and trailing characters removed.
 */
std::string trim(const std::string& s, const char* t) {
  std::string clone{ s };
  clone = ltrim(clone, t);
  clone = rtrim(clone, t);

  return clone;
}

std::vector<std::string> split(const std::string& str, char delimiter, bool trim_tokens) {
  std::vector<std::string> tokens;

  std::istringstream iss;
  iss.str(str);

  std::string token;

  while(std::getline(iss >> std::ws, token, delimiter)) {
    if (trim_tokens) {
      token = trim(token);
    }

    tokens.emplace_back(token);
  }

  return tokens;
}