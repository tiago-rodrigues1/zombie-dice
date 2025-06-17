#include <fstream>
#include <iostream>

#include "ini_parser.hpp"

std::string IniParser::current_section = "";
std::map<std::string, std::map<std::string, std::string>> IniParser::config;

/**
 * Trims characters from the beginning of a string.
 *
 * @param s The input string.
 * @param t A C-string containing the characters to trim.
 * @return A new string with leading characters removed.
 */

std::string ltrim(const std::string& s, const char* t = " \t\n\r\f\v") {
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
std::string rtrim(const std::string& s, const char* t = " \t\n\r\f\v") {
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
std::string trim(const std::string& s, const char* t = " \t\n\r\f\v") {
  std::string clone{ s };
  clone = ltrim(clone, t);
  clone = rtrim(clone, t);

  return clone;
}

void IniParser::update_config(const std::string& key, const std::string& value) {
  config[current_section][key] = value;
}

void IniParser::parse_line(const std::string& line) {
  if (line.size() == 0) {
    return;
  }

  std::string section;
  std::string key;
  std::string value;

  bool any_equal_found { false };

  for (size_t i{ 0 }; i < line.size(); ++i) {
    if (line[i] == ';' or line[i] == '#') {
      return;
    }

    if (line[i] == '=' and !any_equal_found) {
      key.assign(line, 0, (i - 1));
      key = trim(key);

      value.assign(line, i + 1, line.size());
      value = trim(value);

      any_equal_found = true;
    }

    if (line[i] == '[') {
      size_t section_end{ line.find_first_of(']', i) };

      section.assign(line, (i + 1), (section_end - 1));
      current_section = section;

      return;
    }
  }

  update_config(key, value);
}

std::map<std::string, std::map<std::string, std::string>> IniParser::parse(std::ifstream& ini) {
  std::string line;

  if (ini.is_open()) {
    while (std::getline(ini, line)) {
      line = trim(line);
      
      parse_line(line);
    }

    ini.close();
  }

  return config;
}