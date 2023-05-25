#pragma once

#include "binary-counter.hpp"
#include "context.hpp"
#include "logical-expression.hpp"
#include "model/logic-model.hpp"
#include "model/table-model.hpp"
#include "postfix-gen.hpp"

namespace Util
{
template <typename LTp_, typename... RTp_>
DLL_LOCAL bool orAll(const LTp_& l, const RTp_&...r)
{
  return (... || (l == r));
}

DLL_LOCAL std::wstring WSRep(wchar_t c);

DLL_LOCAL std::string Decode(unsigned val);

DLL_LOCAL size_t nByte(unsigned char c);

MAYBE_UNUSED DLL_LOCAL int Join(std::string str, size_t idx, int count);

std::wstring ToWString(std::string str);

DLL_LOCAL std::string ToU8String(std::wstring_view ws);

template <typename T>
using is_valid_arg = std::disjunction<std::is_same<T, const char *>,
                                      std::is_same<T, std::string_view>>;
template <typename... Args, typename = std::enable_if_t<
                                std::conjunction_v<is_valid_arg<Args>...>>>
std::string Join(Args&&...args)
{
  std::size_t total_size = (std::string_view(args).size() + ...);
  std::string result;
  result.reserve(total_size);
  (result.append(args), ...);
  return result;
}

template <typename... Args>
std::string Join(Args&&...args)
{
  return (std::string(args) + ...);
}

namespace error
{
class syntax_error : public std::logic_error
{
public:
  explicit syntax_error(const std::string& error) : std::logic_error(error) {}
};
} // namespace error
} // namespace Util

extern "C"
{

  DLL_EXPORT wchar_t *toWString(const char *str);
  MAYBE_UNUSED DLL_EXPORT void freeArray(wchar_t **array, size_t size);
  MAYBE_UNUSED DLL_EXPORT void freeString(wchar_t *c);
  DLL_EXPORT FILE *tty0();
}