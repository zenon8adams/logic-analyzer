#ifndef POSTFIX_GEN_HPP
#define POSTFIX_GEN_HPP

#include "logical-expression.hpp"
#include <functional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

namespace Logic
{
class DLL_EXPORT PostfixGenerator
{
public:
  explicit PostfixGenerator(const std::wstring& input);
  auto postfixForm() const { return postfix; }

private:
  void consumeWs();
  std::wstring getVariable();
  static bool isOperator(wchar_t c);
  static bool mayBeSpecialOp(wchar_t c);
  void generate();
  void reOrder(int c);
  static bool hasHigherPrecedence(int l, int r);
  bool unwindStackUntil(const std::function<bool(int)>& op);

private:
  std::wstringstream strm;
  std::vector<int> oprStack;
  std::vector<std::wstring> postfix;
  static const std::unordered_map<int, int> precedenceTable;
  static const std::unordered_map<int, std::pair<std::string, Operator>>
      replOps;
};
} // namespace Logic

extern "C"
{

  MAYBE_UNUSED DLL_EXPORT Logic::PostfixGenerator *
  makePostfixGenerator(const wchar_t *input);
  DLL_EXPORT wchar_t **postfixForm(Logic::PostfixGenerator *gen, size_t *len);
  DLL_EXPORT void destroyPostfixObject(Logic::PostfixGenerator *obj);
}

#endif