#include "json-builder.hpp"
#include "utility.hpp"
using namespace std::string_view_literals;

JsonBuilder& JsonBuilder::add(std::string_view key, std::string_view value,
                              bool quote)
{
  extend();
  session += Util::Join(
      DOUBLE_QUOTE, key, DOUBLE_QUOTE, ": ",
      (quote ? Util::Join(DOUBLE_QUOTE, value, DOUBLE_QUOTE) : value));
  return *this;
}

[[maybe_unused]] JsonBuilder& JsonBuilder::add(std::string_view key, bool value)
{
  extend();
  session += Util::Join(DOUBLE_QUOTE, key, DOUBLE_QUOTE, ": ",
                        value ? "true" : "false");
  return *this;
}

[[maybe_unused]] JsonBuilder& JsonBuilder::add(std::string_view key, int value)
{
  extend();
  session +=
      Util::Join(DOUBLE_QUOTE, key, DOUBLE_QUOTE, ": ", std::to_string(value));
  return *this;
}

inline void JsonBuilder::extend(char stamp)
{
  if (session[session.size() - 1] != stamp)
	session += ',';
}

JsonBuilder& JsonBuilder::startList(std::string_view key)
{
  return add(key, "[", false);
}

JsonBuilder& JsonBuilder::add(std::string_view value)
{
  extend('[');
  session += Util::Join(DOUBLE_QUOTE, std::string(value), DOUBLE_QUOTE);

  return *this;
}

JsonBuilder& JsonBuilder::endList()
{
  final(']');

  return *this;
}

std::string JsonBuilder::final(char stamp)
{
  if (session[session.size() - 1] != stamp)
	session += stamp;

  return session;
}