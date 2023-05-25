#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include "config.hpp"
#include <string>
#include <unordered_map>

namespace Logic
{
class DLL_LOCAL Context
{
public:
  static DLL_LOCAL void Lookup(std::wstring_view str, bool& result);
  DLL_LOCAL static void Assign(std::wstring_view s, bool b);

private:
  static std::unordered_map<std::wstring, bool>& Lookup();
};
} // namespace Logic

extern "C"
{
  DLL_EXPORT void Assign(const wchar_t *str, bool result);
}

#endif