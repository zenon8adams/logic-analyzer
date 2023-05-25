#include "context.hpp"

void Logic::Context::Lookup(std::wstring_view str, bool& result)
{
  auto value = Lookup().find(static_cast<std::wstring>(str));
  if (value != Lookup().cend())
	result = value->second;
}
void Logic::Context::Assign(std::wstring_view s, bool b)
{
  Lookup().emplace(s, b);
}
std::unordered_map<std::wstring, bool>& Logic::Context::Lookup()
{
  static std::unordered_map<std::wstring, bool> varTable;
  return varTable;
}

void Assign(const wchar_t *str, bool result)
{
  Logic::Context::Assign(str, result);
}