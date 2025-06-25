#ifndef INI_PARSER_HPP
#define INI_PARSER_HPP

#include <fstream>
#include <string>
#include <map>


class IniParser {
private:
  static std::string current_section;
  static std::map<std::string, std::map<std::string, std::string>> config;
  static std::string get_value(const std::string& accessor);
  static void update_config(const std::string& key, const std::string& value);
  static void parse_line(const std::string& line);
public:
  static bool parse(const std::string& ini_path);
  
  template <typename T>
  static T get_config(const std::string& accessor, const T& default_value);
};
#endif
