#ifndef JSON_BUILDER_HPP
#define JSON_BUILDER_HPP

#include <iomanip>

class JsonBuilder
{
public:
  JsonBuilder& add(std::string_view key, std::string_view value,
                   bool quote = true);

  [[maybe_unused]] JsonBuilder& add(std::string_view key, bool value);

  [[maybe_unused]] JsonBuilder& add(std::string_view key, int value);

  inline void extend(char stamp = '{');

  JsonBuilder& startList(std::string_view key);

  JsonBuilder& add(std::string_view value);

  JsonBuilder& endList();

  std::string final(char stamp = '}');

private:
  std::string session = "{";
  static constexpr std::string_view DOUBLE_QUOTE = R"(")";
};

#endif // JSON_BUILDER_HPP
