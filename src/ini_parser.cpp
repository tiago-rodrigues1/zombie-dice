#include <iostream>
#include <sstream>
#include <vector>

#include "common.hpp"
#include "ini_parser.hpp"

std::string IniParser::current_section = "";
std::map<std::string, std::map<std::string, std::string>> IniParser::config;

void throw_warning(const std::string& msg, const std::string& var) {
  std::ostringstream warn_msg;
  warn_msg << "[warning] " << msg << ": " << var << '\n';

  throw std::invalid_argument(warn_msg.str());
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
      throw_warning("invalid section key", section_key);
    }

    current_section = section_key;

    return;
  }

  std::vector<std::string> split_result{ split(line, '=') };

  if (split_result.size() <= 1) {
    throw std::invalid_argument("[warning]: key and value bad format");
  }

  std::string key{ trim(split_result[0]) };
  std::string value{ remove_quotes(trim(split_result[1])) };

  if (!is_key_valid(key)) {
    throw_warning("invalid key", key);
  }

  update_config(key, value);
}

bool IniParser::parse(const std::string& ini_path) {
  try {
    std::ifstream ini{ ini_path };

    if (!ini.good()) {
      throw_warning("could not read", ini_path);
    }

    std::string line;

    if (ini.is_open()) {
      while (std::getline(ini, line)) {
        line = trim(line);
        line = remove_comments(line);
        
        try {
          parse_line(line);
        } catch (const std::exception& e) {
          std::cerr << e.what();
          continue;
        }
      }

      ini.close();
    }

    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what();
    return false;
  }
  
}

std::string IniParser::get_value(const std::string& accessor) {
  std::vector<std::string> keys{ split(accessor, '.') };

  if (keys.size() > 2) {
    throw_warning("the parser does not support nested sections", accessor);
  } else if (keys.size() == 2) {
    auto it_section{ config.find(keys[0]) };

    if (it_section == config.end()) {
      throw_warning("key section not found", keys[0]);
    }

    auto it_key{ config[keys[0]].find(keys[1]) };
    if (it_key == config[keys[0]].end()) {
      throw_warning("key not found", keys[1]);
    }

    return it_key->second;
  }

  auto it_key{ config["root"].find(keys[0]) };

  if (it_key == config["root"].end()) {
    throw_warning("key not found", keys[0]);
  }

  return it_key->second;
}

template<>
int IniParser::get_config<int>(const std::string& accessor, const int& default_value) {
  try {
    int num = std::stoi(get_value(accessor));
    return num;
  } catch (const std::exception& e) {
    std::cerr << e.what() << "--> using default: " << default_value << '\n';
    return default_value;
  }
}

template<>
std::string IniParser::get_config<std::string>(const std::string& accessor, const std::string& default_value) {
  try {
    return get_value(accessor);
  } catch (const std::exception& e) {
    std::cerr << e.what() << "--> using default: " << default_value << '\n';
    return default_value;
  }
}

template<>
double IniParser::get_config<double>(const std::string& accessor, const double& default_value) {
  try {
    double num = std::stof(get_value(accessor));
    return num;
  } catch (const std::exception& e) {
    std::cerr << e.what() << "--> using default: " << default_value << '\n';
    return default_value;
  }
}