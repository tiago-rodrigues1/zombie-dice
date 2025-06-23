#include <vector>

#include "common.hpp"
#include "ini_parser.hpp"

std::string IniParser::current_section = "";
std::map<std::string, std::map<std::string, std::string>> IniParser::config;

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