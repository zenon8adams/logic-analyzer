#ifndef LOGIC_MODEL_HPP
#define LOGIC_MODEL_HPP

#include "config.hpp"
#include "logical-expression.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Logic
{
class DLL_EXPORT LogicModel
{
public:
  explicit LogicModel(std::vector<std::wstring> postfix);

  [[nodiscard]] auto Model() const { return model; }

  [[nodiscard]] auto NumberOfVariable() const { return nVars; }

private:
  void buildModel();

  wchar_t getOp(std::wstring& str);

private:
  std::vector<std::wstring> token;

  std::vector<std::shared_ptr<LogicalExp>> model;

  size_t idx{}, nVars{};
};
} // namespace Logic

extern "C"
{
  using LogicalExpCollection = std::vector<std::shared_ptr<Logic::LogicalExp>>;

  MAYBE_UNUSED DLL_EXPORT Logic::LogicModel *makeLogicModel(wchar_t **array,
                                                            size_t size);
  DLL_EXPORT void model(Logic::LogicModel *lmodel,
                        LogicalExpCollection *obj_coll);
  MAYBE_UNUSED DLL_EXPORT size_t numberOfVariable(Logic::LogicModel *lexp);
  DLL_EXPORT void destroyLogicModelObject(Logic::LogicModel *lmodel);
}

#endif