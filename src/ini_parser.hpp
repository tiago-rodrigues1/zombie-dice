#ifndef INI_PARSER_HPP
#define INI_PARSER_HPP

#include <string>
#include <map>

class IniParser {
private:
  static std::string current_section;
  static std::map<std::string, std::map<std::string, std::string>> config;
  static void update_config(const std::string& key, const std::string& value);
  static void parse_line(const std::string& line);
public:
  static std::map<std::string, std::map<std::string, std::string>> parse(std::ifstream& ini);
};
#endif