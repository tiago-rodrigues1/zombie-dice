#include <fstream>
#include <iostream>
#include <sstream>      // std::istringstream
#include <vector>

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

std::string remove_comments(std::string& l, const char* c = "#;") {
  std::string clone{ l };
  size_t pos = clone.find_first_of(c);

  if (pos != std::string::npos) {
    clone.erase(pos);
  }

  return clone;
}

std::string remove_quotes(const std::string& l) {
  std::string clone{ l };

  if (clone.front() == '\"' or clone.front() == '\'') {
    clone.erase(0, 1);
  }

  if (!clone.empty() and clone.back() == '\"' or clone.back() == '\'') {
    clone.pop_back();
  }

  return clone;
}

std::vector<std::string> split(const std::string& str, char delimiter = ' ') {
  std::vector<std::string> tokens;

  std::istringstream iss;
  iss.str(str);

  std::string token;

  while(std::getline(iss >> std::ws, token, delimiter)) {
    tokens.emplace_back(token);
  }

  return tokens;
}

bool is_key_valid(const std::string& key) {
  if (key.empty()) {
    return false;
  }

  for (char ch : key) {
    if (!std::isalnum(ch) && ch != '_' && ch != '-') {
      return false;
    }
  }

  return true;
}


void IniParser::update_config(const std::string& key, const std::string& value) {
  if (current_section.empty()) {
    config["root"][key] = value;
  } else {
    config[current_section][key] = value;
  }
}

void IniParser::parse_line(const std::string& line) {
  if (line.size() == 0) {
    return;
  }
  
  if (line[0] == '[') {
    std::string section_key;
    size_t section_end{ line.find_first_of(']') - 1 };

    section_key.assign(line, 1, section_end);

    section_key = trim(section_key);

    if (!is_key_valid(section_key)) {
      throw std::invalid_argument("Invalid section key");
    }

    current_section = section_key;

    return;
  }

  std::vector<std::string> split_result{ split(line, '=') };

  if (split_result.size() <= 1) {
    throw std::invalid_argument("Invalid key and value");
  }

  std::string key{ trim(split_result[0]) };
  std::string value{ remove_quotes(trim(split_result[1])) };

  if (!is_key_valid(key)) {
    std::string msg = key + " is an invalid key";
    throw std::invalid_argument(msg);
  }

  update_config(key, value);
}

void IniParser::parse(std::ifstream& ini) {
  std::string line;

  if (ini.is_open()) {
    while (std::getline(ini, line)) {
      line = trim(line);
      line = remove_comments(line);
      
      parse_line(line);
    }

    ini.close();
  }
}

std::string IniParser::get_value(const std::string& accessor) {
  std::vector<std::string> keys{ split(accessor, '.') };

  if (keys.size() > 2) {
    throw std::invalid_argument("Invalid accessor");
  } else if (keys.size() == 2) {
    auto it_section{ config.find(keys[0]) };

    if (it_section == config.end()) {
      throw std::invalid_argument("Key Section does not exists");
    }

    auto it_key{ config[keys[0]].find(keys[1]) };
    if (it_key == config[keys[0]].end()) {
      throw std::invalid_argument("Key does not exists");
    }

    return it_key->second;

  } else {
    auto it_key{ config["root"].find(keys[0]) };

    if (it_key == config["root"].end()) {
      throw std::invalid_argument("Key does not exists");
    }

    return it_key->second;
  }
}

template<>
int IniParser::get_config<int>(const std::string& accessor) {
  int num = std::stoi(get_value(accessor));

  return num;
}

template<>
std::string IniParser::get_config<std::string>(const std::string& accessor) {
  return get_value(accessor);
}

template<>
double IniParser::get_config<double>(const std::string& accessor) {
  double num = std::stof(get_value(accessor));

  return num;
}